#pragma once
#include <string>
#include <memory>
#include <vector>

namespace Data { class MemoryManager; }
namespace Services { class AIService; }

namespace Core {
    class Agent {
    private:
        enum Mode {
            TOGETHER_AI = 1,
            LLAMA_3B = 2,
            CEREBRAS = 3,
            LLAMA_LATEST = 4
        };
        
        int mode_;
        std::string api_key_;
        std::unique_ptr<Data::MemoryManager> memory_;
        std::unique_ptr<Services::AIService> ai_service_;
        
        void initialize_mode();
        int get_user_choice(const std::string& prompt, const std::vector<int>& valid_choices, int default_choice);
        void process_user_input(const std::string& input);
        void handle_direct_command(const std::string& input);
        void handle_ai_chat(const std::string& input);
        
    public:
        Agent();
        ~Agent(); // Need explicit destructor for unique_ptr with forward declarations
        
        void run();
    };
}