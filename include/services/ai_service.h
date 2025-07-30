#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace Services {
    class AIService {
    private:
        int mode_;
        std::string api_key_;
        
        nlohmann::json create_payload(const std::string& user_input, const std::string& context);
        std::string get_api_url();
        
    public:
        AIService(int mode, const std::string& api_key = "");
        
        std::string chat(const std::string& user_input, const std::string& context);
        bool is_available();
    };
}