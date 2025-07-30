#pragma once
#include <string>

namespace Services {
    class WebService {
    private:
        static std::string get_api_key();
        
    public:
        static std::string search(const std::string& query);
        static bool is_available();
    };
}