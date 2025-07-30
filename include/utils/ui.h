#pragma once
#include <string>
#include <atomic>
#include <vector>
#include <map>

namespace Utils {
    namespace UI {
        // Enhanced ANSI colors and styles
        extern const std::string CYAN;
        extern const std::string GREEN;
        extern const std::string YELLOW;
        extern const std::string RED;
        extern const std::string BLUE;
        extern const std::string MAGENTA;
        extern const std::string WHITE;
        extern const std::string BLACK;
        extern const std::string RESET;
        extern const std::string BOLD;
        extern const std::string DIM;
        extern const std::string ITALIC;
        extern const std::string UNDERLINE;
        extern const std::string BLINK;
        extern const std::string REVERSE;

        // Background colors
        extern const std::string BG_BLACK;
        extern const std::string BG_RED;
        extern const std::string BG_GREEN;
        extern const std::string BG_YELLOW;
        extern const std::string BG_BLUE;
        extern const std::string BG_MAGENTA;
        extern const std::string BG_CYAN;
        extern const std::string BG_WHITE;

        // Gradient colors
        extern const std::string GRADIENT_1;
        extern const std::string GRADIENT_2;
        extern const std::string GRADIENT_3;
        extern const std::string GRADIENT_4;

        // Unicode symbols
        extern const std::string ARROW_RIGHT;
        extern const std::string ARROW_LEFT;
        extern const std::string BULLET;
        extern const std::string CHECK;
        extern const std::string CROSS;
        extern const std::string WARNING;
        extern const std::string INFO;
        extern const std::string LIGHTNING;
        extern const std::string GEAR;
        extern const std::string ROCKET;

        // Screen control functions
        void clear_screen();
        void move_cursor(int row, int col);
        void hide_cursor();
        void show_cursor();
        std::string get_timestamp();

        // Logo and branding
        void print_animated_logo();
        void print_logo();
        
        // Help and information
        void print_help();
        void print_section_header(const std::string& title);
        void print_command_category(const std::string& category);
        void print_command(const std::string& cmd, const std::string& desc);
        void print_tip(const std::string& tip);
        
        // Progress and loading
        void advanced_spinner(std::atomic<bool>& done, const std::string& message);
        void spinner(std::atomic<bool>& done);
        void progress_bar(int percentage, const std::string& label);
        
        // Status messages
        void print_success(const std::string& message);
        void print_error(const std::string& message);
        void print_warning(const std::string& message);
        void print_info(const std::string& message);
        void print_debug(const std::string& message);
        
        // Advanced formatting
        void print_banner(const std::string& message);
        void print_table_header(const std::vector<std::string>& headers);
        void print_table_row(const std::vector<std::string>& values);
        void print_status_dashboard(const std::map<std::string, std::string>& status_items);
        
        // Special effects
        void typewriter_effect(const std::string& text, int delay_ms);
        void print_gradient_text(const std::string& text);
    }
}