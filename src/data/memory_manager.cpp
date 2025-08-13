#include "data/memory_manager.h"
#include "services/file_service.h"
#include <fstream>
#include <filesystem>
#include <stdexcept>

#include <chrono>
#include <iomanip>
#include <sstream>
#include <regex>
#include <algorithm>
#include <cstdlib>


namespace Data {

MemoryManager::MemoryManager(const std::string& filename) 
    : memory_file_(filename) 
{
    ensure_memory_directory();
    global_memory_file_ = get_global_memory_path();
}

void MemoryManager::ensure_memory_directory() const {
    try {
        // Ensure directory exists if a path is provided
        std::filesystem::path file_path(memory_file_);
        if (file_path.has_parent_path()) {
            std::filesystem::create_directories(file_path.parent_path());
        }
        
        // Ensure global memory directory exists
        std::filesystem::path global_path(get_global_memory_path());
        if (global_path.has_parent_path()) {
            std::filesystem::create_directories(global_path.parent_path());
        }
    } catch (const std::filesystem::filesystem_error& e) {
        throw std::runtime_error("Failed to create memory directory: " + std::string(e.what()));
    }
}


std::string MemoryManager::get_timestamp() const {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

std::string MemoryManager::get_global_memory_path() const {
        // Get home directory
        const char* home = std::getenv("HOME");
        if (!home) {
            home = std::getenv("USERPROFILE"); // Windows
        }
        if (!home) {
            return "global_memory.md"; // Fallback to current directory
        }
        
        std::filesystem::path global_path = std::filesystem::path(home) / ".llamaware" / "LLAMAWARE.md";
        return global_path.string();
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


        // Add global context
        std::string global_context = get_global_context();
        if (!global_context.empty()) {
            context = global_context + "\n\nRecent interactions:\n" + context;
        }



        return context;
    }

    size_t MemoryManager::get_memory_size() const {
        return load_memory().size();
    }


    void MemoryManager::save_fact(const std::string& fact) {
        std::ofstream file(memory_file_, std::ios::out | std::ios::app);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open memory file for writing: " + memory_file_);
        }

        file << "Fact [" << get_timestamp() << "]: " << fact << "\n";
    }

    void MemoryManager::save_preference(const std::string& preference) {
        std::ofstream file(memory_file_, std::ios::out | std::ios::app);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open memory file for writing: " + memory_file_);
        }

        file << "Preference [" << get_timestamp() << "]: " << preference << "\n";
    }

    std::vector<MemoryEntry> MemoryManager::load_structured_memory() const {
        std::vector<MemoryEntry> entries;
        std::vector<std::string> memory = load_memory();

        for (const auto& line : memory) {
            MemoryEntry entry;
            entry.content = line;
            entry.timestamp = get_timestamp(); // Default timestamp
            entry.type = "interaction"; // Default type

            // Parse structured entries
            if (line.find("Fact [") == 0) {
                entry.type = "fact";
                size_t start = line.find("]: ");
                if (start != std::string::npos) {
                    entry.content = line.substr(start + 3);
                    // Extract timestamp
                    size_t ts_start = line.find("[") + 1;
                    size_t ts_end = line.find("]");
                    if (ts_end != std::string::npos) {
                        entry.timestamp = line.substr(ts_start, ts_end - ts_start);
                    }
                }
            } else if (line.find("Preference [") == 0) {
                entry.type = "preference";
                size_t start = line.find("]: ");
                if (start != std::string::npos) {
                    entry.content = line.substr(start + 3);
                    // Extract timestamp
                    size_t ts_start = line.find("[") + 1;
                    size_t ts_end = line.find("]");
                    if (ts_end != std::string::npos) {
                        entry.timestamp = line.substr(ts_start, ts_end - ts_start);
                    }
                }
            }

            entries.push_back(entry);
        }

        return entries;
    }

std::string MemoryManager::get_facts_context() const {
        std::vector<MemoryEntry> entries = load_structured_memory();
        std::string facts_context;

        for (const auto& entry : entries) {
            if (entry.type == "fact") {
                facts_context += "- " + entry.content + "\n";
            }
        }

        return facts_context.empty() ? "" : "Known facts:\n" + facts_context;
    }

    std::string MemoryManager::get_preferences_context() const {
        std::vector<MemoryEntry> entries = load_structured_memory();
        std::string prefs_context;

        for (const auto& entry : entries) {
            if (entry.type == "preference") {
                prefs_context += "- " + entry.content + "\n";
            }
        }

        return prefs_context.empty() ? "" : "User preferences:\n" + prefs_context;
    }

