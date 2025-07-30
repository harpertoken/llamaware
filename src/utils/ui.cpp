#include "utils/ui.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <map>

namespace Utils {
    namespace UI {
        // Enhanced ANSI colors and styles
        const std::string CYAN = "\033[36m";
        const std::string GREEN = "\033[32m";
        const std::string YELLOW = "\033[33m";
        const std::string RED = "\033[31m";
        const std::string BLUE = "\033[34m";
        const std::string MAGENTA = "\033[35m";
        const std::string WHITE = "\033[37m";
        const std::string BLACK = "\033[30m";
        const std::string RESET = "\033[0m";
        const std::string BOLD = "\033[1m";
        const std::string DIM = "\033[2m";
        const std::string ITALIC = "\033[3m";
        const std::string UNDERLINE = "\033[4m";
        const std::string BLINK = "\033[5m";
        const std::string REVERSE = "\033[7m";

        // Background colors
        const std::string BG_BLACK = "\033[40m";
        const std::string BG_RED = "\033[41m";
        const std::string BG_GREEN = "\033[42m";
        const std::string BG_YELLOW = "\033[43m";
        const std::string BG_BLUE = "\033[44m";
        const std::string BG_MAGENTA = "\033[45m";
        const std::string BG_CYAN = "\033[46m";
        const std::string BG_WHITE = "\033[47m";

        // Gradient colors
        const std::string GRADIENT_1 = "\033[38;5;33m";  // Deep blue
        const std::string GRADIENT_2 = "\033[38;5;39m";  // Light blue
        const std::string GRADIENT_3 = "\033[38;5;45m";  // Cyan
        const std::string GRADIENT_4 = "\033[38;5;51m";  // Bright cyan

        // Unicode symbols
        const std::string ARROW_RIGHT = "→";
        const std::string ARROW_LEFT = "←";
        const std::string BULLET = "•";
        const std::string CHECK = "✓";
        const std::string CROSS = "✗";
        const std::string WARNING = "⚠";
        const std::string INFO = "ℹ";
        const std::string LIGHTNING = "⚡";
        const std::string GEAR = "⚙";
        const std::string ROCKET = "🚀";

        void clear_screen() {
            std::cout << "\033[2J\033[H";
        }

        void move_cursor(int row, int col) {
            std::cout << "\033[" << row << ";" << col << "H";
        }

        void hide_cursor() {
            std::cout << "\033[?25l";
        }

        void show_cursor() {
            std::cout << "\033[?25h";
        }

