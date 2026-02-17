[![CI/CD](https://github.com/harpertoken/llamaware/actions/workflows/ci.yml/badge.svg)](https://github.com/harpertoken/llamaware/actions/workflows/ci.yml)
[![agent](https://github.com/harpertoken/llamaware/actions/workflows/agent.yml/badge.svg)](https://github.com/harpertoken/llamaware/actions/workflows/agent.yml)

<img width="648" height="698" alt="Screenshot 2026-02-17 at 7 29 53 PM" src="https://github.com/user-attachments/assets/b3f9b585-3f8d-4ae0-900d-220a993cb2e1" />

The [Llamaware](README.md) is a C++20-based AI development agent. It provides core systems for file management, sessions, extensions, AI providers, and GitHub automation. It is the result of many person-years of discussion and design. Its design encourages general applicability and broad adoption but can be freely copied and modified to meet your organization's needs.

## Getting started

The fastest way to get started:

```bash
git clone https://github.com/harpertoken/llamaware.git
cd llamaware
brew install cmake libpqxx
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
./build/bin/llamaware-agent
```

The agent supports both offline (local Ollama) and online (cloud LLM providers) modes. Select your preferred mode and model when starting the agent.

## Background and scope

The aim of Llamaware is to help developers use AI effectively for coding tasks. By "effectively" we mean fast responses, offline capability, and secure operation. In other words: what would you like your AI agent to do in your development workflow?

The project is focused on relatively higher-level issues, such as agent architecture, service modularity, and provider integration. Such design affects application architecture and library development. Following the architecture may lead to extensible and maintainable code. And it may run fast - but only if you build things right.

We are less concerned with low-level implementation details, such as naming conventions and indentation style. However, no topic that can help build a better AI agent is out of bounds.

Our initial set of features emphasizes stability, extensibility, and security. They may very well be too strict. We expect to have to introduce more options to better accommodate real-world needs. We also need more features.

You will find some of the design decisions contrary to your expectations or even contrary to your experience. If we have not suggested that you change your workflow in any way, we have failed. Please try to verify or disprove our design choices. In particular, we would really like to have some of our decisions backed up with measurements or better examples.

You will find some of the features obvious or even trivial. Please remember that one purpose of a tool is to help someone who is less experienced or coming from a different background or language to get up to speed.

The agent is designed to be extended easily. New services can be added by implementing the service interface. We do not expect you to understand all the internals before trying to use the agent.

Llamaware is meant for gradual introduction into your development workflow. We plan to build tools for that and hope others will too.

## Project Structure

```
src/core/         → agent logic, command routing
src/services/     → AI, file, web, git, MCP, etc.
src/utils/       → config, UI, validation helpers
src/data/        → memory management
tests/           → unit & E2E tests
scripts/         → build & reporting tools
package/docker/  → containerization setup
```

## System Requirements

**Supported Platforms**

- Linux (Ubuntu 20.04+)
- macOS (15.0+)
- Windows (MSVC 2019+)

**Compiler**

- C++20 (GCC 10+, Clang 12+, MSVC 2019 16.11+)

**Dependencies**

- cpr ≥ 1.10.0
- nlohmann-json ≥ 3.10.0
- OpenSSL ≥ 1.1.1
- libpqxx ≥ 7.0 (optional, for PostgreSQL)

## AI Providers

Llamaware supports multiple AI providers:

| Provider    | Description             |
|-------------|------------------------|
| Ollama      | Local offline inference |
| OpenAI      | GPT models             |
| Together AI | Llama models           |
| Cerebras    | Fast inference         |
| Fireworks   | Performance-optimized  |
| Groq        | Speed-focused          |
| DeepSeek    | Reasoning models       |

## Basic Commands

```
version           → show version
search:query      → search the web
cmd:command       → execute shell commands
read:filename     → read file contents
write:filename   → write content to file
help              → show help
exit              → quit
```

## Testing

**Unit Tests**

```bash
cmake -S . -B build
cmake --build build
cd build && ctest --output-on-failure
```

**E2E Tests**

```bash
docker compose -f docker-compose.e2e.yml up -d
docker compose -f docker-compose.e2e.yml logs -f e2e-tests
docker compose -f docker-compose.e2e.yml down
```

## Contributions and LICENSE

Comments and suggestions for improvements are most welcome. We plan to modify and extend this project as our understanding improves and the available libraries improve. More details are found at [CONTRIBUTING](./CONTRIBUTING.md) and [LICENSE](./LICENSE).
