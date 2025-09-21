#pragma once

#define LLAMAWARE_VERSION_MAJOR 0
#define LLAMAWARE_VERSION_MINOR 1
#define LLAMAWARE_VERSION_PATCH 0
#define LLAMAWARE_VERSION_STRING "0.1"
#define LLAMAWARE_BUILD_DATE __DATE__
#define LLAMAWARE_BUILD_TIME __TIME__

namespace Version {
    const char* get_version();
    const char* get_build_info();
    void print_version_info();
}