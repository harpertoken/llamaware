# Development

## Build
**Debug**: `cmake -DCMAKE_BUILD_TYPE=Debug .. && make`
**Release**: `cmake .. && make`

## Pre-Push Validation

### Quick Check
```bash
make test-quick
```

### Full Validation
```bash
make check
```

### Production Check
```bash
make preflight
```

### Manual Verification
```bash
make clean && cmake --build build
```

### Model Testing
```bash
echo -e "2\n1\nversion\nexit" | ./build/bin/llamaware-agent
echo -e "2\n2\nversion\nexit" | ./build/bin/llamaware-agent
echo -e "2\n3\nversion\nexit" | ./build/bin/llamaware-agent
```

### Code Quality
```bash
grep -r "TODO\|FIXME\|HACK" src/ include/
grep -r "system(" src/ include/
```

## Validation Sequence

```bash
make test-quick    # Development
make check         # Major changes
make preflight     # Production
```

## Command Coverage

**test-quick**: startup, shutdown, binary validation
**check**: build process, functionality, help system
**preflight**: environment, dependencies, structure, security

## Dependencies
- C++17
- CMake ≥ 3.14
- nlohmann-json
- cpr
- Ollama (for offline mode)

## Install

### System Dependencies
**macOS**: `brew install cpr nlohmann-json cmake`
**Ubuntu**: `sudo apt install nlohmann-json3-dev cmake build-essential git libcurl4-openssl-dev`

### Online Mode Setup
```bash
# Get API keys from providers
# Together AI: https://api.together.xyz/settings/api-keys
# Cerebras: https://cloud.cerebras.ai/platform

# Set environment variables
export TOGETHER_API_KEY="your-together-api-key-here"
export CEREBRAS_API_KEY="your-cerebras-api-key-here"

# Make persistent (add to ~/.bashrc or ~/.zshrc)
echo 'export TOGETHER_API_KEY="your-key"' >> ~/.bashrc
echo 'export CEREBRAS_API_KEY="your-key"' >> ~/.bashrc

# Reload shell or restart terminal
source ~/.bashrc
```

### Offline Mode Setup (Ollama)
```bash
# Install Ollama
curl -fsSL https://ollama.com/install.sh | sh

# Start Ollama service (keep running)
ollama serve

# In another terminal, download required models
ollama pull llama3.2:3b        # 2GB - Fast, efficient
ollama pull llama3.2:latest    # 4.7GB - More capable
ollama pull llama3.1:latest    # 4.9GB - Previous generation

# Verify models are available
ollama list
```

### Troubleshooting

#### Online Mode Issues
- **API key not set**: Check `echo $TOGETHER_API_KEY` returns your key
- **Invalid API key**: Verify key is correct and account is active
- **Rate limits**: Free tiers have usage limits, upgrade if needed
- **Network issues**: Check internet connection and firewall settings

#### Offline Mode Issues  
- **Service not running**: Ensure `ollama serve` is running in background
- **Connection refused**: Check if port 11434 is available
- **Model not found**: Run `ollama pull <model-name>` to download
- **Permission issues**: May need to restart terminal after Ollama install

## Contributing
Fork → branch → commit → push → pull request