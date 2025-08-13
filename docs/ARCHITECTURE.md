# Architecture

## Components
- Agent Core: interaction loop and command routing
- AI Service: multi-provider LLM integration  
- File Service: operations with validation
- Memory Manager: structured persistence
- Validation System: input safety checks

## Models
- Online: Together AI (Llama-3.3-70B), Cerebras (Llama-4-Maverick)
- Offline: Local Ollama (llama3.2:3b, llama3.2:latest, llama3.1:latest)

## Environment
```bash
TOGETHER_API_KEY=your_key
CEREBRAS_API_KEY=your_key
SERPAPI_KEY=your_key
LLAMAWARE_MEMORY_DIR=~/.llamaware
```

## Structure
```
src/
├── core/           # Agent logic
├── services/       # AI, file, web, command
├── data/           # Memory management
├── utils/          # UI, validation, config
└── main.cpp        # Entry point
```

## Features
- Text replacement with context validation
- Line-range reading
- Pattern search with regex
- Path safety checks
- Global persistence in `~/.llamaware/LLAMAWARE.md`
- Context integration
- Command validation
- Parameter sanitization
- Interactive confirmation
- Error handling
- Color-coded output
- Progress indicators