void MemoryManager::save_global_fact(const std::string& fact) {
        try {
            std::string content;
            
            // Read existing content
            if (Services::FileService::file_exists(global_memory_file_)) {
                std::ifstream file(global_memory_file_);
                if (file.is_open()) {
                    std::ostringstream buffer;
                    buffer << file.rdbuf();
                    content = buffer.str();
                }
            }

            // Find or create the memories section
            const std::string header = "## Llamaware Added Memories";
            size_t header_pos = content.find(header);
            
            if (header_pos == std::string::npos) {
                // Add header and fact
                if (!content.empty() && content.back() != '\n') {
                    content += "\n";
                }
                content += "\n" + header + "\n- " + fact + "\n";
            } else {
                // Find insertion point after header
                size_t insert_pos = header_pos + header.length();
                size_t next_header = content.find("\n## ", insert_pos);
                
                if (next_header == std::string::npos) {
                    // Insert at end
                    content += "\n- " + fact;
                } else {
                    // Insert before next header
                    content.insert(next_header, "\n- " + fact);
                }
            }

            // Write back to file
            std::ofstream file(global_memory_file_);
            if (file.is_open()) {
                file << content;
            }

        } catch (const std::exception&) {
            throw std::runtime_error("Failed to save global fact");
        }
    }

std::string MemoryManager::get_global_context() const {
        if (!Services::FileService::file_exists(global_memory_file_)) {
            return "";
        }

        try {
            std::ifstream file(global_memory_file_);
            if (!file.is_open()) {
                return "";
            }

            std::ostringstream buffer;
            buffer << file.rdbuf();
            std::string content = buffer.str();

            // Extract memories section
            const std::string header = "## Llamaware Added Memories";
            size_t header_pos = content.find(header);
            
            if (header_pos == std::string::npos) {
                return "";
            }

            size_t start_pos = header_pos + header.length();
            size_t end_pos = content.find("\n## ", start_pos);
            
            if (end_pos == std::string::npos) {
                end_pos = content.length();
            }

            std::string memories = content.substr(start_pos, end_pos - start_pos);
            return memories.empty() ? "" : "Global memories:\n" + memories;

        } catch (const std::exception&) {
            return "";
        }
    }

void MemoryManager::clear_global_memory() {
        try {
            if (Services::FileService::file_exists(global_memory_file_)) {
                std::filesystem::remove(global_memory_file_);
            }
        } catch (const std::exception&) {
            throw std::runtime_error("Failed to clear global memory");
        }
    }

std::vector<MemoryEntry> MemoryManager::search_memory(const std::string& query) const {
        std::vector<MemoryEntry> results;
        std::vector<MemoryEntry> all_entries = load_structured_memory();

        std::string lower_query = query;
        std::transform(lower_query.begin(), lower_query.end(), lower_query.begin(), ::tolower);

        for (const auto& entry : all_entries) {
            std::string lower_content = entry.content;
            std::transform(lower_content.begin(), lower_content.end(), lower_content.begin(), ::tolower);
            
            if (lower_content.find(lower_query) != std::string::npos) {
                results.push_back(entry);
            }
        }

        return results;
    }

