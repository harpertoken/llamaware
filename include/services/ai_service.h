#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace Services {
    class AIService {
    private:
        int mode_;
        std::string api_key_;

        bool is_online_mode() const;
        nlohmann::json create_standard_payload(const std::string& model, const std::string& user_input, const std::string& context);
        nlohmann::json create_payload(const std::string& user_input, const std::string& context);
        std::string get_api_url();
        std::string parse_cerebras_stream(const std::string& response);
        
    public:
        AIService(int mode, const std::string& api_key = "");
        
        std::string chat(const std::string& user_input, const std::string& context);
        bool is_available();
    };
}