
# llamaware

[![ci/cd pipeline](https://github.com/bniladridas/llamaware/actions/workflows/ci.yml/badge.svg)](https://github.com/bniladridas/llamaware/actions/workflows/ci.yml)

```
██      ██       █████  ███    ███  █████  ██     ██  █████  ██████  ███████ 
██      ██      ██   ██ ████  ████ ██   ██ ██     ██ ██   ██ ██   ██ ██      
██      ██      ███████ ██ ████ ██ ███████ ██  █  ██ ███████ ██████  █████   
██      ██      ██   ██ ██  ██  ██ ██   ██ ██ ███ ██ ██   ██ ██   ██ ██      
███████ ███████ ██   ██ ██      ██ ██   ██  ███ ███  ██   ██ ██   ██ ███████ 
```

ai agent that understands your codebase and accelerates development workflows.

with llamaware you can:
- analyze codebase structure and identify main components
- track git history and understand recent changes
- read and write files with advanced pattern matching
- remember facts and preferences across sessions
- execute shell commands with safety confirmation
- search the web for current information
- chat with ai models (together ai, cerebras, or local ollama)

## setup

```bash
# macos
brew install cpr nlohmann-json cmake

# ubuntu
sudo apt install nlohmann-json3-dev cmake build-essential git libcurl4-openssl-dev

# build
git clone https://github.com/bniladridas/llamaware.git
cd llamaware && mkdir build && cd build && cmake .. && make
./bin/llamaware-agent
```

### online mode
```bash
export TOGETHER_API_KEY="your-key"
export CEREBRAS_API_KEY="your-key"
```

### offline mode
```bash
curl -fsSL https://ollama.com/install.sh | sh
ollama serve
ollama pull llama3.2:3b
```

## usage

```bash
cd your-project/
./build/bin/llamaware-agent

> analyze the main components of this codebase
> read:config.json
> replace:app.js:oldFunc:newFunc
> remember:this project uses typescript strict mode
> search:latest react patterns
```

[commands](docs/COMMANDS.md) | [setup](docs/SETUP.md)

mit license