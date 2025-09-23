#pragma once

// Define DLL export/import macros for Windows
#if defined(_WIN32) || defined(_WIN64)
    #ifdef LLAMAWARE_LIBRARY
        #define LLAMAWARE_API __declspec(dllexport)
        // Disable warning about STL types in the interface
        #pragma warning(disable: 4251)
    #else
        #define LLAMAWARE_API __declspec(dllimport)
    #endif
    
    // Enable secure CRT functions
    #ifndef _CRT_SECURE_NO_WARNINGS
        #define _CRT_SECURE_NO_WARNINGS
    #endif
    
    // Disable min/max macros from windows.h
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
#else
    #define LLAMAWARE_API __attribute__((visibility("default")))
    // On non-Windows, we can use visibility attributes for better control
    #ifdef __GNUC__
        #define LLAMAWARE_LOCAL __attribute__((visibility("hidden")))
    #else
        #define LLAMAWARE_LOCAL
    #endif
#endif

// Disable exporting of STL templates
#ifdef _MSC_VER
    #define LLAMAWARE_NO_EXPORT_TEMPLATE template class LLAMAWARE_API
#else
    #define LLAMAWARE_NO_EXPORT_TEMPLATE extern template class LLAMAWARE_API
#endif

#include <string>

namespace Utils {
    namespace Config {
        LLAMAWARE_API void load_environment(const std::string& filename = ".env");
        LLAMAWARE_API std::string get_env_var(const std::string& key, const std::string& default_value = "");
        LLAMAWARE_API bool has_env_var(const std::string& key);
    }
}