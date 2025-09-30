# Building on macOS

This document outlines the specific requirements and steps for building Llamaware on macOS.

## Prerequisites

- macOS 10.15 (Catalina) or later
- Xcode Command Line Tools
- Homebrew (package manager)
- CMake 3.14 or later
- C++17 compatible compiler (clang++)

## Installation Steps

1. Install Xcode Command Line Tools:
   ```bash
   xcode-select --install
   ```

2. Install Homebrew (if not already installed):
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

3. Install required dependencies:
   ```bash
   brew install cmake pkg-config openssl@3 cpr
   ```
   Note: nlohmann-json is automatically downloaded and built if not found on the system.

## Building the Project

```bash
# Clone the repository (if not already cloned)
git clone https://github.com/harpertoken/llamaware.git
cd llamaware

# Create build directory and run CMake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build build -- -j$(sysctl -n hw.logicalcpu)
```

## Troubleshooting

### Symbol Visibility Issues
If you encounter symbol visibility issues, ensure that:
1. All public symbols are properly exported using `LLAMAWARE_API`
2. No duplicate symbol definitions exist in the codebase
3. The CMake configuration includes proper platform-specific settings

### Common Errors
- **Missing Threads library**: Ensure `find_package(Threads REQUIRED)` is in CMakeLists.txt
- **OpenSSL not found**: Make sure OpenSSL is installed via Homebrew and the path is set correctly
- **C++17 features not available**: Update your compiler or ensure C++17 is enabled in CMake

## Platform-Specific Notes

### macOS Specifics
- The build system automatically handles macOS-specific requirements
- Additional frameworks like `CoreFoundation` and `Security` are linked when needed
- Symbol visibility is configured to work correctly with macOS's dynamic linker

### ARM64 (Apple Silicon) Support
- The build system supports both Intel and Apple Silicon Macs
- For best performance, build natively on your target architecture

## Additional Resources

- [CMake Documentation](https://cmake.org/documentation/)
- [GitHub Repository](https://github.com/harpertoken/llamaware)
- [Issue Tracker](https://github.com/harpertoken/llamaware/issues)
