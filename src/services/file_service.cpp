#include "services/file_service.h"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace Services {
    std::string FileService::read_file(const std::string& filename) {
        if (!file_exists(filename)) {
            return "Error: File '" + filename + "' does not exist";
        }
        
        std::ifstream file(filename);
        if (!file.is_open()) {
            return "Error: Could not open file '" + filename + "'";
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    
    std::string FileService::write_file(const std::string& filename, const std::string& content) {
        // Create directory if it doesn't exist
        std::filesystem::path file_path(filename);
        if (file_path.has_parent_path()) {
            std::filesystem::create_directories(file_path.parent_path());
        }
        
        std::ofstream file(filename);
        if (!file.is_open()) {
            return "Error: Could not create/write file '" + filename + "'";
        }
        
        file << content;
        return "File '" + filename + "' written successfully (" + std::to_string(content.length()) + " bytes)";
    }
    
    bool FileService::file_exists(const std::string& filename) {
        return std::filesystem::exists(filename);
    }
    
    std::string FileService::get_file_extension(const std::string& filename) {
        std::filesystem::path file_path(filename);
        return file_path.extension().string();
    }
}