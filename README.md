# Llamaware

At commit hash **ade4cd57e9d8c4538d33b64c30dee9a079a878fe**, Llamaware introduces itself as a professional AI agent for development. It is built to be stable, extensible, and secure, with core features for file operations, session handling, extensions, and provider support.

## Quick Start

Getting started is straightforward. Install dependencies, build the project, and run the agent:

```bash
make install-deps-ubuntu
cmake -S . -B build && cmake --build build
./build/bin/llamaware-agent
````

### Basic Commands

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
