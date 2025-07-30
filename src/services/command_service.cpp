#include "services/command_service.h"
#include "utils/ui.h"
#include <iostream>
#include <cstdio>

namespace Services {
    std::vector<std::string> CommandService::dangerous_commands_ = {
        "rm -rf", "sudo rm", "format", "del /", "shutdown", "reboot", 
        "mkfs", "fdisk", "dd if=", ":(){ :|:& };:", "chmod -R 777 /"
    };
    
    bool CommandService::is_dangerous_command(const std::string& command) {
        for (const auto& danger : dangerous_commands_) {
            if (command.find(danger) != std::string::npos) {
                return true;
            }
        }
        return false;
    }
    
    std::string CommandService::execute(const std::string& command) {
        std::cout << Utils::UI::YELLOW << "[Executing]: " << command << Utils::UI::RESET << std::endl;
        
        // Safety check
        if (is_dangerous_command(command)) {
            return "SAFETY: Dangerous command blocked. Command: " + command;
        }
        
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            return "Error: Failed to execute command";
        }
        
        std::string result;
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        
        int exit_code = pclose(pipe);
        if (exit_code != 0) {
            result += "\nExit code: " + std::to_string(exit_code);
        }
        
        return result.empty() ? "Command executed successfully (no output)" : result;
    }
}