        std::string get_timestamp() {
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&time_t), "%H:%M:%S");
            return ss.str();
        }

        void print_animated_logo() {
            clear_screen();
            hide_cursor();
            
            std::vector<std::string> logo_lines = {
                "╔══════════════════════════════════════════════════╗",
                "║                                                  ║",
                "║    ██╗███╗   ██╗ ██████╗ ███████╗███╗   ██╗     ║",
                "║    ██║████╗  ██║██╔════╝ ██╔════╝████╗  ██║     ║",
                "║    ██║██╔██╗ ██║██║  ███╗█████╗  ██╔██╗ ██║     ║",
                "║    ██║██║╚██╗██║██║   ██║██╔══╝  ██║╚██╗██║     ║",
                "║    ██║██║ ╚████║╚██████╔╝███████╗██║ ╚████║     ║",
                "║    ╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚══════╝╚═╝  ╚═══╝     ║",
                "║                                                  ║",
                "║        " + LIGHTNING + " Ingenuity x Llama Agent v3.0 " + LIGHTNING + "        ║",
                "║              Professional Edition                 ║",
                "║                                                  ║",
                "╚══════════════════════════════════════════════════╝"
            };

            // Animate logo appearance
            for (size_t i = 0; i < logo_lines.size(); ++i) {
                std::cout << GRADIENT_1 + BOLD + logo_lines[i] + RESET << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(150));
            }

            // Add pulsing effect
            for (int pulse = 0; pulse < 3; ++pulse) {
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                move_cursor(1, 1);
                for (const auto& line : logo_lines) {
                    std::cout << GRADIENT_4 + BOLD + line + RESET << std::endl;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                move_cursor(1, 1);
                for (const auto& line : logo_lines) {
                    std::cout << GRADIENT_1 + BOLD + line + RESET << std::endl;
                }
            }

            show_cursor();
            std::cout << std::endl;
        }
        
        void print_logo() {
            std::cout << GRADIENT_1 << BOLD;
            std::cout << "\n╔══════════════════════════════════════════════════╗\n";
            std::cout << "║                                                  ║\n";
            std::cout << "║    ██╗███╗   ██╗ ██████╗ ███████╗███╗   ██╗     ║\n";
            std::cout << "║    ██║████╗  ██║██╔════╝ ██╔════╝████╗  ██║     ║\n";
            std::cout << "║    ██║██╔██╗ ██║██║  ███╗█████╗  ██╔██╗ ██║     ║\n";
            std::cout << "║    ██║██║╚██╗██║██║   ██║██╔══╝  ██║╚██╗██║     ║\n";
            std::cout << "║    ██║██║ ╚████║╚██████╔╝███████╗██║ ╚████║     ║\n";
            std::cout << "║    ╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚══════╝╚═╝  ╚═══╝     ║\n";
            std::cout << "║                                                  ║\n";
            std::cout << "║        " << LIGHTNING << " Ingenuity x Llama Agent v3.0 " << LIGHTNING << "        ║\n";
            std::cout << "║              Professional Edition                 ║\n";
            std::cout << "║                                                  ║\n";
            std::cout << "╚══════════════════════════════════════════════════╝\n" << RESET;
            std::cout << std::endl;
        }
        
        void print_help() {
            print_section_header("Available Commands");
            
            print_command_category("Search & Discovery");
            print_command("search:query", "Search the web with advanced filtering");
            print_command("find:pattern", "Find files/patterns in current directory");
            print_command("explore:path", "Explore directory structure");
            
            print_command_category("System Operations");
            print_command("cmd:command", "Execute shell command with output");
            print_command("sudo:command", "Execute privileged command");
            print_command("ps", "Show running processes");
            print_command("env", "Display environment variables");
            
            print_command_category("File Management");
            print_command("read:filename", "Read and display file contents");
            print_command("write:file content", "Write content to file");
            print_command("append:file content", "Append content to file");
            print_command("backup:filename", "Create backup of file");
            print_command("diff:file1 file2", "Compare two files");
            
            print_command_category("AI & Chat");
            print_command("chat", "Enter interactive chat mode");
            print_command("analyze:file", "AI analysis of file content");
            print_command("summarize:text", "Generate summary");
            print_command("translate:lang text", "Translate text");
            
            print_command_category("Utility Commands");
            print_command("help", "Show this comprehensive help");
            print_command("version", "Display detailed version info");
            print_command("status", "Show system status");
            print_command("config", "View/edit configuration");
            print_command("history", "Show command history");
            print_command("clear", "Clear the screen");
            print_command("exit", "Gracefully quit the agent");
            
            std::cout << std::endl;
            print_tip("Tip: Just type naturally to chat with AI, or use 'cmd:' prefix for direct commands");
        }
        
        void print_section_header(const std::string& title) {
            std::cout << std::endl;
            std::cout << GRADIENT_2 << BOLD << "+=" << std::string(title.length() + 2, '=') << "=+" << RESET << std::endl;
            std::cout << GRADIENT_2 << BOLD << "| " << title << " |" << RESET << std::endl;
            std::cout << GRADIENT_2 << BOLD << "+=" << std::string(title.length() + 2, '=') << "=+" << RESET << std::endl;
        }

        void print_command_category(const std::string& category) {
            std::cout << std::endl << CYAN << BOLD << category << RESET << std::endl;
            std::cout << CYAN << std::string(category.length(), '-') << RESET << std::endl;
        }

        void print_command(const std::string& cmd, const std::string& desc) {
            std::cout << "  " << GREEN << BOLD << cmd << RESET;
            // Calculate padding for alignment
            int padding = 25 - static_cast<int>(cmd.length());
            if (padding < 1) padding = 1;
            std::cout << std::string(padding, ' ') << DIM << "─ " << desc << RESET << std::endl;
        }

        void print_tip(const std::string& tip) {
            std::cout << std::endl;
            std::cout << YELLOW << BG_BLACK << " " << tip << " " << RESET << std::endl;
        }

        void advanced_spinner(std::atomic<bool>& done, const std::string& message = "Processing") {
            const std::vector<std::string> frames = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
            int frame = 0;
            auto start_time = std::chrono::steady_clock::now();
            
            hide_cursor();
            std::cout << CYAN << message << " " << RESET;
            std::cout.flush();
            
            while (!done) {
                auto current_time = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);
                
                std::cout << "\r" << CYAN << message << " " << YELLOW << frames[frame % frames.size()] 
                         << " " << DIM << "[" << duration.count() << "s]" << RESET;
                std::cout.flush();
                
                frame++;
                std::this_thread::sleep_for(std::chrono::milliseconds(80));
            }
            
            std::cout << "\r" << std::string(message.length() + 20, ' ') << "\r";
            show_cursor();
        }

        void spinner(std::atomic<bool>& done) {
            advanced_spinner(done, "Working");
        }

        void progress_bar(int percentage, const std::string& label = "") {
            const int bar_width = 40;
            const int filled = (percentage * bar_width) / 100;
            
            std::cout << "\r" << CYAN << label;
            if (!label.empty()) std::cout << " ";
            std::cout << "[";
            
            for (int i = 0; i < bar_width; ++i) {
                if (i < filled) {
                    std::cout << GREEN << "█" << RESET;
                } else {
                    std::cout << DIM << "░" << RESET;
                }
            }
            
            std::cout << "] " << BOLD << percentage << "%" << RESET;
            std::cout.flush();
            
            if (percentage >= 100) {
                std::cout << " " << GREEN << CHECK << " Complete!" << RESET << std::endl;
            }
        }

        void print_success(const std::string& message) {
            std::cout << GREEN << BOLD << CHECK << " " << message << RESET 
                     << DIM << " [" << get_timestamp() << "]" << RESET << std::endl;
        }

        void print_error(const std::string& message) {
            std::cout << RED << BOLD << CROSS << " " << message << RESET 
                     << DIM << " [" << get_timestamp() << "]" << RESET << std::endl;
        }

        void print_warning(const std::string& message) {
            std::cout << YELLOW << BOLD << WARNING << " " << message << RESET 
                     << DIM << " [" << get_timestamp() << "]" << RESET << std::endl;
        }

        void print_info(const std::string& message) {
            std::cout << BLUE << BOLD << INFO << " " << message << RESET 
                     << DIM << " [" << get_timestamp() << "]" << RESET << std::endl;
        }

        void print_debug(const std::string& message) {
            std::cout << MAGENTA << BOLD << GEAR << " [DEBUG] " << message << RESET 
                     << DIM << " [" << get_timestamp() << "]" << RESET << std::endl;
        }

        void print_banner(const std::string& message) {
            int width = static_cast<int>(message.length()) + 4;
            std::string border(width, '=');
            
            std::cout << std::endl;
            std::cout << GRADIENT_1 << BOLD << "+" << border << "+" << RESET << std::endl;
            std::cout << GRADIENT_1 << BOLD << "| " << message << " |" << RESET << std::endl;
            std::cout << GRADIENT_1 << BOLD << "+" << border << "+" << RESET << std::endl;
            std::cout << std::endl;
        }

        void print_table_header(const std::vector<std::string>& headers) {
            std::cout << CYAN << BOLD;
            for (size_t i = 0; i < headers.size(); ++i) {
                std::cout << std::setw(15) << std::left << headers[i];
                if (i < headers.size() - 1) std::cout << " │ ";
            }
            std::cout << RESET << std::endl;
            
            // Print separator
            std::cout << CYAN;
            for (size_t i = 0; i < headers.size(); ++i) {
                std::cout << std::string(15, '-');
                if (i < headers.size() - 1) std::cout << "-+-";
            }
            std::cout << RESET << std::endl;
        }

        void print_table_row(const std::vector<std::string>& values) {
            for (size_t i = 0; i < values.size(); ++i) {
                std::cout << std::setw(15) << std::left << values[i];
                if (i < values.size() - 1) std::cout << " │ ";
            }
            std::cout << std::endl;
        }

        void print_status_dashboard(const std::map<std::string, std::string>& status_items) {
            print_section_header("System Status");
            
            for (const auto& item : status_items) {
                std::cout << "  " << BLUE << BOLD << item.first << ":" << RESET;
                int padding = 20 - static_cast<int>(item.first.length());
                if (padding < 1) padding = 1;
                std::cout << std::string(padding, ' ') << GREEN << item.second << RESET << std::endl;
            }
            std::cout << std::endl;
        }

        void typewriter_effect(const std::string& text, int delay_ms = 50) {
            for (char c : text) {
                std::cout << c << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
            }
            std::cout << std::endl;
        }

        void print_gradient_text(const std::string& text) {
            std::vector<std::string> colors = {GRADIENT_1, GRADIENT_2, GRADIENT_3, GRADIENT_4};
            for (size_t i = 0; i < text.length(); ++i) {
                std::cout << colors[i % colors.size()] << text[i] << RESET;
            }
            std::cout << std::endl;
        }
    }
}