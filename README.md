
<div align="center">
  <h1>Llamaware</h1>
  <p>AI agent for code understanding and development workflow automation</p>
  
  [![CI](https://github.com/bniladridas/llamaware/actions/workflows/ci.yml/badge.svg)](https://github.com/bniladridas/llamaware/actions/workflows/ci.yml)
</div>

Llamaware is an AI agent that understands codebases and accelerates development through natural language interaction. It operates as an autonomous assistant, integrating with local and cloud AI models while maintaining security and efficiency.

## Core Capabilities

- **Code Analysis**: Understand and navigate complex codebases
- **File Management**: Read, write, and modify files with precision
- **Context Awareness**: Maintains conversation context and project state
- **Shell Integration**: Safe command execution with confirmation
- **Multi-Model Support**: Local (Ollama) and cloud AI providers
- **Cross-Platform**: Fully functional on Windows, macOS, and Linux

## Installation

### Requirements
- C++17 compiler
- CMake 3.14+
- [Ollama](https://ollama.com/download) (offline mode)
- API keys (cloud providers)

### System Setup
```bash
# macOS
brew install cpr nlohmann-json cmake

# Ubuntu/Debian
sudo apt install nlohmann-json3-dev cmake build-essential git libcurl4-openssl-dev
```

## Quick Start

```bash
git clone https://github.com/bniladridas/llamaware.git
cd llamaware
mkdir build && cd build
cmake .. && make
cp ../.env.example .env
./bin/llamaware-agent
```

Edit `.env` with API keys for cloud services.

## Configuration

### API Keys
```bash
export TOGETHER_API_KEY="your-key"
export CEREBRAS_API_KEY="your-key"
```

### Local Models
```bash
curl -fssl https://ollama.com/install.sh | sh
ollama serve
ollama pull llama3.2:3b
```

## Usage Examples

### Basic Interaction
```
> analyze project structure
> read:src/main.cpp
> search:modern C++ patterns
> cmd:ls -la
```

### Key Commands
- `search:query` - Web search
- `cmd:command` - Execute shell command
- `read:file` - View file
- `write:file text` - Modify file
- `/checkpoint` - Save state
- `/restore [id]` - Load state

## Best Practices

### Security
- Use built-in credential management for API keys
- Review commands before execution
- Run with minimal required privileges

### Performance
- Monitor system resources with local models
- Use checkpoints for complex operations

## Troubleshooting

Common issues and solutions:
- Authentication: Remove `~/.llamaware/auth.json`
- Docker: Verify service status and permissions
- Build: Check dependency installation
- Windows: Update to latest version

## Development

Key components:
- Models: `src/services/ai_service.cpp`
- Interface: `src/utils/ui.cpp`
- Commands: `src/core/agent.cpp`
- Safety: `src/services/command_service.cpp`

---

MIT License — (c) 2025 bniladridas
