#include "core/agent.h"
#include "services/command_service.h"
#include "services/file_service.h"
#include "services/web_service.h"
#include "services/ai_service.h"
#include "data/memory_manager.h"
#include "utils/ui.h"
#include "utils/config.h"
#include "version.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <algorithm>
#include <cctype>

// Define simple integer mode constants so implementation is clear and independent of the header enum layout.
// If you already have enum Mode in core/agent.h, ensure the values match these constants or remove these and use the enum.
constexpr int MODE_UNSET      = 0;
constexpr int MODE_TOGETHER   = 1;
constexpr int MODE_LLAMA_3B   = 2;
constexpr int MODE_CEREBRAS   = 3;
constexpr int MODE_LLAMA_LATEST = 4;

namespace Core {
    Agent::Agent() : mode_(MODE_UNSET), memory_(std::make_unique<Data::MemoryManager>()) {}
    Agent::~Agent() = default;

    // Helper: trim whitespace
    static inline std::string trim_copy(const std::string& s) {
        size_t a = 0;
        while (a < s.size() && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
        size_t b = s.size();
        while (b > a && std::isspace(static_cast<unsigned char>(s[b-1]))) --b;
        return s.substr(a, b - a);
    }

    void Agent::run() {
        initialize_mode();

        // Show enhanced ready interface with system info and quick help
        std::string mode_name = (mode_ == MODE_TOGETHER || mode_ == MODE_CEREBRAS) ? "Online" : "Offline";
        std::string model_name;
        switch (mode_) {
            case MODE_TOGETHER: model_name = "Together AI"; break;
            case MODE_CEREBRAS: model_name = "Cerebras"; break;
            case MODE_LLAMA_3B: model_name = "llama3.2:3b"; break;
            case MODE_LLAMA_LATEST: model_name = "llama3.2:latest"; break;
            default: model_name = "Unknown"; break;
        }
        Utils::UI::print_ready_interface(mode_name, model_name);

        while (true) {
            std::cout << "> ";
            std::string user_input;
            if (!std::getline(std::cin, user_input)) {
                break; // EOF or error
            }

            user_input = trim_copy(user_input);
            if (user_input.empty()) continue;

            if (user_input == "exit" || user_input == "quit") break;
            if (user_input == "help") {
                Utils::UI::print_help();
                continue;
            }
            if (user_input == "version") {
                Version::print_version_info();
                continue;
            }

            process_user_input(user_input);
        }

        std::cout << "Goodbye\n";
    }

    void Agent::initialize_mode() {
        // Simplified mode selection with consistent numbering
        int choice = get_user_choice("Mode [1=Online / 2=Offline] (default 2): ", {1, 2}, 2);

        if (choice == 1) {
            // Online mode: pick provider
            int provider = get_user_choice("Provider [1=Together AI / 2=Cerebras] (default 1): ", {1, 2}, 1);

            if (provider == 1) {
                mode_ = MODE_TOGETHER;
                api_key_ = Utils::Config::get_env_var("TOGETHER_API_KEY");
                if (api_key_.empty()) {
                    throw std::runtime_error("TOGETHER_API_KEY not set");
                }
                Utils::UI::print_success("Together AI");
            } else {
                mode_ = MODE_CEREBRAS;
                api_key_ = Utils::Config::get_env_var("CEREBRAS_API_KEY");
                if (api_key_.empty()) {
                    throw std::runtime_error("CEREBRAS_API_KEY not set");
                }
                Utils::UI::print_success("Cerebras");
            }
        } else {
            // Offline mode: pick model
            int model = get_user_choice("Model [1=llama3.2:3b / 2=llama3.2:latest] (default 1): ", {1, 2}, 1);

            mode_ = (model == 1) ? MODE_LLAMA_3B : MODE_LLAMA_LATEST;
            Utils::UI::print_success((model == 1) ? "llama3.2:3b" : "llama3.2:latest");
        }
    }

    int Agent::get_user_choice(const std::string& prompt, const std::vector<int>& valid_choices, int default_choice) {
        while (true) {
            std::cout << prompt;
            std::string input;
            if (!std::getline(std::cin, input)) {
                // EOF -> return default
                return default_choice;
            }

            input = trim_copy(input);

            // Accept empty line as confirmation of default choice
            if (input.empty()) return default_choice;

            try {
                int choice = std::stoi(input);
                if (std::find(valid_choices.begin(), valid_choices.end(), choice) != valid_choices.end()) {
                    return choice;
                }
            } catch (const std::exception&) {
                // fallthrough -> invalid input, prompt again
            }
            Utils::UI::print_warning("Invalid choice, please try again.");
        }
    }

    void Agent::process_user_input(const std::string& input) {
        // Check for direct commands (detect colon)
        if (input.find(':') != std::string::npos) {
            handle_direct_command(input);
        } else {
            handle_ai_chat(input);
        }
    }

    void Agent::handle_direct_command(const std::string& input) {
        std::string result;

        if (input.rfind("search:", 0) == 0) {
            std::string query = trim_copy(input.substr(7));
            result = Services::WebService::search(query);
            memory_->save_interaction("search:" + query, result);
        }
        else if (input.rfind("cmd:", 0) == 0) {
            std::string command = trim_copy(input.substr(4));
            result = Services::CommandService::execute(command);
            memory_->save_interaction("cmd:" + command, result);
        }
        else if (input.rfind("read:", 0) == 0) {
            std::string filename = trim_copy(input.substr(5));
            result = Services::FileService::read_file(filename);
            memory_->save_interaction("read:" + filename, result);
        }
        else if (input.rfind("write:", 0) == 0) {
            // Format: write:filename content...
            // Find first space after the "write:" prefix
            size_t space_pos = input.find(' ', 6); // index 6 is safe for "write:"
            if (space_pos != std::string::npos) {
                std::string filename = trim_copy(input.substr(6, space_pos - 6));
                std::string content = input.substr(space_pos + 1);
                result = Services::FileService::write_file(filename, content);
                memory_->save_interaction("write:" + filename, result);
            } else {
                result = "Usage: write:filename content";
            }
        }
        else {
            result = "Unknown command";
        }

        if (!result.empty()) {
            std::cout << result << std::endl;
        }
    }

    void Agent::handle_ai_chat(const std::string& input) {
        if (!ai_service_) {
            ai_service_ = std::make_unique<Services::AIService>(mode_, api_key_);
        }

        if (!ai_service_->is_available()) {
            std::cout << "AI service unavailable\n";
            return;
        }

        std::atomic<bool> done(false);
        std::thread spin([&done]() { Utils::UI::spinner(done); });

        std::string context = memory_->get_context_string();
        std::string response = ai_service_->chat(input, context);

        done = true;
        if (spin.joinable()) spin.join();

        if (!response.empty()) {
            std::cout << response << std::endl;
            memory_->save_interaction(input, response);
        } else {
            std::cout << "No response\n";
        }
    }
} // namespace Core
