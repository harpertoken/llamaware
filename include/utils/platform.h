#pragma once

#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
    #include <process.h>
    #define popen _popen
    #define pclose _pclose
    #define NULL_DEVICE "NUL"
    #define PATH_SEPARATOR "\\"
    #define SHELL_REDIRECT " 2>NUL"
    #define SHELL_REDIRECT_BOTH " >NUL 2>&1"
#else
    #include <unistd.h>
    #include <sys/wait.h>
    #define NULL_DEVICE "/dev/null"
    #define PATH_SEPARATOR "/"
    #define SHELL_REDIRECT " 2>/dev/null"
    #define SHELL_REDIRECT_BOTH " >/dev/null 2>&1"
#endif

#include <string>
#include <cstdio>

namespace Utils {
    namespace Platform {
        
        // Cross-platform command execution
        inline FILE* open_process(const std::string& command, const char* mode) {
            return popen(command.c_str(), mode);
        }
        
        inline int close_process(FILE* pipe) {
            return pclose(pipe);
        }
        
        // Cross-platform null device
        inline std::string get_null_device() {
            return NULL_DEVICE;
        }
        
        // Cross-platform path separator
        inline std::string get_path_separator() {
            return PATH_SEPARATOR;
        }
        
        // Cross-platform shell redirection
        inline std::string get_shell_redirect() {
            return SHELL_REDIRECT;
        }
        
        inline std::string get_shell_redirect_both() {
            return SHELL_REDIRECT_BOTH;
        }
        
        // Check if running on Windows
        inline bool is_windows() {
#ifdef _WIN32
            return true;
#else
            return false;
#endif
        }
        
    } // namespace Platform
} // namespace Utils
