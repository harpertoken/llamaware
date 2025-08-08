#include "services/ai_service.h"
#include "utils/config.h"

#include <cpr/cpr.h>
#include <sstream>
#include <nlohmann/json.hpp>

namespace Services {

    AIService::AIService(int mode, const std::string& api_key)
        : mode_(mode), api_key_(api_key) {}

    bool AIService::is_available() {
        // For online providers (Together, Cerebras) API key is required.
        if (mode_ == 1 || mode_ == 3) {
            return !api_key_.empty();
        }
        // Offline modes don't need an API key (they talk to local server)
        return true;
    }

    std::string AIService::get_api_url() {
        switch (mode_) {
            case 1: return "https://api.together.xyz/v1/chat/completions";
            case 3: return "https://api.cerebras.ai/v1/chat/completions";
            case 2:
            case 4:
            default: return "http://localhost:11434/api/chat";
        }
    }

    nlohmann::json AIService::create_payload(const std::string& user_input, const std::string& context) {
        std::string system_prompt =
            "You are an advanced AI agent with the following capabilities:\n"
            "• search:query - Search the web for information\n"
            "• cmd:command - Execute shell commands safely\n"
            "• read:filename - Read file contents\n"
            "• write:filename content - Write content to files\n\n"
            "When users ask you to perform tasks, suggest the appropriate commands.\n"
            "Be helpful, concise, and professional.\n\n"
            "Conversation history:\n" + context;

        switch (mode_) {
            case 1: // Together AI
                return {
                    {"model", "meta-llama/Llama-3.3-70B-Instruct-Turbo-Free"},
                    {"messages", {
                        {{"role", "system"}, {"content", system_prompt}},
                        {{"role", "user"}, {"content", user_input}}
                    }},
                    {"max_tokens", 1000},
                    {"temperature", 0.7}
                };
            case 3: // Cerebras
                return {
                    {"model", "llama-4-maverick-17b-128e-instruct"},
                    {"messages", {
                        {{"role", "system"}, {"content", system_prompt}},
                        {{"role", "user"}, {"content", user_input}}
                    }},
                    {"stream", true},
                    {"max_completion_tokens", 4096},
                    {"temperature", 0.7},
                    {"top_p", 0.9}
                };
            case 2: // Llama 3B (local)
                return {
                    {"model", "llama3.2:3b"},
                    {"stream", false},
                    {"messages", {
                        {{"role", "system"}, {"content", system_prompt}},
                        {{"role", "user"}, {"content", user_input}}
                    }}
                };
            default: // Llama latest (local)
                return {
                    {"model", "llama3.2:latest"},
                    {"stream", false},
                    {"messages", {
                        {{"role", "system"}, {"content", system_prompt}},
                        {{"role", "user"}, {"content", user_input}}
                    }}
                };
        }
    }

    std::string AIService::parse_cerebras_stream(const std::string& response) {
        // Response is server-sent events style; accumulate content deltas.
        std::string result;
        std::istringstream stream(response);
        std::string line;

        while (std::getline(stream, line)) {
            // trim leading spaces
            if (line.rfind("data: ", 0) == 0) {
                std::string json_str = line.substr(6);
                if (json_str == "[DONE]") break;

                try {
                    auto json = nlohmann::json::parse(json_str);
                    if (json.contains("choices") && !json["choices"].empty()) {
                        auto& choice = json["choices"][0];
                        if (choice.contains("delta") && choice["delta"].contains("content")) {
                            // content might be string
                            result += choice["delta"]["content"].get<std::string>();
                        } else if (choice.contains("text")) {
                            result += choice["text"].get<std::string>();
                        }
                    }
                } catch (const std::exception&) {
                    // ignore malformed chunk and continue
                    continue;
                }
            }
        }
        return result;
    }

    std::string safe_get_string(const nlohmann::json& j, const std::initializer_list<std::string>& path, const std::string& fallback = "") {
        const nlohmann::json* cur = &j;
        for (const auto& p : path) {
            if (!cur->is_object() || !cur->contains(p)) return fallback;
            cur = &((*cur)[p]);
        }
        if (cur->is_string()) return cur->get<std::string>();
        return fallback;
    }

    std::string AIService::chat(const std::string& user_input, const std::string& context) {
        cpr::Header headers;
        if (mode_ == 1 || mode_ == 3) {
            headers = {
                {"Authorization", "Bearer " + api_key_},
                {"Content-Type", "application/json"}
            };
        } else {
            headers = { {"Content-Type", "application/json"} };
        }

        nlohmann::json payload = create_payload(user_input, context);

        cpr::Response response;
        try {
            response = cpr::Post(
                cpr::Url{get_api_url()},
                headers,
                cpr::Body{payload.dump()}
            );
        } catch (const std::exception& e) {
            return std::string("HTTP request failed: ") + e.what();
        }

        if (response.status_code == 200) {
            try {
                if (mode_ == 3) {
                    // Cerebras streaming style
                    return parse_cerebras_stream(response.text);
                } else {
                    auto json = nlohmann::json::parse(response.text);

                    if (mode_ == 1) {
                        // Together: choices[0].message.content
                        std::string val = safe_get_string(json, {"choices"}, "");
                        // more robust extraction:
                        if (json.contains("choices") && !json["choices"].empty()) {
                            const auto& ch = json["choices"][0];
                            if (ch.contains("message") && ch["message"].contains("content")) {
                                return ch["message"]["content"].get<std::string>();
                            } else if (ch.contains("text")) {
                                return ch["text"].get<std::string>();
                            }
                        }
                        return "AI responded with unexpected format";
                    } else {
                        // Offline modes (2,4) - assume ollama-like: { "message": {"content": "..." } }
                        if (json.contains("message") && json["message"].contains("content")) {
                            return json["message"]["content"].get<std::string>();
                        }
                        // Fallbacks
                        if (json.contains("choices") && !json["choices"].empty()) {
                            const auto& ch = json["choices"][0];
                            if (ch.contains("text")) return ch["text"].get<std::string>();
                        }
                        return "AI responded with unexpected format";
                    }
                }
            } catch (const std::exception& e) {
                return "Error parsing AI response: " + std::string(e.what());
            }
        } else {
            if ((mode_ == 2 || mode_ == 4) && response.status_code == 0) {
                return "Offline server not found. Please start Ollama with 'ollama serve'";
            }
            return "AI service error: HTTP " + std::to_string(response.status_code) + " - " + response.error.message;
        }
    }

} // namespace Services
