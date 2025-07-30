#include "version.h"
#include "utils/ui.h"
#include <iostream>
#include <sstream>

namespace Version {
    const char* get_version() {
        return LLAMAWARE_VERSION_STRING;
    }
    
    const char* get_build_info() {
        static std::string build_info = std::string(LLAMAWARE_BUILD_DATE) + " " + LLAMAWARE_BUILD_TIME;
        return build_info.c_str();
    }
    
    void print_version_info() {
        std::cout << Utils::UI::CYAN << Utils::UI::BOLD;
        std::cout << "Llamaware Agent v" << get_version() << Utils::UI::RESET << std::endl;
        std::cout << Utils::UI::BLUE << "Built: " << get_build_info() << Utils::UI::RESET << std::endl;
        std::cout << Utils::UI::GREEN << "Professional Edition" << Utils::UI::RESET << std::endl;
        std::cout << std::endl;
    }
}