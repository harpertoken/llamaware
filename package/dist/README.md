# Llamaware Agent v2.0 - Professional Edition

A powerful, modular AI agent built with modern C++ that provides seamless interaction with language models and comprehensive system operations.

**Repository:** [https://github.com/bniladridas/Llamaware](https://github.com/bniladridas/Llamaware)

## Features

### Core Capabilities

* **Dual Mode Operation**: Online (Together AI) and Offline (Ollama) support
* **Command Execution**: Safe system command execution with built-in safety checks
* **File Operations**: Read, write, and manage files efficiently
* **Web Search**: Integrated web search capabilities via SerpAPI
* **Persistent Memory**: Advanced conversation history and context management
* **Professional UI**: Clean, colorful terminal interface with status indicators

### Technical Excellence

* **Modular Architecture**: Well-structured, maintainable codebase
* **Modern C++17**: Professional coding standards and practices
* **Error Handling**: Comprehensive error management and user feedback
* **Security Features**: Built-in dangerous command detection and safe operations
* **Performance**: Optimized build configuration and efficient resource usage

## Project Structure

```
├── src/
│   ├── main.cpp                 # Application entry point
│   ├── core/
│   │   └── agent.cpp           # Main agent logic and orchestration
│   ├── services/
│   │   ├── ai_service.cpp      # AI model integration (Together/Ollama)
│   │   ├── command_service.cpp # System command execution with safety
│   │   ├── file_service.cpp    # File operations and management
│   │   └── web_service.cpp     # Web search functionality
│   ├── utils/
│   │   ├── ui.cpp              # User interface utilities and styling
│   │   └── config.cpp          # Configuration and environment management
│   └── data/
│       └── memory_manager.cpp  # Memory and context management
├── include/                     # Header files (organized by namespace)
├── data/                       # Runtime data storage
├── build/                      # Build artifacts
└── .env                        # Environment configuration (optional)
```

## Prerequisites

* **C++17** compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
* **CMake >= 3.14**
* **[cpr](https://github.com/libcpr/cpr)** library for HTTP requests
* **[nlohmann/json](https://github.com/nlohmann/json)** for JSON parsing
* **Ollama** (for offline mode): [https://ollama.com/download](https://ollama.com/download)
* **Together API key** (for online mode): [https://www.together.ai/](https://www.together.ai/)
* **SerpAPI key** (for web search): [https://serpapi.com/](https://serpapi.com/)

### Install Dependencies

**macOS (Homebrew):**

```bash
brew install cpr nlohmann-json cmake
```

**Ubuntu/Debian:**

```bash
sudo apt update
sudo apt install libcpr-dev nlohmann-json3-dev cmake build-essential
```

## Installation

### 1. Clone and Build

```bash
git clone https://github.com/bniladridas/Llamaware.git
cd Llamaware
mkdir build && cd build
cmake ..
make
```

### 2. Configuration (Optional)

Copy and edit the configuration template:

```bash
cp .env.example .env
# Edit .env with your API keys
```

### 3. Setup Ollama (For Offline Mode)

```bash
# Install Ollama
curl -fsSL https://ollama.com/install.sh | sh

# Start Ollama service
ollama serve

# Pull a model (in another terminal)
ollama pull llama3.2:latest
```

## Usage

### Running the Agent

```bash
./build/bin/llamaware-agent
```

### Available Commands

**Direct Commands:**

* `search:query` - Search the web for information
* `cmd:command` - Execute shell commands safely
* `read:filename` - Read file contents
* `write:filename content` - Write content to files

**General Commands:**

* `help` - Show available commands and usage
* `exit` or `quit` - Exit the agent

**Chat Mode:**
Simply type naturally to interact with the AI, which can suggest appropriate commands for your tasks.

### Example Session

```
Choose mode [1=Online / 2=Offline]: 2
✓ Offline mode: llama3.2
ℹ Agent initialized. Type 'help' for commands or 'exit' to quit.

› help
=== Available Commands ===
Direct Commands:
  • search:query     - Search the web
  • cmd:command      - Execute shell command
  • read:filename    - Read file contents
  • write:file content - Write to file

› search:latest AI news
ℹ [Web Search Result]
• Latest AI developments...

› cmd:ls -la
[Executing]: ls -la
[Command Result]
total 48
drwxr-xr-x  12 user  staff   384 Jan 28 10:30 .
...

› what programming languages are trending?
⠋ [AI responds with current trends and suggests: search:programming languages 2025]
```

## Troubleshooting

* **Ollama not found:**

  * Make sure you installed Ollama and it's in your PATH.
* **Ollama port in use:**

  * Only one `ollama serve` can run at a time. Use the running instance.
* **Model not found:**

  * Run `ollama pull llama3.2:latest` to download the model.
* **Together API key missing:**

  * Add your key to `.env` as shown above.
* **JSON parse errors (Ollama):**

  * Make sure you have the latest code with `"stream": false` in the payload.
* **Web search not working:**

  * Add your SerpAPI key to `.env` file.

## Customization

* **Change model:** Edit the model name in `agent.cpp` for Ollama or Together API.
* **Add features:** Multi-turn chat, file I/O, etc.
* **Color scheme:** Adjust ANSI codes in `agent.cpp`.
* **Memory file:** Change the filename in `load_memory()` and `save_to_memory()`.

## License

MIT License - see [LICENSE](LICENSE) file for details.

Copyright (c) 2025 bniladridas
