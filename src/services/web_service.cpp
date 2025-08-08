#include "services/web_service.h"
#include "utils/config.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <sstream>

namespace Services {
    std::string WebService::get_api_key() {
        return Utils::Config::get_env_var("SERPAPI_KEY");
    }

    bool WebService::is_available() {
        return !get_api_key().empty();
    }

    std::string WebService::search(const std::string& query) {
        try {
            std::string api_key = get_api_key();
            if (api_key.empty()) {
                return "Error: SERPAPI_KEY is missing.";
            }

            std::string search_url =
                "https://serpapi.com/search.json?q=" +
                std::string(cpr::util::urlEncode(query)) + "&api_key=" + api_key;

            auto response = cpr::Get(
                cpr::Url{search_url},
                cpr::Timeout{5000} // 5s timeout
            );

            if (response.error) {
                return "Network error: " + response.error.message;
            }

            if (response.status_code == 429) {
                return "Error: Rate limit exceeded (HTTP 429)";
            }

            if (response.status_code != 200) {
                return "Search error: HTTP " + std::to_string(response.status_code);
            }

            auto json = nlohmann::json::parse(response.text, nullptr, true, true);

            if (!json.contains("organic_results") || !json["organic_results"].is_array()) {
                return "Error: Unexpected API response format.";
            }

            std::ostringstream result;
            size_t printed = 0;
            for (const auto& item : json["organic_results"]) {
                if (item.contains("title") && item["title"].is_string()) {
                    result << "• " << item["title"].get<std::string>() << "\n";
                }
                if (item.contains("snippet") && item["snippet"].is_string()) {
                    result << "  " << item["snippet"].get<std::string>() << "\n\n";
                }
                if (++printed >= 10) break; // max 10 results
            }

            auto output = result.str();
            return output.empty() ? "No search results found." : output;
        } catch (const std::exception& e) {
            return std::string("Error performing search: ") + e.what();
        }
    }
}
