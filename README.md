
# Llamaware

[![CI](https://github.com/harpertoken/llamaware/actions/workflows/ci.yml/badge.svg)](https://github.com/harpertoken/llamaware/actions/workflows/ci.yml)
[![CodeQL](https://github.com/harpertoken/llamaware/actions/workflows/codeql.yml/badge.svg)](https://github.com/harpertoken/llamaware/actions/workflows/codeql.yml)

**Professional AI Agent with 16 Enterprise Features**

Llamaware is an autonomous AI assistant that understands codebases and accelerates development through natural language interaction. It provides a comprehensive development environment with advanced file operations, session management, extensibility, and security features.

## Table of Contents

- [Enterprise Features](#enterprise-features)
- [Quick Start](#quick-start)
- [AI Providers](#ai-providers)
- [Web Interface](#web-interface)
- [Architecture](#architecture)
- [Prerequisites](#prerequisites)
- [Docker Deployment](#docker-deployment)
- [Usage Examples](#usage-examples)
- [Security Features](#security-features)
- [Documentation](#documentation)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## Enterprise Features

**16 features across 4 categories:**

### Files (4 features)
- **File Injection**: `@file` and `@directory` commands for context management
- **Advanced Operations**: Read, write, replace, and search with validation
- **Context Integration**: Hierarchical context with LLAMAWARE.md files
- **Shell Integration**: Interactive shell with toggle mode

### Sessions (4 features)
- **Session Management**: Save/resume conversations and project state
- **Tool Registry**: Comprehensive tool management and discovery
- **Configuration**: Layered configuration system with persistence
- **Compression**: AI-powered context compression for efficiency

### Extensions (4 features)
- **MCP Support**: Model Context Protocol server integration
- **Checkpointing**: File-based checkpoints with backup/restore
- **Web Integration**: Advanced web fetch and search capabilities
- **Filtering**: Git-aware advanced file filtering

### Security (4 features)
- **Themes**: Multi-theme system with ANSI color support
- **Authentication**: Provider management and secure credential storage
- **Sandboxing**: Docker-based execution environment
- **Error Handling**: Enhanced validation and error reporting

## Quick Start

### Native Installation

```bash
# Clone repository
git clone https://github.com/harpertoken/llamaware.git
cd llamaware

# Install dependencies
# macOS
brew install cpr nlohmann-json cmake

# Ubuntu/Debian
sudo apt install nlohmann-json3-dev cmake build-essential git libcurl4-openssl-dev

# Build
mkdir build && cd build
cmake .. && make

# Run
./bin/llamaware-agent
```

## AI Providers

### Online Mode (Cloud AI)
- **Together AI**: Meta Llama-3.3-70B with free tier
- **Cerebras**: Ultra-fast Llama-4-Maverick with streaming

### Offline Mode (Local AI)
- **Ollama Integration**: Local Llama models (3.2:3b, 3.2:latest, 3.1:latest)
- **Privacy-First**: No data sent to external services
- **Resource Efficient**: Runs on local hardware

## Web Interface

Professional React showcase demonstrating all enterprise features:

```bash
cd web
npm install
npm start
```

Features:
- Interactive terminal demonstrations
- Feature showcase with animations
- Cross-platform installation guides
- Responsive design for all devices

## Architecture

```
src/
├── core/           # Agent logic and command routing
├── services/       # AI, file, web, command services
├── data/           # Memory management and persistence
├── utils/          # UI, validation, and configuration
└── main.cpp        # Application entry point
```

## Prerequisites

### System Requirements
- **OS**: Linux, macOS, or Windows
- **RAM**: 8GB minimum (16GB+ recommended for offline mode)
- **Storage**: 10GB+ for models and data
- **C++17**: Compatible compiler (GCC 9+, Clang 10+, MSVC 2019+)

### Dependencies
```bash
# macOS
brew install cpr nlohmann-json cmake

# Ubuntu/Debian
sudo apt install nlohmann-json3-dev cmake build-essential git libcurl4-openssl-dev

# Windows (via vcpkg)
vcpkg install cpr nlohmann-json
```

## Docker Deployment

### Quick Start with Docker Compose
```bash
cd package/docker
docker-compose up --build
```

### Advanced Docker Setup

#### With Ollama (Offline Mode)
```bash
cd package/docker
docker-compose --profile ollama up --build
```

#### Environment Configuration
Create a `.env` file with your API keys:
```bash
TOGETHER_API_KEY="your-together-key"
CEREBRAS_API_KEY="your-cerebras-key"
SERPAPI_KEY="your-serpapi-key"
```

#### Manual Docker Commands
```bash
# Build image
make docker-build

# Run container
docker run -it --rm \
  -v $(pwd)/.env:/home/llamaware/.env:ro \
  -v llamaware_data:/home/llamaware/data \
  llamaware/agent
```

## Usage Examples

### Basic Commands
```bash
# File operations
> @file src/main.cpp              # Inject file into context
> @directory src/                 # Inject entire directory
> read:src/main.cpp               # Read file content
> write:output.txt Hello World    # Write to file
> grep:TODO:src:*.cpp             # Search for patterns

# AI interaction
> What is this codebase about?     # Ask questions
> search:modern C++ patterns      # Web search
> remember:strict TypeScript      # Store facts

# Session management
> /save project                   # Save current session
> /resume project                 # Resume saved session
> /checkpoint v1.0                # Create checkpoint
> /compress                       # Compress context
```

### Advanced Features
```bash
# MCP (Model Context Protocol)
> /mcp servers                    # List available servers
> /mcp tools ollama               # List server tools

# Web integration
> /fetch https://api.github.com/user json
> search:latest React patterns

# Security & theming
> /theme set dark                 # Change theme
> /auth providers                 # List auth providers
> /sandbox run "npm test"         # Run in sandbox
```

### Interactive Shell
```bash
> !                             # Toggle shell mode
$ ls -la                        # Execute shell commands
$ git status                    # Git operations
$ exit                          # Return to agent mode
```

## Security Features

- **Encrypted Credentials**: AES-256-GCM encryption for API keys
- **Command Validation**: All commands validated before execution
- **Sandbox Execution**: Docker-based sandboxing for safe operations
- **Secure Storage**: Cross-platform secure credential management
- **Input Sanitization**: Comprehensive parameter validation
- **Error Handling**: Enhanced error reporting and recovery

## Documentation

### Core Documentation
- **[Setup Guide](docs/SETUP.md)** - Complete installation and configuration
- **[Commands Reference](docs/COMMANDS.md)** - All 16 enterprise features
- **[Architecture](docs/ARCHITECTURE.md)** - System design and components
- **[Development](docs/DEVELOPMENT.md)** - Contributing and development workflow

### Project Structure
```
├── src/                    # C++ source code
│   ├── core/              # Agent logic
│   ├── services/          # AI, file, web services
│   ├── data/              # Memory management
│   └── utils/             # UI and utilities
├── web/                   # React web interface
├── package/               # Distribution packages
│   ├── docker/           # Container setup
│   ├── binary/           # Binary distributions
│   └── scripts/          # Build automation
├── docs/                  # Documentation
└── include/              # C++ headers
```

## Troubleshooting

### Build Issues
```bash
# Clean and rebuild
make clean
mkdir build && cd build
cmake .. && make

# Check dependencies
cmake --version
pkg-config --exists nlohmann_json
```

### API Configuration
```bash
# Verify API keys
echo $TOGETHER_API_KEY
echo $CEREBRAS_API_KEY

# Test API connectivity
curl -H "Authorization: Bearer $TOGETHER_API_KEY" \
     https://api.together.xyz/v1/models
```

### Offline Mode Issues
```bash
# Check Ollama service
curl http://localhost:11434/api/tags

# Start Ollama if needed
ollama serve &

# Download models
ollama pull llama3.2:3b
ollama list
```

### Performance Optimization
```bash
# Memory limits (optional)
export OLLAMA_MAX_LOADED_MODELS=1

# Thread configuration
export OLLAMA_NUM_PARALLEL=4
```

## Contributing

We welcome contributions! Please see our [Development Guide](docs/DEVELOPMENT.md) for:

- Development setup and workflow
- Code standards and testing
- Pull request guidelines
- Pre-push validation steps

### Quick Development Setup
```bash
# Fork and clone
git clone https://github.com/your-username/llamaware.git
cd llamaware

# Install dependencies
make install-deps-ubuntu  # or install-deps-mac

# Build in debug mode
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug .. && make

# Run tests
make test
```

## License

Llamaware is licensed under the [MIT License](LICENSE).

## Acknowledgments

- **Together AI** for providing free access to Llama models
- **Cerebras** for ultra-fast inference capabilities
- **Ollama** for local AI model support
- **Open Source Community** for the amazing libraries and tools

---

**Built for developers, by developers**