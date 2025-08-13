#pragma once
#include <string>
#include <atomic>

namespace Utils {
    // Color namespace
    namespace Color {
        extern const std::string RESET;
        extern const std::string GREEN;
        extern const std::string YELLOW;
        extern const std::string RED;
        extern const std::string CYAN;
        extern const std::string BOLD;
        extern const std::string DIM;
    }

    // UI namespace
    namespace UI {
        // Core functions
        void print_logo();
        void print_help();

        void print_enterprise_status();
        void spinner(const std::string& message, int duration_ms);
        void spinner(std::atomic<bool>& done);  // For threaded spinner
        
        // Status messages
        void print_success(const std::string& message);
        void print_error(const std::string& message);
        void print_warning(const std::string& message);
        void print_info(const std::string& message);
        
        // Utility functions
        void print_divider();
        void print_quick_help();
        void print_system_info(const std::string& mode, const std::string& model);
        void print_ready_interface(const std::string& mode, const std::string& model);
        std::string prompt_user(const std::string& prompt_text);
    }
}