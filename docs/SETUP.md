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