#pragma once
#include <string>
#include <vector>

namespace Data {
    struct MemoryEntry {
        std::string content;
        std::string timestamp;
        std::string type; // "interaction", "fact", "preference", etc.
    };

    class MemoryManager {
    private:
        std::string memory_file_;
        std::string global_memory_file_;
        
        void ensure_memory_directory() const;
        std::string get_timestamp() const;
        std::string get_global_memory_path() const;
        
    public:
        MemoryManager(const std::string& filename = "data/memory.txt");
        

        // Basic memory operations


        std::vector<std::string> load_memory() const;
        void save_interaction(const std::string& user_input, const std::string& response);
        void clear_memory();
        std::string get_context_string() const;
        size_t get_memory_size() const;

        
        // Enhanced memory operations
        void save_fact(const std::string& fact);
        void save_preference(const std::string& preference);
        std::vector<MemoryEntry> load_structured_memory() const;
        std::string get_facts_context() const;
        std::string get_preferences_context() const;
        
        // Global memory (persistent across sessions)
        void save_global_fact(const std::string& fact);
        std::string get_global_context() const;
        void clear_global_memory();
        
        // Memory search and management
        std::vector<MemoryEntry> search_memory(const std::string& query) const;
        void export_memory(const std::string& filename) const;
        bool import_memory(const std::string& filename);
        
        // Conversation state management
        void save_conversation_state(const std::string& tag);
        bool resume_conversation_state(const std::string& tag);
        std::vector<std::string> list_conversation_states() const;
        void delete_conversation_state(const std::string& tag);
        
        // Context compression
        void compress_memory(const std::string& compressed_summary);
        std::string get_compressible_context() const;

    };
}