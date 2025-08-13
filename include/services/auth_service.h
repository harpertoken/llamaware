#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace Services {
    
    struct AuthProvider {
        std::string name;
        std::string display_name;
        std::string api_key;
        std::string base_url;
        std::string model;
        bool is_active = false;
        bool is_valid = false;
        std::map<std::string, std::string> additional_config;
    };
    
    class AuthService {
    private:
        static std::map<std::string, AuthProvider> providers_;
        static std::string active_provider_;
        static std::string get_auth_config_path();
        static void ensure_auth_directory();
        static std::string encrypt_credential(const std::string& credential);
        static std::string decrypt_credential(const std::string& encrypted_credential);
        static void initialize_default_providers();
        
    public:
        // Provider management
        static void initialize();
        static bool add_provider(const AuthProvider& provider);
        static bool remove_provider(const std::string& provider_name);
        static bool set_active_provider(const std::string& provider_name);
        static std::string get_active_provider();
        static std::vector<std::string> list_providers();
        static AuthProvider get_provider_info(const std::string& provider_name);
        
        // Credential management
        static bool set_api_key(const std::string& provider_name, const std::string& api_key);
        static std::string get_api_key(const std::string& provider_name = "");
        static bool validate_credentials(const std::string& provider_name);
        static void clear_credentials(const std::string& provider_name);
        
        // Configuration management
        static bool save_auth_config();
        static bool load_auth_config();
        static bool update_provider_config(const std::string& provider_name, const std::map<std::string, std::string>& config);
        
        // Health checks
        static bool test_provider_connection(const std::string& provider_name);
        static std::string get_provider_status(const std::string& provider_name);
        static void refresh_all_provider_status();
        
        // Security utilities
        static bool is_credential_secure();
        static void generate_encryption_key();
        static bool backup_credentials(const std::string& backup_path);
        static bool restore_credentials(const std::string& backup_path);
    };
}
