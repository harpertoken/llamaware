#pragma once
#include <string>
#include <vector>

namespace Data {
    class MemoryManager {
    private:
        std::string memory_file_;
        
    public:
        MemoryManager(const std::string& filename = "data/memory.txt");
        
        std::vector<std::string> load_memory() const;
        void save_interaction(const std::string& user_input, const std::string& response);
        void clear_memory();
        std::string get_context_string() const;
        size_t get_memory_size() const;
    };
}