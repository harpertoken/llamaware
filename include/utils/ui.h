#pragma once
#include <string>
#include <atomic>
#include "utils/config.h"  // For LLAMAWARE_API

namespace Utils {
    // Color namespace
    namespace Color {
        extern LLAMAWARE_API const std::string RESET;
        extern LLAMAWARE_API const std::string GREEN;
        extern LLAMAWARE_API const std::string YELLOW;
        extern LLAMAWARE_API const std::string RED;
        extern LLAMAWARE_API const std::string CYAN;
        extern LLAMAWARE_API const std::string BOLD;
        extern LLAMAWARE_API const std::string DIM;
    }

    // UI namespace
    namespace UI {
        // Core functions
        LLAMAWARE_API void print_logo();
        LLAMAWARE_API void print_help();
        LLAMAWARE_API void print_enterprise_status();
        LLAMAWARE_API void spinner(const std::string& message, int duration_ms);
        LLAMAWARE_API void spinner(std::atomic<bool>& done);  // For threaded spinner
        
        // Status messages
        LLAMAWARE_API void print_success(const std::string& message);
        LLAMAWARE_API void print_error(const std::string& message);
        LLAMAWARE_API void print_warning(const std::string& message);
        LLAMAWARE_API void print_info(const std::string& message);
        
        // Utility functions
        LLAMAWARE_API void print_divider();
        LLAMAWARE_API void print_quick_help();
        LLAMAWARE_API void print_system_info(const std::string& mode, const std::string& model);
        LLAMAWARE_API void print_ready_interface(const std::string& mode, const std::string& model);
        LLAMAWARE_API std::string prompt_user(const std::string& prompt_text);
    }
}