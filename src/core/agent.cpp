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

namespace Core {
    Agent::Agent() : mode_(0), memory_(std::make_unique<Data::MemoryManager>()) {}
    
    Agent::~Agent() = default;
    
    void Agent::run() {
        initialize_mode();
        
        Utils::UI::print_info("Ready");
        
        while (true) {
            std::cout << "> ";
            std::string user_input;
            if (!std::getline(std::cin, user_input)) {
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
        
        std::cout << "Goodbye\n";
    }
    
    void Agent::initialize_mode() {
        // Simplified mode selection with consistent numbering
        int choice = get_user_choice("Mode [1=Online / 2=Offline]: ", {1, 2}, 2);
        
        if (choice == 1) {
            // Online mode
            int provider = get_user_choice("Provider [1=Together AI / 2=Cerebras]: ", {1, 2}, 1);
            
            if (provider == 1) {
                mode_ = Mode::TOGETHER_AI;
                api_key_ = Utils::Config::get_env_var("TOGETHER_API_KEY");
                if (api_key_.empty()) {
                    throw std::runtime_error("TOGETHER_API_KEY not set");
                }
                Utils::UI::print_success("Together AI");
            } else {
                mode_ = Mode::CEREBRAS;
                api_key_ = Utils::Config::get_env_var("CEREBRAS_API_KEY");
                if (api_key_.empty()) {
                    throw std::runtime_error("CEREBRAS_API_KEY not set");
                }
                Utils::UI::print_success("Cerebras");
            }
        } else {
            // Offline mode
            int model = get_user_choice("Model [1=llama3.2:3b / 2=llama3.2:latest]: ", {1, 2}, 1);
            
            mode_ = (model == 1) ? Mode::LLAMA_3B : Mode::LLAMA_LATEST;
            Utils::UI::print_success((model == 1) ? "llama3.2:3b" : "llama3.2:latest");
        }
    }
    
    int Agent::get_user_choice(const std::string& prompt, const std::vector<int>& valid_choices, int default_choice) {
        while (true) {
            std::cout << prompt;
            std::string input;
            if (!std::getline(std::cin, input)) {
                return default_choice;
            }
            
            if (input.empty()) continue;
            
            try {
                int choice = std::stoi(input);
                if (std::find(valid_choices.begin(), valid_choices.end(), choice) != valid_choices.end()) {
                    return choice;
                }
            } catch (const std::exception&) {
                // Invalid input, continue loop
            }
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
        std::string result;
        
        if (input.rfind("search:", 0) == 0) {
            std::string query = input.substr(7);
            result = Services::WebService::search(query);
            memory_->save_interaction("search:" + query, result);
        }
        else if (input.rfind("cmd:", 0) == 0) {
            std::string command = input.substr(4);
            result = Services::CommandService::execute(command);
            memory_->save_interaction("cmd:" + command, result);
        }
        else if (input.rfind("read:", 0) == 0) {
            std::string filename = input.substr(5);
            result = Services::FileService::read_file(filename);
            memory_->save_interaction("read:" + filename, result);
        }
        else if (input.rfind("write:", 0) == 0) {
            size_t space_pos = input.find(' ', 6);
            if (space_pos != std::string::npos) {
                std::string filename = input.substr(6, space_pos - 6);
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
        std::thread spin(Utils::UI::spinner, std::ref(done));
        
        std::string context = memory_->get_context_string();
        std::string response = ai_service_->chat(input, context);
        
        done = true;
        spin.join();
        
        if (!response.empty()) {
            std::cout << response << std::endl;
            memory_->save_interaction(input, response);
        } else {
            std::cout << "No response\n";
        }
    }
}