#pragma once
#include <string>
#include <memory>
#include <vector>
#include "utils/config.h"  // For LLAMAWARE_API

namespace Data { class MemoryManager; }
namespace Services { class AIService; }

namespace Core {
    class LLAMAWARE_API Agent {
    public:
        enum Mode {
            MODE_UNSET = 0,
            MODE_TOGETHER = 1,
            MODE_LLAMA_3B = 2,
            MODE_CEREBRAS = 3,
            MODE_LLAMA_LATEST = 4,
            MODE_LLAMA_31 = 5
        };
        
    private:
        Mode mode_;
        std::string api_key_;
        bool shell_mode_;
        std::unique_ptr<Data::MemoryManager> memory_;
        std::unique_ptr<Services::AIService> ai_service_;
        int command_count_;
        long long token_usage_;
        
        void initialize_mode();
        int get_user_choice(const std::string& prompt, const std::vector<int>& valid_choices, int default_choice);
        void process_user_input(const std::string& input);
        void handle_direct_command(const std::string& input);
        void handle_ai_chat(const std::string& input);
        void handle_file_injection_command(const std::string& input);
        void handle_shell_command(const std::string& input);
        void handle_meta_command(const std::string& input);
        
        // File injection helpers
        std::string process_file_injections(const std::string& input);
        std::string read_file_or_directory(const std::string& path);
        
        // Shell mode management
        void toggle_shell_mode();
        
        // Helper methods
        bool should_skip_file(const std::string& file_path, const std::string& ext);
        void show_meta_help();
        void clear_screen();
        void handle_chat_management(const std::string& command);
        void show_available_tools();
        void show_memory_context();
        void add_to_memory(const std::string& text);
        void compress_context();
        void show_session_stats();
        void handle_context_management(const std::string& command);
        void handle_multi_file_command(const std::string& command);
        void handle_web_fetch_command(const std::string& command);
        void handle_checkpoint_command(const std::string& command);
        void handle_mcp_command(const std::string& command);
        void handle_theme_command(const std::string& command);
        void handle_auth_command(const std::string& command);
        void handle_sandbox_command(const std::string& command);
        void handle_error_command(const std::string& command);
        
    public:
        Agent();
        ~Agent(); // Need explicit destructor for unique_ptr with forward declarations
        
        void run();
    };
}