# Setup

## Quick Start

```bash
# macOS
brew install cpr nlohmann-json cmake

# Ubuntu
sudo apt install nlohmann-json3-dev cmake build-essential libcurl4-openssl-dev

# Build
git clone https://github.com/harpertoken/llamaware.git
cd llamaware
cmake -S . -B build && cmake --build build

# Run
./build/bin/llamaware-agent
```

## Platform-Specific Setup

### macOS

#### Prerequisites
- macOS 10.15 (Catalina) or later
- Xcode Command Line Tools
- Homebrew (package manager)
- CMake 3.14 or later

#### Installation Steps
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

#### Building on macOS
```bash
# Create build directory and run CMake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build the project (using all available cores)
cmake --build build -- -j$(sysctl -n hw.logicalcpu)
```

#### macOS-Specific Notes
- The build system automatically handles macOS-specific requirements
- Additional frameworks like `CoreFoundation` and `Security` are linked when needed
- Symbol visibility is configured to work correctly with macOS's dynamic linker
- Supports both Intel and Apple Silicon Macs (build natively for best performance)

#### Troubleshooting macOS
- **Missing Threads library**: Ensure `find_package(Threads REQUIRED)` is in CMakeLists.txt
- **OpenSSL not found**: Make sure OpenSSL is installed via Homebrew and the path is set correctly
- **C++17 features not available**: Update your compiler or ensure C++17 is enabled in CMake
- **Symbol visibility issues**: Ensure all public symbols are properly exported using `LLAMAWARE_API`

### Ubuntu/Linux

#### Prerequisites
- Ubuntu 18.04 or later (or equivalent Linux distribution)
- GCC 7+ or Clang 6+ (C++17 support)
- CMake 3.14 or later

#### Installation
```bash
sudo apt update
sudo apt install nlohmann-json3-dev cmake build-essential libcurl4-openssl-dev pkg-config
```

## Offline Mode

```bash
# Install Ollama
curl -fsSL https://ollama.com/install.sh | sh

# Pull models
ollama pull llama3.2:3b
ollama pull llama3.2:latest
```

## Online Mode

Set API keys in .env:
```
TOGETHER_API_KEY=your_key
CEREBRAS_API_KEY=your_key
```

## Docker

```bash
cd package/docker
docker-compose up --build
```
