# Llamaware

At commit hash **ade4cd57e9d8c4538d33b64c30dee9a079a878fe**, Llamaware introduces itself as a professional AI agent for development. It is built to be stable, extensible, and secure, with core features for file operations, session handling, extensions, and provider support.

## Quick Start

Getting started is straightforward. Install dependencies, build the project, and run the agent:

```bash
make install-deps-ubuntu
cmake -S . -B build && cmake --build build
./build/bin/llamaware-agent
```

## Providers

Llamaware supports both online and offline modes:

* **Together AI**: Llama-3.3-70B
* **Cerebras**: Llama-4-Maverick-17B
* **Offline**: Ollama (llama3.2:3b, latest)

## Running Tests

### Unit Tests
```bash
# Build and run unit tests
cmake -S . -B build && cmake --build build
cd build && ctest --output-on-failure
```

### End-to-End (E2E) Tests
```bash
# Start test containers in detached mode
docker-compose -f docker-compose.e2e.yml up -d

# Run the tests
docker-compose -f docker-compose.e2e.yml logs -f e2e-tests

# Stop and clean up
docker-compose -f docker-compose.e2e.yml down
```

## Documentation

Guides are available to help with setup, usage, and development:

* [Setup](docs/SETUP.md)
* [Commands](docs/COMMANDS.md)
* [Development](docs/DEVELOPMENT.md)

## Code Structure

The codebase is organized for clarity and maintainability:

* **Main Integration**: `src/core/agent.cpp` – central agent logic and command routing
* **Services**: `src/services/` – AI, file operations, web, git, MCP, and more
* **Core Components**: `src/core/` – agent class and shared functionality
* **Utilities**: `src/utils/` – configuration, UI, validation helpers

## Testing

End-to-end tests can be run locally:

```bash
./tests/e2e/run_e2e_tests.sh
```

Or in Docker for isolation:

```bash
./tests/e2e/run_tests_in_docker.sh
```

## License

MIT