#include "services/ai_service.h"
#include "utils/config.h"
#include <cpr/cpr.h>

namespace Services {
    AIService::AIService(int mode, const std::string& api_key) 
        : mode_(mode), api_key_(api_key) {}
    
    bool AIService::is_available() {
        if (mode_ == 1) {
            return !api_key_.empty();
        }
        // For offline mode, we assume Ollama is available
        return true;
    }
    
    std::string AIService::get_api_url() {
        if (mode_ == 1) {
            return "https://api.together.xyz/v1/chat/completions";
        } else {
            return "http://localhost:11434/api/chat";
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
        
        if (mode_ == 1) {
            return {
                {"model", "meta-llama/Llama-3.3-70B-Instruct-Turbo-Free"},
                {"messages", {
                    {{"role", "system"}, {"content", system_prompt}},
                    {{"role", "user"}, {"content", user_input}}
                }},
                {"max_tokens", 1000},
                {"temperature", 0.7}
            };
        } else {
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
    
    std::string AIService::chat(const std::string& user_input, const std::string& context) {
        cpr::Header headers;
        if (mode_ == 1) {
            headers = {
                {"Authorization", "Bearer " + api_key_},
                {"Content-Type", "application/json"}
            };
        } else {
            headers = {{"Content-Type", "application/json"}};
        }
        
        nlohmann::json payload = create_payload(user_input, context);
        cpr::Response response = cpr::Post(
            cpr::Url{get_api_url()}, 
            headers, 
            cpr::Body{payload.dump()}
        );
        
        if (response.status_code == 200) {
            try {
                auto json = nlohmann::json::parse(response.text);
                
                if (mode_ == 1) {
                    return json["choices"][0]["message"]["content"];
                } else {
                    return json["message"]["content"];
                }
            } catch (const std::exception& e) {
                return "Error parsing AI response: " + std::string(e.what());
            }
        } else {
            if (mode_ == 2 && response.status_code == 0) {
                return "Offline server not found. Please start Ollama with 'ollama serve'";
            }
            return "AI service error: HTTP " + std::to_string(response.status_code);
        }
    }
}