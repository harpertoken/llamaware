# Llamaware

Llamaware is a professional AI agent for development, built to be stable, extensible, and secure. It provides core features for file operations, session handling, extensions, and provider support.

## Build Status

[![CI](https://github.com/harpertoken/llamaware/actions/workflows/ci.yml/badge.svg)](https://github.com/harpertoken/llamaware/actions/workflows/ci.yml)
[![E2E Tests](https://github.com/harpertoken/llamaware/actions/workflows/e2e.yml/badge.svg)](https://github.com/harpertoken/llamaware/actions/workflows/e2e.yml)

## System Requirements

- **Supported Platforms**:
  - ✅ Linux (Ubuntu 20.04+)
  - ✅ macOS (11.0+)
  - ✅ Windows (MSVC 2019+)
- **Compiler**: C++17 compatible (GCC 9+, Clang 10+, or MSVC 2019+)
- **Build System**: CMake 3.14+
- **Version Control**: Git
- **Dependencies**:
  - cpr 1.10.0+
  - nlohmann-json 3.10.0+
  - OpenSSL 1.1.1+

## Quick Start

### Ubuntu/Debian

1. Install build dependencies:
   ```bash
   sudo apt update
   sudo apt install -y build-essential cmake git libcurl4-openssl-dev
   ```

2. Clone the repository:
   ```bash
   git clone https://github.com/harpertoken/llamaware.git
   cd llamaware
   ```

3. Build the project:
   ```bash
   make install-deps-ubuntu  # Installs remaining dependencies
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
   cmake --build build --config Release -- -j$(nproc)
   ```

4. Run the agent:
   ```bash
   ./build/bin/llamaware-agent
   ```

### macOS

1. Install Xcode command line tools and Homebrew if not already installed.
2. Install dependencies:
   ```bash
   brew install cmake
   ```
3. Clone and build as shown in the Ubuntu instructions.

## Configuration

Create a `.env` file in the project root with your configuration:

```env
# Example configuration
LOG_LEVEL=info
API_KEY=your_api_key_here
```

## Development

### Building with Debug Symbols
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

### Running Tests
```bash
# Unit tests
make test

# End-to-end tests
make test-e2e

# Run tests in Docker
make test-e2e-docker
```

## Basic Commands

Once the agent is running, you can interact with it using simple commands:

```bash
version                # Show version information
search:query           # Search (e.g., search:weather in New York)
cmd:command            # Execute a system command (e.g., cmd:ls -la)
read:/path/to/file.txt # Read a file
write:/path/to/file content  # Write content to a file
help                   # Show help information
exit | quit            # Exit the agent
```

### Example Session

Enter the container, start the agent, and try a few commands:

```bash
docker exec -it llamaware-agent /bin/bash
llamaware-agent

# Inside the agent
version
cmd:ls -la
search:current time
exit
```

### Advanced Usage

**Environment Variables**
Set API keys in the `.env` file at the project root. The container automatically loads them.

**Ollama Integration**
The Ollama API is available at `http://localhost:11434`.
List models with:

```bash
curl http://localhost:11434/api/tags
```

**Persistent Data**
Data is stored in Docker volumes. Check volumes with:

```bash
docker volume ls
```

### Troubleshooting

If commands fail:

```bash
# Verify container is running
docker ps

# View logs
docker logs llamaware-agent
```

Restart the agent:

```bash
docker-compose -f package/docker/docker-compose.yml restart llamaware-agent
```

## Providers

Llamaware supports both online and offline modes:

* **Together AI**: Llama-3.3-70B
* **Cerebras**: Llama-4-Maverick-17B
* **Offline**: Ollama (llama3.2:3b, latest)

## Running Tests

### Unit Tests

```bash
cmake -S . -B build && cmake --build build
cd build && ctest --output-on-failure
```

### End-to-End (E2E) Tests

```bash
docker-compose -f docker-compose.e2e.yml up -d
docker-compose -f docker-compose.e2e.yml logs -f e2e-tests
docker-compose -f docker-compose.e2e.yml down
```

## Documentation

Guides are available to help with setup, usage, and development:

* [Setup](docs/SETUP.md)
* [Commands](docs/COMMANDS.md)
* [Development](docs/DEVELOPMENT.md)
* [Development Workflow](docs/DEVELOPMENT-WORKFLOW.md)
* [CI/CD Pipeline](docs/CI-CD.md)

## Code Structure

* **Main Integration**: `src/core/agent.cpp` – central agent logic and command routing
* **Services**: `src/services/` – AI, file operations, web, git, MCP, and more
* **Core Components**: `src/core/` – agent class and shared functionality
* **Utilities**: `src/utils/` – configuration, UI, validation helpers

## Testing

Run end-to-end tests locally:

```bash
./tests/e2e/run_e2e_tests.sh
```

Or in Docker:

```bash
./tests/e2e/run_tests_in_docker.sh
```

## License

MIT
