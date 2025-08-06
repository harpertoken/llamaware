#include "services/ai_service.h"
#include "utils/config.h"
#include <cpr/cpr.h>
#include <sstream>

namespace Services {
    AIService::AIService(int mode, const std::string& api_key) 
        : mode_(mode), api_key_(api_key) {}
    
    bool AIService::is_available() {
        return (mode_ == 1 || mode_ == 3) ? !api_key_.empty() : true;
    }
    
    std::string AIService::get_api_url() {
        switch (mode_) {
            case 1: return "https://api.together.xyz/v1/chat/completions";
            case 3: return "https://api.cerebras.ai/v1/chat/completions";
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
            case 2: // Llama 3B
                return {
                    {"model", "llama3.2:3b"},
                    {"stream", false},
                    {"messages", {
                        {{"role", "system"}, {"content", system_prompt}},
                        {{"role", "user"}, {"content", user_input}}
                    }}
                };
            default: // Llama Latest
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
        std::string result;
        std::istringstream stream(response);
        std::string line;
        
        while (std::getline(stream, line)) {
            if (line.find("data: ") == 0) {
                std::string json_str = line.substr(6);
                if (json_str == "[DONE]") break;
                
                try {
                    auto json = nlohmann::json::parse(json_str);
                    if (json.contains("choices") && !json["choices"].empty()) {
                        auto& choice = json["choices"][0];
                        if (choice.contains("delta") && choice["delta"].contains("content")) {
                            result += choice["delta"]["content"].get<std::string>();
                        }
                    }
                } catch (const std::exception& e) {
                    // Skip malformed JSON chunks
                    continue;
                }
            }
        }
        return result;
    }

    std::string AIService::chat(const std::string& user_input, const std::string& context) {
        cpr::Header headers;
        if (mode_ == 1 || mode_ == 3) {
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
                if (mode_ == 3) {
                    // Handle Cerebras streaming response
                    return parse_cerebras_stream(response.text);
                } else {
                    auto json = nlohmann::json::parse(response.text);
                    
                    if (mode_ == 1) {
                        return json["choices"][0]["message"]["content"];
                    } else {
                        // Offline modes (2, 4) use Ollama format
                        return json["message"]["content"];
                    }
                }
            } catch (const std::exception& e) {
                return "Error parsing AI response: " + std::string(e.what());
            }
        } else {
            if ((mode_ == 2 || mode_ == 4) && response.status_code == 0) {
                return "Offline server not found. Please start Ollama with 'ollama serve'";
            }
            return "AI service error: HTTP " + std::to_string(response.status_code);
        }
    }
}