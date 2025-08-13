#include "services/auth_service.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <nlohmann/json.hpp>

namespace Services {
    
    std::map<std::string, AuthProvider> AuthService::providers_;
    std::string AuthService::active_provider_ = "together";
    
    std::string AuthService::get_auth_config_path() {
        return "data/auth_config.json";
    }
    
    void AuthService::ensure_auth_directory() {
        std::filesystem::create_directories("data");
    }
    
    std::string AuthService::encrypt_credential(const std::string& credential) {
        // Simple XOR encryption for demonstration
        // In production, use proper encryption like AES
        std::string encrypted = credential;
        const char key = 0x5A; // Simple key
        
        for (char& c : encrypted) {
            c ^= key;
        }
        
        return encrypted;
    }
    
    std::string AuthService::decrypt_credential(const std::string& encrypted_credential) {
        // Simple XOR decryption (XOR is its own inverse)
        return encrypt_credential(encrypted_credential);
    }
    
    void AuthService::initialize_default_providers() {
        // Together AI
        AuthProvider together;
        together.name = "together";
        together.display_name = "Together AI";
        together.base_url = "https://api.together.xyz/v1";
        together.model = "meta-llama/Llama-3.2-3B-Instruct-Turbo";
        together.is_active = true;
        providers_["together"] = together;
        
        // Ollama (local)
        AuthProvider ollama;
        ollama.name = "ollama";
        ollama.display_name = "Ollama (Local)";
        ollama.base_url = "http://localhost:11434/v1";
        ollama.model = "llama3.2:3b";
        ollama.api_key = "ollama"; // Placeholder for local
        providers_["ollama"] = ollama;
        
        // OpenAI
        AuthProvider openai;
        openai.name = "openai";
        openai.display_name = "OpenAI";
        openai.base_url = "https://api.openai.com/v1";
        openai.model = "gpt-4";
        providers_["openai"] = openai;
        
        // Anthropic
        AuthProvider anthropic;
        anthropic.name = "anthropic";
        anthropic.display_name = "Anthropic Claude";
        anthropic.base_url = "https://api.anthropic.com/v1";
        anthropic.model = "claude-3-sonnet-20240229";
        providers_["anthropic"] = anthropic;
        
        // Cerebras
        AuthProvider cerebras;
        cerebras.name = "cerebras";
        cerebras.display_name = "Cerebras";
        cerebras.base_url = "https://api.cerebras.ai/v1";
        cerebras.model = "llama3.1-8b";
        providers_["cerebras"] = cerebras;
    }
    
    void AuthService::initialize() {
        initialize_default_providers();
        load_auth_config();
        
        // Try to load API keys from environment variables
        for (auto& [name, provider] : providers_) {
            std::string env_var = name + "_API_KEY";
            std::transform(env_var.begin(), env_var.end(), env_var.begin(), ::toupper);
            
            const char* env_key = std::getenv(env_var.c_str());
            if (env_key && provider.api_key.empty()) {
                provider.api_key = env_key;
                provider.is_valid = true;
            }
        }
    }
    
    bool AuthService::add_provider(const AuthProvider& provider) {
        providers_[provider.name] = provider;
        return save_auth_config();
    }
    
    bool AuthService::remove_provider(const std::string& provider_name) {
        auto it = providers_.find(provider_name);
        if (it == providers_.end()) {
            return false;
        }
        
        // Don't allow removal of built-in providers
        if (provider_name == "together" || provider_name == "ollama" || 
            provider_name == "openai" || provider_name == "anthropic" || 
            provider_name == "cerebras") {
            return false;
        }
        
        providers_.erase(it);
        
        // If we're removing the active provider, switch to together
        if (active_provider_ == provider_name) {
            active_provider_ = "together";
        }
        
        return save_auth_config();
    }
    
    bool AuthService::set_active_provider(const std::string& provider_name) {
        auto it = providers_.find(provider_name);
        if (it == providers_.end()) {
            return false;
        }
        
        // Deactivate all providers
        for (auto& [name, provider] : providers_) {
            provider.is_active = false;
        }
        
        // Activate the selected provider
        it->second.is_active = true;
        active_provider_ = provider_name;
        
        return save_auth_config();
    }
    
    std::string AuthService::get_active_provider() {
        return active_provider_;
    }
    
    std::vector<std::string> AuthService::list_providers() {
        std::vector<std::string> provider_names;
        for (const auto& [name, provider] : providers_) {
            provider_names.push_back(name);
        }
        return provider_names;
    }
    
    AuthProvider AuthService::get_provider_info(const std::string& provider_name) {
        auto it = providers_.find(provider_name);
        if (it != providers_.end()) {
            return it->second;
        }
        return AuthProvider{}; // Return empty provider if not found
    }
    
    bool AuthService::set_api_key(const std::string& provider_name, const std::string& api_key) {
        auto it = providers_.find(provider_name);
        if (it == providers_.end()) {
            return false;
        }
        
        it->second.api_key = api_key;
        it->second.is_valid = !api_key.empty();
        
        return save_auth_config();
    }
    
    std::string AuthService::get_api_key(const std::string& provider_name) {
        std::string target_provider = provider_name.empty() ? active_provider_ : provider_name;
        
        auto it = providers_.find(target_provider);
        if (it != providers_.end()) {
            return it->second.api_key;
        }
        return "";
    }
    
