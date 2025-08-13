
# Llamaware

[![CI](https://github.com/bniladridas/llamaware/actions/workflows/ci.yml/badge.svg)](https://github.com/bniladridas/llamaware/actions/workflows/ci.yml)

Llamaware is an AI agent that understands codebases and accelerates development through natural language interaction. It operates as an autonomous assistant, integrating with local and cloud AI models while maintaining security and efficiency.

```bash
# Clone and build
$ git clone https://github.com/bniladridas/llamaware.git
$ cd llamaware
$ mkdir build && cd build
$ cmake .. && make

# Run with default settings
$ ./bin/llamaware-agent
```

## Features

- **Code Understanding**: Navigate and analyze complex codebases
- **File Operations**: Read, write, and modify files with precision
- **Context Management**: Maintains conversation history and project state
- **Shell Integration**: Execute commands safely with confirmation
- **Multi-Model Support**: Local (Ollama) and cloud AI providers
- **Cross-Platform**: Fully functional on Windows, macOS, and Linux

## Quick Start

1. Install dependencies:
   ```bash
   # macOS
   $ brew install cpr nlohmann-json cmake
   
   # Ubuntu/Debian
   $ sudo apt install nlohmann-json3-dev cmake build-essential git libcurl4-openssl-dev
   ```

2. Set up local models (optional):
   ```bash
   $ curl -fssl https://ollama.com/install.sh | sh
   $ ollama serve
   $ ollama pull llama3.2:3b
   ```

3. Configure API keys in `.env`:
   ```bash
   TOGETHER_API_KEY="your-key"
   CEREBRAS_API_KEY="your-key"
   ```

## Usage

### Basic Commands
```
> search:modern C++ patterns     # Web search
> cmd:ls -la                    # Execute shell command
> read:src/main.cpp             # View file
> write:README.md New content   # Modify file
```

### Advanced Features
- `/checkpoint` - Save current state
- `/restore [id]` - Load previous state
- `/theme [name]` - Change UI theme
- `/auth list` - View authentication status

## Security

- Credentials are encrypted using AES-256-GCM
- Commands are validated before execution
- Sensitive operations require confirmation
- Cross-platform secure storage

## Troubleshooting

### Common Issues
- **Build failures**: Verify all dependencies are installed
- **Authentication errors**: Check `.env` configuration
- **Permission issues**: Run with appropriate user privileges

## License

Llamaware is licensed under the [MIT License](LICENSE).

## Development

Key components:
- Models: `src/services/ai_service.cpp`
- Interface: `src/utils/ui.cpp`
- Commands: `src/core/agent.cpp`
- Safety: `src/services/command_service.cpp`