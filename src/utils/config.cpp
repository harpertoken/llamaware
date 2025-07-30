#include "utils/config.h"
#include <fstream>
#include <sstream>
#include <cstdlib>

namespace Utils {
    namespace Config {
        void load_environment(const std::string& filename) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                return; // .env file is optional
            }
            
            std::string line;
            while (std::getline(file, line)) {
                // Skip empty lines and comments
                if (line.empty() || line[0] == '#') continue;
                
                std::istringstream is_line(line);
                std::string key, value;
                
                if (std::getline(is_line, key, '=') && std::getline(is_line, value)) {
                    // Trim whitespace
                    key.erase(0, key.find_first_not_of(" \t\n\r"));
                    key.erase(key.find_last_not_of(" \t\n\r") + 1);
                    value.erase(0, value.find_first_not_of(" \t\n\r"));
                    value.erase(value.find_last_not_of(" \t\n\r") + 1);
                    
                    // Remove quotes if present
                    if (value.length() >= 2 && value.front() == '\"' && value.back() == '\"') {
                        value = value.substr(1, value.length() - 2);
                    }
                    
                    setenv(key.c_str(), value.c_str(), 1);
                }
            }
        }
        
        std::string get_env_var(const std::string& key, const std::string& default_value) {
            const char* value = std::getenv(key.c_str());
            return value ? std::string(value) : default_value;
        }
        
        bool has_env_var(const std::string& key) {
            return std::getenv(key.c_str()) != nullptr;
        }
    }
}