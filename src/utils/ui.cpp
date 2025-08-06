#include "utils/ui.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace Utils {
    namespace UI {
        // Minimal color palette
        const std::string CYAN = "\033[36m";
        const std::string GREEN = "\033[32m";
        const std::string YELLOW = "\033[33m";
        const std::string RED = "\033[31m";
        const std::string RESET = "\033[0m";
        const std::string DIM = "\033[2m";

        void print_logo() {
    std::cout << "\n";
    std::cout << GREEN << "   ███      ███      ███   " << RESET << "\n";
    std::cout << GREEN << "   ███      ███      ███   " << RESET << "\n";
    std::cout << GREEN << "   ███      ███      ███   " << RESET << "\n";
    std::cout << GREEN << "          ███  ███          " << RESET << "\n";
    std::cout << GREEN << "          ███  ███          " << RESET << "\n";
    std::cout << GREEN << "          ███  ███          " << RESET << "\n";
    std::cout << GREEN << "          ███  ███          " << RESET << "\n";
    std::cout << GREEN << "          ███  ███          " << RESET << "\n";
    std::cout << GREEN << "          ███  ███          " << RESET << "\n";
    std::cout << "\nLlamaware" << DIM << " - Open Source AI Agent" << RESET << "\n";
    std::cout << DIM << "Type 'help' for commands" << RESET << "\n\n";
}

        
        void print_help() {
            std::cout << "\nLlamaware - Open Source AI Agent\n";
            std::cout << DIM << "MIT License - github.com/bniladridas/llamaware" << RESET << "\n\n";
            std::cout << "Commands:\n";
            std::cout << "  search:query     web search\n";
            std::cout << "  cmd:command      run command\n";
            std::cout << "  read:file        read file\n";
            std::cout << "  write:file text  write file\n";
            std::cout << "  help             show help\n";
            std::cout << "  version          show version\n";
            std::cout << "  exit             quit\n\n";
            std::cout << DIM << "Chat naturally or use commands above" << RESET << "\n\n";
        }
        
        void spinner(std::atomic<bool>& done) {
            const char frames[] = {'|', '/', '-', '\\'};
            int frame = 0;
            
            while (!done) {
                std::cout << "\r" << frames[frame % 4] << std::flush;
                frame++;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            std::cout << "\r \r" << std::flush;
        }

        void print_success(const std::string& message) {
            std::cout << GREEN << message << RESET << std::endl;
        }

        void print_error(const std::string& message) {
            std::cout << RED << message << RESET << std::endl;
        }

        void print_warning(const std::string& message) {
            std::cout << YELLOW << message << RESET << std::endl;
        }

        void print_info(const std::string& message) {
            std::cout << message << std::endl;
        }
    }
}