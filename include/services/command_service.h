#pragma once
#include <string>
#include <array>

namespace Services {
    class CommandService {
    private:
        static const std::array<std::string, 9> dangerous_commands_;
        static bool is_dangerous_command(const std::string& command);
        
    public:
        static std::string execute(const std::string& command);
    };
}