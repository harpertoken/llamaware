#include "data/memory_manager.h"
#include "services/file_service.h"
#include <fstream>
#include <filesystem>

namespace Data {
    MemoryManager::MemoryManager(const std::string& filename) : memory_file_(filename) {
        // Ensure data directory exists
        std::filesystem::path file_path(filename);
        if (file_path.has_parent_path()) {
            std::filesystem::create_directories(file_path.parent_path());
        }
    }
    
    std::vector<std::string> MemoryManager::load_memory() {
        std::vector<std::string> memory;
        
        if (!Services::FileService::file_exists(memory_file_)) {
            return memory; // Return empty if file doesn't exist
        }
        
        std::ifstream file(memory_file_);
        std::string line;
        
        while (std::getline(file, line)) {
            if (!line.empty()) {
                memory.push_back(line);
            }
        }
        
        return memory;
    }
    
    void MemoryManager::save_interaction(const std::string& user_input, const std::string& response) {
        std::ofstream file(memory_file_, std::ios::app);
        if (file.is_open()) {
            file << "User: " << user_input << "\n";
            file << "Assistant: " << response << "\n";
            file << "---\n"; // Separator for better readability
        }
    }
    
    void MemoryManager::clear_memory() {
        std::ofstream file(memory_file_, std::ios::trunc);
        // File is now empty
    }
    
    std::string MemoryManager::get_context_string() {
        std::vector<std::string> memory = load_memory();
        std::string context;
        
        // Limit context to last 20 interactions to avoid token limits
        size_t start_index = memory.size() > 40 ? memory.size() - 40 : 0;
        
        for (size_t i = start_index; i < memory.size(); ++i) {
            context += memory[i] + "\n";
        }
        
        return context;
    }
    
    size_t MemoryManager::get_memory_size() {
        return load_memory().size();
    }
}