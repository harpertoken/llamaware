#pragma once
#include <string>
#include <memory>

namespace Core {
    class Agent {
    private:
        int mode_;
        std::string api_key_;
        
        void initialize_mode();
        void process_user_input(const std::string& input);
        void handle_direct_command(const std::string& input);
        void handle_ai_chat(const std::string& input);
        
    public:
        Agent();
        ~Agent() = default;
        
        void run();
    };
}