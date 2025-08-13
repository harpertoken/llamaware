
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

[![CI/CD Pipeline](https://github.com/bniladridas/Llamaware/actions/workflows/ci.yml/badge.svg)](https://github.com/bniladridas/Llamaware/actions/workflows/ci.yml)

```
██      ██       █████  ███    ███  █████  ██     ██  █████  ██████  ███████ 
██      ██      ██   ██ ████  ████ ██   ██ ██     ██ ██   ██ ██   ██ ██      
██      ██      ███████ ██ ████ ██ ███████ ██  █  ██ ███████ ██████  █████   
██      ██      ██   ██ ██  ██  ██ ██   ██ ██ ███ ██ ██   ██ ██   ██ ██      
███████ ███████ ██   ██ ██      ██ ██   ██  ███ ███  ██   ██ ██   ██ ███████ 
```

AI agent with online/offline LLM support, shell commands, web search, memory.

## install

**macOS:** `brew install cpr nlohmann-json cmake`

**ubuntu:** `sudo apt install nlohmann-json3-dev cmake build-essential git libcurl4-openssl-dev && git clone --depth=1 https://github.com/libcpr/cpr.git && cmake -S cpr -B cpr/build -DBUILD_CPR_TESTS=OFF && sudo cmake --build cpr/build --target install`

**requires:** c++17, cmake ≥ 3.14, [ollama](https://ollama.com/download), API keys

## run

```bash
git clone https://github.com/bniladridas/llamaware.git && cd llamaware
mkdir build && cd build && cmake .. && make
cp .env.example .env && export $(cat .env | xargs) && ./build/bin/llamaware-agent

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

```
Mode [1=Online / 2=Offline]: 2
Model [1=llama3.2:3b / 2=llama3.2:latest]: 1
System: Mode=Offline | Model=llama3.2:3b | Memory=Active | Commands=Available
Ready - Type a command or chat naturally:
```

**models:** online (Together AI, Cerebras), offline (llama3.2:3b, llama3.2:latest)
**commands:** `search:query` `cmd:command` `read:file` `write:file text` `help` `version` `exit`

**example:**
```
> search:latest ai news
[results]

> cmd:ls -la
total 48
drwxr-xr-x@ 19 user staff 608 Aug 6 23:04 .

> write:note.txt Hello from Llamaware!
File 'note.txt' written successfully (22 bytes)

> read:note.txt
Hello from Llamaware!
```

## features

enhanced interface • multiple AI providers • conversation memory • safe command execution • file operations • web search

## customize

**models:** `src/services/ai_service.cpp` **interface:** `src/utils/ui.cpp` **commands:** `src/core/agent.cpp` **safety:** `src/services/command_service.cpp`

MIT license — (c) 2025 bniladridas

