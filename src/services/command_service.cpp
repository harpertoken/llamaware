#include "services/command_service.h"

#include "utils/platform.h"

#include <cstdio>
#include <cstring>
#include <array>
#include <regex>
#include <stdexcept>
#include <future>
#include <chrono>

namespace Services {
    const std::array<std::string, 9> CommandService::dangerous_commands_ = {
        "rm", "sudo rm", "format", "del /", "shutdown", "reboot",
        "mkfs", "fdisk", "dd"
    };

    bool CommandService::is_dangerous_command(const std::string& command) {
        // Token-based matching to avoid substring false positives
        for (const auto& danger : dangerous_commands_) {
            std::regex pattern("\\b" + std::regex_replace(danger, std::regex(" "), "\\s+") + "\\b");
            if (std::regex_search(command, pattern)) {
                return true;
            }
        }
        return false;
    }

    std::string CommandService::execute_command(const std::string& command, FILE* pipe) {
        if (!pipe) {
            throw std::runtime_error("Failed to execute command");
        }

        std::string result;
        char buffer[256];
        size_t total_bytes = 0;
        const size_t MAX_BYTES = 10 * 1024; // 10 KB limit

        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            total_bytes += strlen(buffer);
            if (total_bytes > MAX_BYTES) {
                result += "\n[Output truncated]";
                break;
            }
            result += buffer;
        }

        int exit_code = Utils::Platform::close_process(pipe);
        if (exit_code != 0) {
            result += "\nExit code: " + std::to_string(exit_code);
        }

        return result.empty() ? "Command completed" : result;
    }

    std::string CommandService::execute(const std::string& command) {
        if (is_dangerous_command(command)) {
            return "Error: Dangerous command blocked";
        }

        try {
            auto future = std::async(std::launch::async, [&]() -> std::string {
                // Use platform-agnostic process handling
                FILE* pipe = Utils::Platform::open_process(
                    command + Utils::Platform::get_shell_redirect_both(), 
                    "r"
                );
                return execute_command(command, pipe);
            });

            // Timeout: 5 seconds
            if (future.wait_for(std::chrono::seconds(5)) == std::future_status::timeout) {
                return "Error: Command timed out";
            }
            return future.get();
        } catch (const std::exception& e) {
            return std::string("Error executing command: ") + e.what();
        }
    }
}
                }

                int exit_code = pclose(pipe);
                if (exit_code != 0) {
                    result += "\nExit code: " + std::to_string(exit_code);
                }
                return result.empty() ? "Command completed" : result;
            });

            // Timeout: 5 seconds
            if (future.wait_for(std::chrono::seconds(5)) == std::future_status::timeout) {
                return "Error: Command timed out";
            }
            return future.get();
        } catch (const std::exception& e) {
            return std::string("Error executing command: ") + e.what();
        }
    }
}
