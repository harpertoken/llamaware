#include "services/file_service.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <limits>

namespace Services {
    static const std::size_t MAX_FILE_SIZE = 5 * 1024 * 1024; // 5 MB limit

    std::string FileService::read_file(const std::string& filename) {
        try {
            if (!file_exists(filename)) {
                return "Error: File '" + filename + "' does not exist";
            }

            std::ifstream file(filename, std::ios::binary | std::ios::ate);
            if (!file.is_open()) {
                return "Error: Could not open file '" + filename + "'";
            }

            auto size = file.tellg();
            if (size > static_cast<std::streampos>(MAX_FILE_SIZE)) {
                return "Error: File too large to read (" + std::to_string(size) + " bytes)";
            }
            file.seekg(0);

            std::ostringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        } catch (const std::exception& e) {
            return std::string("Error reading file: ") + e.what();
        }
    }

    std::string FileService::write_file(const std::string& filename, const std::string& content) {
        try {
            if (content.size() > MAX_FILE_SIZE) {
                return "Error: Content too large to write";
            }

            std::filesystem::path file_path(filename);
            if (file_path.has_parent_path()) {
                std::filesystem::create_directories(file_path.parent_path());
            }

            std::ofstream file(filename, std::ios::binary);
            if (!file.is_open()) {
                return "Error: Could not create/write file '" + filename + "'";
            }

            file << content;
            if (!file) {
                return "Error: Write operation failed";
            }
            return "File '" + filename + "' written successfully (" + std::to_string(content.length()) + " bytes)";
        } catch (const std::exception& e) {
            return std::string("Error writing file: ") + e.what();
        }
    }

    bool FileService::file_exists(const std::string& filename) {
        try {
            return std::filesystem::exists(filename);
        } catch (...) {
            return false;
        }
    }

    std::string FileService::get_file_extension(const std::string& filename) {
        std::filesystem::path file_path(filename);
        return file_path.extension().string();
    }
}
