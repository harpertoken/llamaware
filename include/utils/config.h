#pragma once
#include <string>

namespace Utils {
    namespace Config {
        void load_environment(const std::string& filename = ".env");
        std::string get_env_var(const std::string& key, const std::string& default_value = "");
        bool has_env_var(const std::string& key);
    }
}