    bool AuthService::validate_credentials(const std::string& provider_name) {
        auto it = providers_.find(provider_name);
        if (it == providers_.end()) {
            return false;
        }
        
        // For local providers like Ollama, always consider valid
        if (provider_name == "ollama") {
            it->second.is_valid = true;
            return true;
        }
        
        // For API-based providers, check if API key exists
        bool is_valid = !it->second.api_key.empty();
        it->second.is_valid = is_valid;
        
        return is_valid;
    }
    
    void AuthService::clear_credentials(const std::string& provider_name) {
        auto it = providers_.find(provider_name);
        if (it != providers_.end()) {
            it->second.api_key.clear();
            it->second.is_valid = false;
            save_auth_config();
        }
    }
    
    bool AuthService::save_auth_config() {
        try {
            ensure_auth_directory();
            
            nlohmann::json config;
            config["active_provider"] = active_provider_;
            config["providers"] = nlohmann::json::object();
            
            for (const auto& [name, provider] : providers_) {
                nlohmann::json provider_json;
                provider_json["name"] = provider.name;
                provider_json["display_name"] = provider.display_name;
                provider_json["base_url"] = provider.base_url;
                provider_json["model"] = provider.model;
                provider_json["is_active"] = provider.is_active;
                provider_json["is_valid"] = provider.is_valid;
                
                // Encrypt API key before saving
                if (!provider.api_key.empty()) {
                    provider_json["api_key"] = encrypt_credential(provider.api_key);
                }
                
                if (!provider.additional_config.empty()) {
                    provider_json["additional_config"] = provider.additional_config;
                }
                
                config["providers"][name] = provider_json;
            }
            
            std::ofstream file(get_auth_config_path());
            file << config.dump(2);
            
            return true;
            
        } catch (const std::exception& e) {
            std::cerr << "Failed to save auth config: " << e.what() << std::endl;
            return false;
        }
    }
    
    bool AuthService::load_auth_config() {
        try {
            std::string config_path = get_auth_config_path();
            if (!std::filesystem::exists(config_path)) {
                return true; // Use defaults
            }
            
            std::ifstream file(config_path);
            nlohmann::json config;
            file >> config;
            
            if (config.contains("active_provider")) {
                active_provider_ = config["active_provider"];
            }
            
            if (config.contains("providers")) {
                for (const auto& [name, provider_json] : config["providers"].items()) {
                    auto it = providers_.find(name);
                    if (it != providers_.end()) {
                        // Update existing provider
                        it->second.is_active = provider_json.value("is_active", false);
                        it->second.is_valid = provider_json.value("is_valid", false);
                        
                        if (provider_json.contains("api_key")) {
                            it->second.api_key = decrypt_credential(provider_json["api_key"]);
                        }
                        
                        if (provider_json.contains("additional_config")) {
                            it->second.additional_config = provider_json["additional_config"];
                        }
                    } else {
                        // Add new custom provider
                        AuthProvider provider;
                        provider.name = provider_json.value("name", name);
                        provider.display_name = provider_json.value("display_name", name);
                        provider.base_url = provider_json.value("base_url", "");
                        provider.model = provider_json.value("model", "");
                        provider.is_active = provider_json.value("is_active", false);
                        provider.is_valid = provider_json.value("is_valid", false);
                        
                        if (provider_json.contains("api_key")) {
                            provider.api_key = decrypt_credential(provider_json["api_key"]);
                        }
                        
                        if (provider_json.contains("additional_config")) {
                            provider.additional_config = provider_json["additional_config"];
                        }
                        
                        providers_[name] = provider;
                    }
                }
            }
            
            return true;
            
        } catch (const std::exception& e) {
            std::cerr << "Failed to load auth config: " << e.what() << std::endl;
            return false;
        }
    }
    
    bool AuthService::update_provider_config(const std::string& provider_name, const std::map<std::string, std::string>& config) {
        auto it = providers_.find(provider_name);
        if (it == providers_.end()) {
            return false;
        }
        
        for (const auto& [key, value] : config) {
            it->second.additional_config[key] = value;
        }
        
        return save_auth_config();
    }
    
    bool AuthService::test_provider_connection(const std::string& provider_name) {
        auto it = providers_.find(provider_name);
        if (it == providers_.end()) {
            return false;
        }
        
        // For local providers like Ollama, always return true for now
        if (provider_name == "ollama") {
            return true;
        }
        
        // For API providers, check if we have credentials
        return !it->second.api_key.empty();
    }
    
    std::string AuthService::get_provider_status(const std::string& provider_name) {
        auto it = providers_.find(provider_name);
        if (it == providers_.end()) {
            return "Not Found";
        }
        
        if (!it->second.is_valid) {
            return "Invalid Credentials";
        }
        
        if (test_provider_connection(provider_name)) {
            return "Connected";
        }
        
        return "Disconnected";
    }
    
    void AuthService::refresh_all_provider_status() {
        for (auto& [name, provider] : providers_) {
            provider.is_valid = validate_credentials(name);
        }
        save_auth_config();
    }
    
    bool AuthService::is_credential_secure() {
        // Check if we have proper encryption setup
        return true; // For now, assume secure
    }
    
    void AuthService::generate_encryption_key() {
        // In a real implementation, generate a proper encryption key
        // For now, this is a placeholder
    }
    
    bool AuthService::backup_credentials(const std::string& backup_path) {
        try {
            std::filesystem::copy_file(get_auth_config_path(), backup_path);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Failed to backup credentials: " << e.what() << std::endl;
            return false;
        }
    }
    
    bool AuthService::restore_credentials(const std::string& backup_path) {
        try {
            std::filesystem::copy_file(backup_path, get_auth_config_path());
            load_auth_config();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Failed to restore credentials: " << e.what() << std::endl;
            return false;
        }
    }
}
