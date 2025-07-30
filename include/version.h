#pragma once

#define LLAMAWARE_VERSION_MAJOR 2
#define LLAMAWARE_VERSION_MINOR 0
#define LLAMAWARE_VERSION_PATCH 0
#define LLAMAWARE_VERSION_STRING "2.0.0"
#define LLAMAWARE_BUILD_DATE __DATE__
#define LLAMAWARE_BUILD_TIME __TIME__

namespace Version {
    const char* get_version();
    const char* get_build_info();
    void print_version_info();
}