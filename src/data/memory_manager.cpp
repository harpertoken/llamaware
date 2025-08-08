#include "data/memory_manager.h"
#include "services/file_service.h"
#include <fstream>
#include <filesystem>
#include <stdexcept>

namespace Data {

    MemoryManager::MemoryManager(const std::string& filename) 
        : memory_file_(filename) 
    {
        try {
            // Ensure directory exists if a path is provided
            std::filesystem::path file_path(filename);
            if (file_path.has_parent_path()) {
                std::filesystem::create_directories(file_path.parent_path());
            }
        } catch (const std::filesystem::filesystem_error& e) {
            throw std::runtime_error("Failed to create memory directory: " + std::string(e.what()));
        }
    }

    std::vector<std::string> MemoryManager::load_memory() const {
        std::vector<std::string> memory;

        if (!Services::FileService::file_exists(memory_file_)) {
            return memory; // Return empty if file doesn't exist
        }

        std::ifstream file(memory_file_, std::ios::in);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open memory file for reading: " + memory_file_);
        }

        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                memory.push_back(line);
            }
        }

        return memory;
    }

    void MemoryManager::save_interaction(const std::string& user_input, const std::string& response) {
        std::ofstream file(memory_file_, std::ios::out | std::ios::app);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open memory file for writing: " + memory_file_);
        }

        file << "User: " << user_input << "\n";
        file << "Assistant: " << response << "\n";
        file << "---\n"; // Separator for better readability
    }

    void MemoryManager::clear_memory() {
        std::ofstream file(memory_file_, std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open memory file for clearing: " + memory_file_);
        }
        // File is now empty
    }

    std::string MemoryManager::get_context_string() const {
        std::vector<std::string> memory = load_memory();
        std::string context;

        // Limit context to last 20 interactions (40 lines)
        constexpr size_t max_lines = 40;
        size_t start_index = memory.size() > max_lines ? memory.size() - max_lines : 0;

        for (size_t i = start_index; i < memory.size(); ++i) {
            context += memory[i] + "\n";
        }

        return context;
    }

    size_t MemoryManager::get_memory_size() const {
        return load_memory().size();
    }

}
