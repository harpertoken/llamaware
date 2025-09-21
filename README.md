# Llamaware

[![CI](https://github.com/harpertoken/llamaware/actions/workflows/ci.yml/badge.svg)](https://github.com/harpertoken/llamaware/actions/workflows/ci.yml)

Professional AI agent for development. Features: file ops, sessions, extensions, security.

## Quick Start

```bash
make install-deps-ubuntu
cmake -S . -B build && cmake --build build
./build/bin/llamaware-agent
```

## Providers

- Together AI: Llama-3.3-70B
- Cerebras: Llama-4-Maverick-17B
- Offline: Ollama (llama3.2:3b, latest)

## Docs

- [Setup](docs/SETUP.md)
- [Commands](docs/COMMANDS.md)
- [Development](docs/DEVELOPMENT.md)

## License

MIT