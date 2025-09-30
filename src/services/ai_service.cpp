#include "services/ai_service.h"
#include "utils/config.h"
#include "services/web_service.h"
#include "core/agent.h"
#include <sstream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>

// Use the HeaderMap from web_service.h

// Reuse the CaseInsensitiveCompare and HeaderMap from web_service.h

namespace Services {

    AIService::AIService(Core::Agent::Mode mode, const std::string& api_key)
        : mode_(mode), api_key_(api_key) {}

    bool AIService::is_online_mode() const {
        return mode_ == Core::Agent::MODE_TOGETHER || mode_ == Core::Agent::MODE_CEREBRAS ||
               mode_ == Core::Agent::MODE_FIREWORKS || mode_ == Core::Agent::MODE_GROQ ||
               mode_ == Core::Agent::MODE_DEEPSEEK || mode_ == Core::Agent::MODE_OPENAI;
    }

    bool AIService::is_available() {
        // For online providers API key is required.
        if (is_online_mode()) {
            return !api_key_.empty();
        }
        // Offline modes don't need an API key (they talk to local server)
        return true;
    }

    std::string AIService::get_api_url() {
        if (std::getenv("TEST_MODE")) {
            // Use mock URLs for testing
            switch (mode_) {
                case Core::Agent::MODE_TOGETHER: return "http://mock-together/v1/chat/completions";
                case Core::Agent::MODE_CEREBRAS: return "http://mock-cerebras/v1/chat/completions";
                case Core::Agent::MODE_FIREWORKS: return "http://mock-fireworks/inference/v1/chat/completions";
                case Core::Agent::MODE_GROQ: return "http://mock-groq/openai/v1/chat/completions";
                case Core::Agent::MODE_DEEPSEEK: return "http://mock-deepseek/v1/chat/completions";
                case Core::Agent::MODE_OPENAI: return "http://mock-openai/v1/chat/completions";
                case Core::Agent::MODE_LLAMA_3B:
                case Core::Agent::MODE_LLAMA_LATEST:
                case Core::Agent::MODE_LLAMA_31:
                default: return "http://mock-ollama:11434/api/chat";
            }
        } else {
            switch (mode_) {
                case Core::Agent::MODE_TOGETHER: return "https://api.together.xyz/v1/chat/completions";
                case Core::Agent::MODE_CEREBRAS: return "https://api.cerebras.ai/v1/chat/completions";
                case Core::Agent::MODE_FIREWORKS: return "https://api.fireworks.ai/inference/v1/chat/completions";
                case Core::Agent::MODE_GROQ: return "https://api.groq.com/openai/v1/chat/completions";
                case Core::Agent::MODE_DEEPSEEK: return "https://api.deepseek.com/v1/chat/completions";
                case Core::Agent::MODE_OPENAI: return "https://api.openai.com/v1/chat/completions";
                case Core::Agent::MODE_LLAMA_3B:
                case Core::Agent::MODE_LLAMA_LATEST:
                case Core::Agent::MODE_LLAMA_31:
                default: return "http://localhost:11434/api/chat";
            }
        }
    }

    nlohmann::json AIService::create_standard_payload(const std::string& model, const std::string& user_input, const std::string& context) {
        std::string system_prompt = "You are a helpful AI assistant. " + context;
        return {
            {"model", model},
            {"messages", {
                {{"role", "system"}, {"content", system_prompt}},
                {{"role", "user"}, {"content", user_input}}
            }},
            {"max_tokens", 1000},
            {"temperature", 0.7}
        };
    }