void MemoryManager::export_memory(const std::string& filename) const {
        std::vector<MemoryEntry> entries = load_structured_memory();
        
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open export file: " + filename);
        }

        file << "# Llamaware Memory Export\n\n";
        file << "Export Date: " << get_timestamp() << "\n\n";

        for (const auto& entry : entries) {
            file << "## " << entry.type << " [" << entry.timestamp << "]\n";
            file << entry.content << "\n\n";
        }
    }

    bool MemoryManager::import_memory(const std::string& filename) {
        if (!Services::FileService::file_exists(filename)) {
            return false;
        }

        try {
            std::ifstream file(filename);
            if (!file.is_open()) {
                return false;
            }

            std::string line;
            while (std::getline(file, line)) {
                if (!line.empty() && line != "# Llamaware Memory Export" && 
                    line.find("Export Date:") == std::string::npos) {
                    
                    std::ofstream memory_file(memory_file_, std::ios::out | std::ios::app);
                    if (memory_file.is_open()) {
                        memory_file << line << "\n";
                    }
                }
            }

            return true;
        } catch (const std::exception&) {
            return false;
        }
    }

    void MemoryManager::save_conversation_state(const std::string& tag) {
        try {
            // Create conversations directory
            std::filesystem::path conv_dir = std::filesystem::path(memory_file_).parent_path() / "conversations";
            std::filesystem::create_directories(conv_dir);
            
            // Save current memory to tagged file
            std::filesystem::path conv_file = conv_dir / (tag + ".txt");
            std::filesystem::copy_file(memory_file_, conv_file, std::filesystem::copy_options::overwrite_existing);
            
        } catch (const std::exception&) {
            throw std::runtime_error("Failed to save conversation state");
        }
    }

    bool MemoryManager::resume_conversation_state(const std::string& tag) {
        try {
            std::filesystem::path conv_dir = std::filesystem::path(memory_file_).parent_path() / "conversations";
            std::filesystem::path conv_file = conv_dir / (tag + ".txt");
            
            if (!std::filesystem::exists(conv_file)) {
                return false;
            }
            
            // Replace current memory with saved state
            std::filesystem::copy_file(conv_file, memory_file_, std::filesystem::copy_options::overwrite_existing);
            return true;
            
        } catch (const std::exception&) {
            return false;
        }
    }

    std::vector<std::string> MemoryManager::list_conversation_states() const {
        std::vector<std::string> conversations;
        
        try {
            std::filesystem::path conv_dir = std::filesystem::path(memory_file_).parent_path() / "conversations";
            
            if (!std::filesystem::exists(conv_dir)) {
                return conversations;
            }
            
            for (const auto& entry : std::filesystem::directory_iterator(conv_dir)) {
                if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                    conversations.push_back(entry.path().stem().string());
                }
            }
            
            std::sort(conversations.begin(), conversations.end());
            
        } catch (const std::exception&) {
            // Return empty vector on error
        }
        
        return conversations;
    }

    void MemoryManager::delete_conversation_state(const std::string& tag) {
        try {
            std::filesystem::path conv_dir = std::filesystem::path(memory_file_).parent_path() / "conversations";
            std::filesystem::path conv_file = conv_dir / (tag + ".txt");
            
            if (std::filesystem::exists(conv_file)) {
                std::filesystem::remove(conv_file);
            }
            
        } catch (const std::exception&) {
            throw std::runtime_error("Failed to delete conversation state");
        }
    }

    std::string MemoryManager::get_compressible_context() const {
        std::vector<MemoryEntry> entries = load_structured_memory();
        
        // Only compress interaction entries, preserve facts and preferences
        std::string compressible_content;
        int interaction_count = 0;
        
        for (const auto& entry : entries) {
            if (entry.type == "interaction") {
                compressible_content += "[" + entry.timestamp + "] " + entry.content + "\n";
                interaction_count++;
            }
        }
        
        if (interaction_count < 5) {
            return ""; // Not enough content to compress
        }
        
        return compressible_content;
    }

    void MemoryManager::compress_memory(const std::string& compressed_summary) {
        try {
            // Load current memory entries
            std::vector<MemoryEntry> entries = load_structured_memory();
            
            // Create backup before compression
            std::string backup_file = memory_file_ + ".backup." + get_timestamp();
            std::filesystem::copy_file(memory_file_, backup_file);
            
            // Clear current memory file
            std::ofstream file(memory_file_, std::ios::trunc);
            if (!file.is_open()) {
                throw std::runtime_error("Could not open memory file for compression");
            }
            
            // Write compressed summary as a single entry
            file << "[" << get_timestamp() << "] COMPRESSED_SUMMARY: " << compressed_summary << std::endl;
            
            // Preserve recent interactions (last 3), facts, and preferences
            int recent_interactions = 0;
            for (auto it = entries.rbegin(); it != entries.rend(); ++it) {
                const auto& entry = *it;
                
                if (entry.type == "interaction" && recent_interactions < 3) {
                    file << "[" << entry.timestamp << "] " << entry.content << std::endl;
                    recent_interactions++;
                } else if (entry.type == "fact" || entry.type == "preference") {
                    file << "[" << entry.timestamp << "] " << entry.type << ": " << entry.content << std::endl;
                }
            }
            
            file.close();
            
        } catch (const std::exception&) {
            throw std::runtime_error("Failed to compress memory");
        }
    }

} // namespace Data
