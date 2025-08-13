#pragma once
#include <string>
#include <map>

namespace Services {
    struct WebResponse {
        int status_code;
        std::string content;
        std::string content_type;
        std::map<std::string, std::string> headers;
        bool success;
        std::string error_message;
    };

    class WebService {
    private:
        static std::string get_api_key();
        static std::string extract_text_content(const std::string& html);
        static std::string sanitize_content(const std::string& content);
        
    public:
        // Existing search functionality
        static std::string search(const std::string& query);
        static bool is_available();
        
        // New web fetch capabilities
        static WebResponse fetch_url(const std::string& url);
        static std::string fetch_text(const std::string& url);
        static std::string fetch_json(const std::string& url);
        static WebResponse fetch_with_headers(const std::string& url, const std::map<std::string, std::string>& headers);
        static bool is_valid_url(const std::string& url);
    };
}