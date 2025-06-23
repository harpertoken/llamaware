#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

// ANSI colors
const std::string CYAN = "\033[36m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string RED = "\033[31m";
const std::string RESET = "\033[0m";

// Load .env file (minimal)
void load_env(const std::string& filename = ".env") {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream is_line(line);
        std::string key, value;
        if (std::getline(is_line, key, '=') && std::getline(is_line, value)) {
            key.erase(0, key.find_first_not_of(" \t\n\r"));
            key.erase(key.find_last_not_of(" \t\n\r") + 1);
            value.erase(0, value.find_first_not_of(" \t\n\r"));
            value.erase(value.find_last_not_of(" \t\n\r") + 1);
            setenv(key.c_str(), value.c_str(), 1);
        }
    }
}

// Logo
void print_logo() {
    std::cout << CYAN;
    std::cout << "\n██  ██  ██\n  ██  ██  \n" << RESET;
    std::cout << "\n\033[1;36mIngenuity x Llama Agent\033[0m\n\n";
}

// Memory functions
std::vector<std::string> load_memory(const std::string& filename = "memory.txt") {
    std::vector<std::string> memory;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) memory.push_back(line);
    }
    return memory;
}

void save_to_memory(const std::string& user_input, const std::string& response, const std::string& filename = "memory.txt") {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << "User: " << user_input << "\n";
        file << "Assistant: " << response << "\n";
    }
}

// Web search function
std::string search_web(const std::string& query) {
    const char* serp_key = std::getenv("SERPAPI_KEY");
    if (!serp_key) return "Web search not available. SERPAPI_KEY is missing.";

    std::string search_url = "https://serpapi.com/search.json?q=" + cpr::util::urlEncode(query) + "&api_key=" + serp_key;
    cpr::Response search_res = cpr::Get(cpr::Url{search_url});

    if (search_res.status_code == 200) {
        auto json = nlohmann::json::parse(search_res.text);
        std::string result;
        for (const auto& item : json["organic_results"]) {
            result += "- " + item["title"].get<std::string>() + ": " + item["snippet"].get<std::string>() + "\n";
            if (result.size() > 1000) break;
        }
        return result.empty() ? "No results found." : result;
    }

    return "Search error: " + std::to_string(search_res.status_code);
}

// Spinner
void spinner(std::atomic<bool>& done) {
    const char* chars = "|/-\\";
    int i = 0;
    std::cout << YELLOW << "↺ " << RESET;
    std::cout.flush();
    while (!done) {
        std::cout << YELLOW << "\b" << chars[i++ % 4] << RESET;
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "\b \b";
}

int main() {
    print_logo();
    load_env();

    int mode = 0;
    while (mode != 1 && mode != 2) {
        std::cout << CYAN << "Choose mode [1=Online / 2=Offline]: " << RESET;
        std::string input;
        std::getline(std::cin, input);
        if (input == "1" || input == "2") mode = std::stoi(input);
    }

    const char* api_key = std::getenv("TOGETHER_API_KEY");
    if (mode == 1 && !api_key) {
        std::cerr << RED << "Error: TOGETHER_API_KEY not set." << RESET << std::endl;
        return 1;
    }

    std::cout << GREEN << (mode == 1 ? "Online model: Llama-3.3-70B" : "Offline model: llama3.2") << RESET << std::endl;

    while (true) {
        std::cout << CYAN << "› " << RESET;
        std::string user_input;
        std::getline(std::cin, user_input);
        if (user_input.empty() || user_input == "exit") break;

        std::vector<std::string> memory = load_memory();
        std::string memory_context;
        for (const auto& m : memory) memory_context += m + "\n";

        if (user_input.rfind("search:", 0) == 0) {
            std::string query = user_input.substr(7);
            std::string web_result = search_web(query);
            std::cout << YELLOW << "[Web Result]\n" << web_result << RESET << "\n";
            memory_context += "User asked: " + query + "\nWeb result:\n" + web_result + "\n";
        }

        std::string url;
        cpr::Header headers;
        nlohmann::json payload;

        if (mode == 1) {
            url = "https://api.together.xyz/v1/chat/completions";
            headers = {
                {"Authorization", std::string("Bearer ") + api_key},
                {"Content-Type", "application/json"}
            };
            payload = {
                {"model", "meta-llama/Llama-3.3-70B-Instruct-Turbo-Free"},
                {"messages", {
                    {{"role", "system"}, {"content", "You are a helpful assistant. Here is conversation history:\n" + memory_context}},
                    {{"role", "user"}, {"content", user_input}}
                }}
            };
        } else {
            url = "http://localhost:11434/api/chat";
            headers = {{"Content-Type", "application/json"}};
            payload = {
                {"model", "llama3.2:latest"},
                {"stream", false},
                {"messages", {
                    {{"role", "system"}, {"content", "You are a helpful assistant. Here is conversation history:\n" + memory_context}},
                    {{"role", "user"}, {"content", user_input}}
                }}
            };
        }

        std::atomic<bool> done(false);
        std::thread spin(spinner, std::ref(done));
        cpr::Response res = cpr::Post(cpr::Url{url}, headers, cpr::Body{payload.dump()});
        done = true;
        spin.join();

        if (res.status_code == 200) {
            auto json = nlohmann::json::parse(res.text);
            std::string content = (mode == 1)
                ? json["choices"][0]["message"]["content"]
                : json["message"]["content"];
            std::cout << GREEN << content << RESET << "\n";
            save_to_memory(user_input, content);
        } else {
            if (mode == 2 && res.status_code == 0) {
                std::cerr << RED << "Offline server not found. Start with `ollama serve`." << RESET << std::endl;
            } else {
                std::cerr << RED << "Error " << res.status_code << ": " << res.text << RESET << std::endl;
            }
        }
    }

    return 0;
}
