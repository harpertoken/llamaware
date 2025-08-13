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

    bool WebService::is_valid_url(const std::string& url) {
        // Basic URL validation
        if (url.empty()) return false;
        
        // Check for common protocols
        if (url.find("http://") == 0 || url.find("https://") == 0) {
            // Must have at least a domain after protocol
            size_t domain_start = url.find("://") + 3;
            if (domain_start < url.length() && url.find('.', domain_start) != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    std::string WebService::extract_text_content(const std::string& html) {
        std::string text = html;
        
        // Remove script and style tags and their content
        size_t pos = 0;
        while ((pos = text.find("<script", pos)) != std::string::npos) {
            size_t end = text.find("</script>", pos);
            if (end != std::string::npos) {
                text.erase(pos, end - pos + 9);
            } else {
                break;
            }
        }
        
        pos = 0;
        while ((pos = text.find("<style", pos)) != std::string::npos) {
            size_t end = text.find("</style>", pos);
            if (end != std::string::npos) {
                text.erase(pos, end - pos + 8);
            } else {
                break;
            }
        }
        
        // Remove HTML tags
        pos = 0;
        while ((pos = text.find('<', pos)) != std::string::npos) {
            size_t end = text.find('>', pos);
            if (end != std::string::npos) {
                text.erase(pos, end - pos + 1);
            } else {
                break;
            }
        }
        
        return sanitize_content(text);
    }

    std::string WebService::sanitize_content(const std::string& content) {
        std::string sanitized = content;
        
        // Replace HTML entities
        size_t pos = 0;
        while ((pos = sanitized.find("&amp;", pos)) != std::string::npos) {
            sanitized.replace(pos, 5, "&");
            pos += 1;
        }
        while ((pos = sanitized.find("&lt;", pos)) != std::string::npos) {
            sanitized.replace(pos, 4, "<");
            pos += 1;
        }
        while ((pos = sanitized.find("&gt;", pos)) != std::string::npos) {
            sanitized.replace(pos, 4, ">");
            pos += 1;
        }
        while ((pos = sanitized.find("&quot;", pos)) != std::string::npos) {
            sanitized.replace(pos, 6, "\"");
            pos += 1;
        }
        while ((pos = sanitized.find("&#39;", pos)) != std::string::npos) {
            sanitized.replace(pos, 5, "'");
            pos += 1;
        }
        
        // Clean up whitespace
        pos = 0;
        while ((pos = sanitized.find("\n\n\n", pos)) != std::string::npos) {
            sanitized.replace(pos, 3, "\n\n");
        }
        
        // Trim leading/trailing whitespace
        size_t start = sanitized.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) return "";
        
        size_t end = sanitized.find_last_not_of(" \t\n\r");
        return sanitized.substr(start, end - start + 1);
    }

    WebResponse WebService::fetch_url(const std::string& url) {
        WebResponse response;
        response.success = false;
        
        if (!is_valid_url(url)) {
            response.error_message = "Invalid URL format";
            response.status_code = 0;
            return response;
        }
        
        try {
            auto cpr_response = cpr::Get(
                cpr::Url{url},
                cpr::Timeout{10000}, // 10s timeout
                cpr::UserAgent{"Llamaware-Agent/1.0"}
            );
            
            if (cpr_response.error) {
                response.error_message = "Network error: " + cpr_response.error.message;
                return response;
            }
            
            response.status_code = cpr_response.status_code;
            response.content = cpr_response.text;
            response.success = (cpr_response.status_code >= 200 && cpr_response.status_code < 300);
            
            // Extract headers
            for (const auto& header : cpr_response.header) {
                response.headers[header.first] = header.second;
            }
            
            // Set content type
            auto content_type_it = response.headers.find("content-type");
            if (content_type_it != response.headers.end()) {
                response.content_type = content_type_it->second;
            }
            
            if (!response.success) {
                response.error_message = "HTTP " + std::to_string(response.status_code);
            }
            
        } catch (const std::exception& e) {
            response.error_message = "Exception: " + std::string(e.what());
        }
        
        return response;
    }

    WebResponse WebService::fetch_with_headers(const std::string& url, const std::map<std::string, std::string>& headers) {
        WebResponse response;
        response.success = false;
        
        if (!is_valid_url(url)) {
            response.error_message = "Invalid URL format";
            response.status_code = 0;
            return response;
        }
        
        try {
            cpr::Header cpr_headers;
            for (const auto& header : headers) {
                cpr_headers[header.first] = header.second;
            }
            
            auto cpr_response = cpr::Get(
                cpr::Url{url},
                cpr_headers,
                cpr::Timeout{10000}, // 10s timeout
                cpr::UserAgent{"Llamaware-Agent/1.0"}
            );
            
            if (cpr_response.error) {
                response.error_message = "Network error: " + cpr_response.error.message;
                return response;
            }
            
            response.status_code = cpr_response.status_code;
            response.content = cpr_response.text;
            response.success = (cpr_response.status_code >= 200 && cpr_response.status_code < 300);
            
            // Extract headers
            for (const auto& header : cpr_response.header) {
                response.headers[header.first] = header.second;
            }
            
            // Set content type
            auto content_type_it = response.headers.find("content-type");
            if (content_type_it != response.headers.end()) {
                response.content_type = content_type_it->second;
            }
            
            if (!response.success) {
                response.error_message = "HTTP " + std::to_string(response.status_code);
            }
            
        } catch (const std::exception& e) {
            response.error_message = "Exception: " + std::string(e.what());
        }
        
        return response;
    }

    std::string WebService::fetch_text(const std::string& url) {
        WebResponse response = fetch_url(url);
        
        if (!response.success) {
            return "Error fetching URL: " + response.error_message;
        }
        
        // Check if content is HTML and extract text
        if (response.content_type.find("text/html") != std::string::npos) {
            std::string extracted = extract_text_content(response.content);
            if (extracted.length() > 8000) {
                extracted = extracted.substr(0, 8000) + "\n\n[Content truncated - showing first 8000 characters]";
            }
            return extracted;
        }
        
        // For plain text or other content types
        if (response.content.length() > 8000) {
            return response.content.substr(0, 8000) + "\n\n[Content truncated - showing first 8000 characters]";
        }
        
        return response.content;
    }

    std::string WebService::fetch_json(const std::string& url) {
        WebResponse response = fetch_url(url);
        
        if (!response.success) {
            return "Error fetching URL: " + response.error_message;
        }
        
        try {
            // Validate JSON
            auto json = nlohmann::json::parse(response.content);
            
            // Pretty print JSON with truncation if needed
            std::string formatted = json.dump(2);
            if (formatted.length() > 8000) {
                formatted = formatted.substr(0, 8000) + "\n\n[JSON truncated - showing first 8000 characters]";
            }
            return formatted;
            
        } catch (const std::exception& e) {
            return "Error parsing JSON: " + std::string(e.what()) + "\n\nRaw content:\n" + 
                   (response.content.length() > 1000 ? response.content.substr(0, 1000) + "..." : response.content);

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
