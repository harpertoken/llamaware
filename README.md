# Llamaware

This project is an advanced, minimal, and colorful CLI agent for interacting with Llama models, supporting both **online (Together API)** and **offline (Ollama)** modes.

**Repository:** [https://github.com/bniladridas/Llamaware](https://github.com/bniladridas/Llamaware)

## Features
- **Interactive CLI**: Type prompts and get responses in a chat-like interface.
- **Dual Mode**:
  - **Online (Together API)**: Access Llama 3.3-70B-Instruct-Turbo-Free in the cloud (requires API key).
  - **Offline (Ollama)**: Use your local Ollama server and any compatible model (e.g., `llama3.2:latest`).
- **Colorful Output**: User prompts, spinner, responses, and errors are color-coded for clarity.
- **Spinner Animation**: Shows a spinner while waiting for a response.
- **Memory System**: Saves conversation history to `memory.txt` and loads it for context.
- **Web Search**: Integration with SerpAPI for web search capabilities (use `search: query`).
- **ASCII Logo**: Cool visual branding with "Ingenuity x Llama Agent".
- **Robust Error Handling**: Clear messages for missing API key, Ollama not running, or model not found.
- **Minimal Output**: Only the essentials—no extra noise.

## Prerequisites
- **C++17** compiler
- **CMake >= 3.14**
- **[cpr](https://github.com/libcpr/cpr)** (HTTP requests)
- **[nlohmann/json](https://github.com/nlohmann/json)** (JSON parsing)
- **Ollama** (for offline mode): [https://ollama.com/download](https://ollama.com/download)
- **Together API key** (for online mode): [https://www.together.ai/](https://www.together.ai/)
- **SerpAPI key** (for web search): [https://serpapi.com/](https://serpapi.com/)

### Install dependencies (macOS example)
```sh
brew install cpr nlohmann-json
```

## Setup

### 1. Clone and build
```sh
git clone https://github.com/bniladridas/Llamaware.git
cd Llamaware
mkdir build
cd build
cmake ..
make
```

### 2. (Optional, for Together API) Create `.env` file in project root
```
TOGETHER_API_KEY=your_together_api_key_here
SERPAPI_KEY=your_serpapi_key_here
```

### 3. (Optional, for Ollama) Install and pull a model
```sh
ollama serve
ollama pull llama3.2:latest
```

## Usage
From your project root:
```sh
./build/agent
```

- **Choose mode:**
  - `1` for Together API (online)
  - `2` for Ollama (offline)
- **Type your prompt** after `You:` and press Enter.
- **Web search:** Use `search: your query` to search the web.
- **See the spinner** while waiting, then the model's response in green.
- **Type `exit` or press Enter on an empty line to quit.**
- **Conversation history** is automatically saved to `memory.txt`.

## Troubleshooting
- **Ollama not found:**
  - Make sure you installed Ollama and it's in your PATH.
- **Ollama port in use:**
  - Only one `ollama serve` can run at a time. Use the running instance.
- **Model not found:**
  - Run `ollama pull llama3.2:latest` to download the model.
- **Together API key missing:**
  - Add your key to `.env` as shown above.
- **JSON parse errors (Ollama):**
  - Make sure you have the latest code with `"stream": false` in the payload.
- **Web search not working:**
  - Add your SerpAPI key to `.env` file.

## Customization
- **Change model:** Edit the model name in `agent.cpp` for Ollama or Together API.
- **Add features:** Multi-turn chat, file I/O, etc.
- **Color scheme:** Adjust ANSI codes in `agent.cpp`.
- **Memory file:** Change the filename in `load_memory()` and `save_to_memory()`.

## License
MIT License - see [LICENSE](LICENSE) file for details.

Copyright (c) 2025 bniladridas