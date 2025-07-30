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

namespace Core {
    Agent::Agent() : mode_(0) {}
    
    void Agent::run() {
        initialize_mode();
        
        Data::MemoryManager memory;
        Utils::UI::print_info("Agent initialized. Type 'help' for commands or 'exit' to quit.");
        
        while (true) {
            std::cout << Utils::UI::CYAN << "› " << Utils::UI::RESET;
            std::string user_input;
            if (!std::getline(std::cin, user_input)) {
                // Handle EOF (e.g., Ctrl+D or piped input ended)
                break;
            }
            
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
        
        Utils::UI::print_info("Goodbye!");
    }
    
    void Agent::initialize_mode() {
        while (mode_ != 1 && mode_ != 2) {
            std::cout << Utils::UI::CYAN << "Choose mode [1=Online / 2=Offline]: " << Utils::UI::RESET;
            std::string input;
            if (!std::getline(std::cin, input)) {
                // Handle EOF or input failure (e.g., piped input ended)
                Utils::UI::print_error("Input stream ended. Defaulting to offline mode.");
                mode_ = 2;
                break;
            }
            if (input == "1" || input == "2") {
                mode_ = std::stoi(input);
            } else if (input.empty()) {
                // Handle empty input gracefully
                continue;
            }
        }
        
        if (mode_ == 1) {
            api_key_ = Utils::Config::get_env_var("TOGETHER_API_KEY");
            if (api_key_.empty()) {
                throw std::runtime_error("TOGETHER_API_KEY not set for online mode");
            }
            Utils::UI::print_success("Online mode: Llama-3.3-70B");
        } else {
            Utils::UI::print_success("Offline mode: llama3.2");
        }
    }
    
    void Agent::process_user_input(const std::string& input) {
        // Check for direct commands
        if (input.find(':') != std::string::npos) {
            handle_direct_command(input);
        } else {
            handle_ai_chat(input);
        }
    }
    
    void Agent::handle_direct_command(const std::string& input) {
        Data::MemoryManager memory;
        
        if (input.rfind("search:", 0) == 0) {
            std::string query = input.substr(7);
            std::string result = Services::WebService::search(query);
            Utils::UI::print_info("[Web Search Result]");
            std::cout << Utils::UI::YELLOW << result << Utils::UI::RESET << std::endl;
            memory.save_interaction("search:" + query, result);
        }
        else if (input.rfind("cmd:", 0) == 0) {
            std::string command = input.substr(4);
            std::string result = Services::CommandService::execute(command);
            Utils::UI::print_info("[Command Result]");
            std::cout << Utils::UI::GREEN << result << Utils::UI::RESET << std::endl;
            memory.save_interaction("cmd:" + command, result);
        }
        else if (input.rfind("read:", 0) == 0) {
            std::string filename = input.substr(5);
            std::string result = Services::FileService::read_file(filename);
            Utils::UI::print_info("[File Content]");
            std::cout << Utils::UI::GREEN << result << Utils::UI::RESET << std::endl;
            memory.save_interaction("read:" + filename, result);
        }
        else if (input.rfind("write:", 0) == 0) {
            size_t space_pos = input.find(' ', 6);
            if (space_pos != std::string::npos) {
                std::string filename = input.substr(6, space_pos - 6);
                std::string content = input.substr(space_pos + 1);
                std::string result = Services::FileService::write_file(filename, content);
                Utils::UI::print_success(result);
                memory.save_interaction("write:" + filename, result);
            } else {
                Utils::UI::print_error("Usage: write:filename content");
            }
        }
        else {
            Utils::UI::print_error("Unknown command. Type 'help' for available commands.");
        }
    }
    
    void Agent::handle_ai_chat(const std::string& input) {
        Data::MemoryManager memory;
        Services::AIService ai_service(mode_, api_key_);
        
        if (!ai_service.is_available()) {
            Utils::UI::print_error("AI service not available. Check your configuration.");
            return;
        }
        
        std::atomic<bool> done(false);
        std::thread spin(Utils::UI::spinner, std::ref(done));
        
        std::string context = memory.get_context_string();
        std::string response = ai_service.chat(input, context);
        
        done = true;
        spin.join();
        
        if (!response.empty()) {
            std::cout << Utils::UI::GREEN << response << Utils::UI::RESET << std::endl;
            memory.save_interaction(input, response);
        } else {
            Utils::UI::print_error("Failed to get AI response");
        }
    }
}