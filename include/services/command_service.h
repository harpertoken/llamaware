#pragma once
#include <string>
#include <vector>

namespace Services {
    class CommandService {
    private:
        static std::vector<std::string> dangerous_commands_;
        static bool is_dangerous_command(const std::string& command);
        
    public:
        static std::string execute(const std::string& command);
    };
}