#pragma once
#include <string>

// Define DLL export/import macros for Windows
#if defined(_WIN32) || defined(_WIN64)
    #ifdef LLAMAWARE_LIBRARY
        #define LLAMAWARE_API __declspec(dllexport)
    #else
        #define LLAMAWARE_API __declspec(dllimport)
    #endif
#else
    #define LLAMAWARE_API __attribute__((visibility("default")))
#endif

namespace Utils {
    namespace Config {
        LLAMAWARE_API void load_environment(const std::string& filename = ".env");
        LLAMAWARE_API std::string get_env_var(const std::string& key, const std::string& default_value = "");
        LLAMAWARE_API bool has_env_var(const std::string& key);
    }
}