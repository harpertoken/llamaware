#include "services/web_service.h"
#include "utils/config.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

namespace Services {
    std::string WebService::get_api_key() {
        return Utils::Config::get_env_var("SERPAPI_KEY");
    }
    
    bool WebService::is_available() {
        return !get_api_key().empty();
    }
    
    std::string WebService::search(const std::string& query) {
        std::string api_key = get_api_key();
        if (api_key.empty()) {
            return "Web search not available. SERPAPI_KEY is missing.";
        }
        
        std::string search_url = "https://serpapi.com/search.json?q=" + 
                                cpr::util::urlEncode(query) + "&api_key=" + api_key;
        
        cpr::Response response = cpr::Get(cpr::Url{search_url});
        
        if (response.status_code != 200) {
            return "Search error: HTTP " + std::to_string(response.status_code);
        }
        
        try {
            auto json = nlohmann::json::parse(response.text);
            std::string result;
            
            if (json.contains("organic_results")) {
                for (const auto& item : json["organic_results"]) {
                    if (item.contains("title") && item.contains("snippet")) {
                        result += "• " + item["title"].get<std::string>() + 
                                 "\n  " + item["snippet"].get<std::string>() + "\n\n";
                        if (result.size() > 1500) break; // Limit output size
                    }
                }
            }
            
            return result.empty() ? "No search results found." : result;
            
        } catch (const std::exception& e) {
            return "Error parsing search results: " + std::string(e.what());
        }
    }
}