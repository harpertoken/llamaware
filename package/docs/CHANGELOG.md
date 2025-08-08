# changelog

## [2.0.0] - 2025-08-07

### clean & optimized edition

**interface:**
* minimal UI with custom ASCII logo
* clean typography, no visual noise
* streamlined mode selection (online/offline → provider/model)

**AI providers:**
* Together AI: Llama-3.3-70B-Instruct-Turbo-Free
* Cerebras: Llama-4-Maverick-17B-128E-Instruct
* Offline: llama3.2:3b, llama3.2:latest

**commands:**
* `search:query` - web search
* `cmd:command` - safe shell execution
* `read:file`, `write:file` - file operations
* `help`, `version` - system info

**technical:**
* single MemoryManager instance
* lazy AI service initialization
* smart pointers, enum-based modes
* removed UI dependencies from services
* centralized input validation

**breaking changes:**
* UI redesigned (minimal aesthetic)
* mode selection flow changed
* command output simplified
* Cerebras model: gpt-oss-120b → llama-4-maverick-17b

## [1.0.0] - 2025-01-XX

**initial release:**
* basic chat with Llama models
* online (Together API) and offline (Ollama)
* web search integration
* memory system
* monolithic architecture
