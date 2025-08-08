#include "version.h"
#include <string>
#include <iostream>

namespace Version {
    const char* get_version() {
        return LLAMAWARE_VERSION_STRING;
    }

    const char* get_build_info() {
        static const std::string build_info = std::string(LLAMAWARE_BUILD_DATE) + " " + LLAMAWARE_BUILD_TIME;
        return build_info.c_str();
    }

    void print_version_info() {
        std::cout << "Llamaware v" << get_version() << "\n";
        std::cout << "Built: " << get_build_info() << "\n";
    }
}
