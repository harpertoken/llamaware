
# llamaware

C++ AI agent with online/offline LLM support, shell commands, web search, memory.

## install

**macOS:**
```bash
brew install cpr nlohmann-json cmake
```

**ubuntu/debian:**
```bash
sudo apt update
sudo apt install -y nlohmann-json3-dev cmake build-essential git libcurl4-openssl-dev
git clone --depth=1 https://github.com/libcpr/cpr.git
cmake -S cpr -B cpr/build -DBUILD_CPR_TESTS=OFF -DCPR_USE_SYSTEM_CURL=ON
sudo cmake --build cpr/build --target install
```

**requirements:**
* c++17 compiler
* cmake ≥ 3.14
* [cpr](https://github.com/libcpr/cpr), [nlohmann/json](https://github.com/nlohmann/json)
* [ollama](https://ollama.com/download) (offline)
* API keys: together ai, cerebras, serpapi

## build

```bash
git clone https://github.com/bniladridas/llamaware.git
cd llamaware
mkdir build && cd build
cmake .. && make
```

```bash
cp .env.example .env  # add API keys
```

## run

```bash
export $(cat .env | xargs)
./build/bin/llamaware-agent
```

**optional:**
```bash
make preflight  # run checks
cd web && npm install && npm start  # web ui
```

## usage

on startup:

```
   ███      ███      ███   
   ███      ███      ███   
   ███      ███      ███   
          ███  ███          
          ███  ███          
          ███  ███          
          ███  ███          
          ███  ███          
          ███  ███          

Llamaware - Open Source AI Agent
Type 'help' for commands

Mode [1=Online / 2=Offline]: 1
Provider [1=Together AI / 2=Cerebras]: 2
Cerebras
Ready
```

**models:**
- online: Together AI (Llama-3.3-70B), Cerebras (Llama-4-Maverick-17B)
- offline: llama3.2:3b, llama3.2:latest

**commands:**
```
search:query     web search
cmd:command      run command
read:file        read file
write:file text  write file
help             show help
version          show version
exit             quit
```

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

* clean interface with custom ASCII logo
* multiple AI providers (Together AI, Cerebras, Ollama)
* conversation memory and context
* safe command execution with blocking
* file operations and web search
* open source (MIT license)

## customize

* models: `src/services/ai_service.cpp`
* interface: `src/utils/ui.cpp`
* commands: `src/core/agent.cpp`
* safety: `src/services/command_service.cpp`

## license

MIT — see `LICENSE`
(c) 2025 bniladridas