    nlohmann::json AIService::create_payload(const std::string& user_input, const std::string& context) {
        std::string system_prompt =
            "You are an advanced AI agent with comprehensive codebase analysis and development capabilities.\n\n"
            "BASIC COMMANDS:\n"
            "• search:query - Search the web for information\n"
            "• cmd:command - Execute shell commands safely\n"
            "• read:filename - Read file contents\n"
            "• read:filename:start:count - Read specific line ranges from files\n"
            "• write:filename content - Write content to files\n\n"
            "ADVANCED FILE OPERATIONS:\n"
            "• replace:filename:old_text:new_text[:expected_count] - Replace text in files with precision\n"
            "• grep:pattern[:directory[:file_filter]] - Search for patterns in files and directories\n\n"
            "CODEBASE ANALYSIS:\n"
            "• analyze:path - Analyze codebase structure, file types, and project configuration\n"
            "• components:path - Find main components and their relationships\n"
            "• todos:path - Find all TODO/FIXME/HACK comments in codebase\n"
            "• tree:path - Display directory tree structure\n\n"
            "GIT INTEGRATION:\n"
            "• git:log - Show recent git commit history\n"
            "• git:status - Show git working directory status\n"
            "• git:analyze - Comprehensive git repository analysis\n\n"
            "MEMORY MANAGEMENT:\n"
            "• remember:fact - Save important facts to persistent global memory\n"
            "• memory - View stored global memories\n"
            "• clear - Clear current session memory\n"
            "• forget - Clear all global memories\n\n"
            "CAPABILITIES:\n"
            "- Understand codebase structure and relationships\n"
            "- Analyze git history and track changes\n"
            "- Find and prioritize TODO comments\n"
            "- Advanced text search and replacement with context validation\n"
            "- Structured memory system for facts and preferences\n"
            "- File operations with safety checks and path validation\n"
            "- Enhanced error handling and user feedback\n\n"
            "When users ask about codebase analysis, use analyze: or components: commands.\n"
            "For git-related questions, use git: commands.\n"
            "For finding TODOs or technical debt, use todos: command.\n"
            "For complex file editing, use replace: instead of write: when modifying existing content.\n"
            "Use grep: to search for code patterns or text across multiple files.\n"
            "Remember important user preferences and facts using remember:.\n"
            "Be helpful, precise, and professional.\n\n"
            "Conversation history:\n" + context;

        switch (mode_) {
            case Core::Agent::MODE_TOGETHER: // Together AI
                return create_standard_payload("meta-llama/Llama-3.3-70B-Instruct-Turbo-Free", user_input, system_prompt);
            case Core::Agent::MODE_CEREBRAS: // Cerebras
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
            case Core::Agent::MODE_FIREWORKS: // Fireworks
                return create_standard_payload("accounts/fireworks/models/llama-v3-70b-instruct", user_input, system_prompt);
            case Core::Agent::MODE_GROQ: // Groq
                return create_standard_payload("llama-3.1-70b-versatile", user_input, system_prompt);
            case Core::Agent::MODE_DEEPSEEK: // DeepSeek
                return create_standard_payload("deepseek-chat", user_input, system_prompt);
            case Core::Agent::MODE_OPENAI: // OpenAI
                return create_standard_payload("gpt-4", user_input, system_prompt);
            case Core::Agent::MODE_LLAMA_3B: // Llama 3B (local)
                return {
                    {"model", "llama3.2:3b"},
                    {"stream", false},
                    {"messages", {
                        {{"role", "system"}, {"content", system_prompt}},
                        {{"role", "user"}, {"content", user_input}}
                    }}
                };

            case Core::Agent::MODE_LLAMA_LATEST: // Llama latest (local)
                return {
                    {"model", "llama3.2:latest"},
                    {"stream", false},
                    {"messages", {
                        {{"role", "system"}, {"content", system_prompt}},
                        {{"role", "user"}, {"content", user_input}}
                    }}
                };

            case Core::Agent::MODE_LLAMA_31: // Llama 3.1 (local)
                return {
                    {"model", "llama3.1:latest"},
                    {"stream", false},
                    {"messages", {
                        {{"role", "system"}, {"content", system_prompt}},
                        {{"role", "user"}, {"content", user_input}}
                    }}
                };

            default: // Fallback to Llama 3B
                return {
                    {"model", "llama3.2:3b"},
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
        if (!is_available()) {
            return "Error: AI service is not available. Please check your API key and internet connection.";
        }

        auto payload = create_payload(user_input, context);
        auto url = get_api_url();
        
        try {
            // Set up headers
            HeaderMap headers = {
                {"Content-Type", "application/json"},
                {"Accept", "text/event-stream"}
            };
            
            // Set API key in appropriate header based on service
            switch (mode_) {
                case Core::Agent::MODE_TOGETHER: // Together AI
                case Core::Agent::MODE_FIREWORKS: // Fireworks
                case Core::Agent::MODE_GROQ: // Groq
                case Core::Agent::MODE_DEEPSEEK: // DeepSeek
                case Core::Agent::MODE_OPENAI: // OpenAI
                    headers["Authorization"] = "Bearer " + api_key_;
                    break;
                case Core::Agent::MODE_CEREBRAS: // Cerebras
                    headers["X-API-Key"] = api_key_;
                    break;
                case Core::Agent::MODE_LLAMA_3B: // Local Ollama
                case Core::Agent::MODE_LLAMA_LATEST:
                case Core::Agent::MODE_LLAMA_31:
                    // No API key needed for local
                    break;
            }
            
            // Use WebService to make the HTTP request
            WebService web_service;
            std::string json_body = payload.dump();

            WebResponse response;
            if (mode_ == Core::Agent::MODE_TOGETHER || mode_ == Core::Agent::MODE_CEREBRAS ||
                mode_ == Core::Agent::MODE_FIREWORKS || mode_ == Core::Agent::MODE_GROQ ||
                mode_ == Core::Agent::MODE_DEEPSEEK || mode_ == Core::Agent::MODE_OPENAI) {
                // Online providers: use POST
                response = web_service.post_json(url, json_body, headers);
            } else {
                // Local providers: use GET with query param (for Ollama)
                std::string full_url = url + "?data=" + json_body;
                response = web_service.fetch_with_headers(full_url, headers);
            }
            
            if (response.status_code != 200) {
                return "Error: AI service returned status code " + std::to_string(response.status_code) + 
                       " - " + response.content;
            }
            
            // Handle streaming response for Cerebras
            if (mode_ == Core::Agent::MODE_CEREBRAS) {
return parse_cerebras_stream(response.content);
            }
            
            // Parse the response based on the service
            auto json_response = nlohmann::json::parse(response.content);
            
            // Handle different response formats
            switch (mode_) {
                case Core::Agent::MODE_TOGETHER: // Together AI
                case Core::Agent::MODE_FIREWORKS: // Fireworks
                case Core::Agent::MODE_GROQ: // Groq
                case Core::Agent::MODE_DEEPSEEK: // DeepSeek
                case Core::Agent::MODE_OPENAI: // OpenAI
                    if (json_response.contains("choices") && !json_response["choices"].empty()) {
                        auto& choice = json_response["choices"][0];
                        if (choice.contains("message") && choice["message"].contains("content")) {
                            return choice["message"]["content"].get<std::string>();
                        } else if (choice.contains("text")) {
                            return choice["text"].get<std::string>();
                        }
                    }
                    break;
                case Core::Agent::MODE_LLAMA_3B: // Local Ollama
                case Core::Agent::MODE_LLAMA_LATEST:
                case Core::Agent::MODE_LLAMA_31:
                    if (json_response.contains("message") && json_response["message"].contains("content")) {
                        return json_response["message"]["content"].get<std::string>();
                    } else if (json_response.contains("response")) {
                        return json_response["response"].get<std::string>();
                    }
                    break;
            }
            
            // If we get here, the response format wasn't as expected
            return "Error: Unexpected response format from AI service: " + response.content;
            
        } catch (const std::exception& e) {
            return "Error: " + std::string(e.what());
        }
        return "Error: Unknown error occurred in AI service";
    }

} // namespace Services
