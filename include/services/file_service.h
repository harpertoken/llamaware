#pragma once
#include <string>

namespace Services {
    class FileService {
    public:
        static std::string read_file(const std::string& filename);
        static std::string write_file(const std::string& filename, const std::string& content);
        static bool file_exists(const std::string& filename);
        static std::string get_file_extension(const std::string& filename);
    };
}