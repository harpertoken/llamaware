# Changelog

## [2.0.0] - 2025-08-07

### Clean & Optimized Edition

**Interface:**
- Minimal UI with custom ASCII logo
- Clean typography, no visual noise
- Streamlined mode selection (online/offline to provider/model)

**AI Providers:**
- Together AI: Llama-3.3-70B-Instruct-Turbo-Free
- Cerebras: Llama-4-Maverick-17B-128E-Instruct
- Offline: llama3.2:3b, llama3.2:latest

**Commands:**
- `search:query` web search
- `cmd:command` safe shell execution
- `read:file`, `write:file` file operations
- `help`, `version` system info

**Technical:**
- Single MemoryManager instance
- Lazy AI service initialization
- Smart pointers, enum-based modes
- Removed UI dependencies from services
- Centralized input validation

**Breaking Changes:**
- UI redesigned (minimal aesthetic)
- Mode selection flow changed
- Command output simplified
- Cerebras model: gpt-oss-120b to llama-4-maverick-17b

## [1.0.0] - 2025-01-XX

**Initial Release:**
- Basic chat with Llama models
- Online (Together API) and offline (Ollama)
- Web search integration
- Memory system
- Monolithic architecture
