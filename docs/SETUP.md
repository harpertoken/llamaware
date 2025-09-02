# Llamaware Setup Guide

Complete setup instructions for Llamaware AI agent with both online and offline capabilities.

## Quick Start

```bash
# 1. Install system dependencies
# macOS: brew install cpr nlohmann-json cmake
# Ubuntu: sudo apt install nlohmann-json3-dev cmake build-essential git libcurl4-openssl-dev

# 2. Install Ollama for offline mode
curl -fsSL https://ollama.com/install.sh | sh

# 3. Clone and build
git clone https://github.com/harpertoken/llamaware.git
cd llamaware && mkdir build && cd build && cmake .. && make

# 4. Setup offline models
ollama serve &  # Start in background
ollama pull llama3.2:3b

# 5. Run
./bin/llamaware-agent
```

## Detailed Setup

### 1. System Dependencies

#### macOS
```bash
# Using Homebrew
brew install cpr nlohmann-json cmake

# Verify installation
cmake --version
```

#### Ubuntu/Debian
```bash
# Install build tools and libraries
sudo apt update
sudo apt install nlohmann-json3-dev cmake build-essential git libcurl4-openssl-dev

# Verify installation
cmake --version
gcc --version
```

### 2. Ollama Installation

Ollama is required for offline AI capabilities. It provides local language model inference.

#### Install Ollama
```bash
# Official installer (Linux/macOS)
curl -fsSL https://ollama.com/install.sh | sh

# Alternative: Manual download from https://ollama.com/download
```

#### Start Ollama Service
```bash
# Start Ollama server (keep this running)
ollama serve

# Or run in background
ollama serve &

# Verify service is running
curl http://localhost:11434/api/tags
```

#### Download Models
```bash
# Essential models for Llamaware
ollama pull llama3.2:3b        # 2.0GB - Fast, efficient for development
ollama pull llama3.2:latest    # 4.7GB - More capable, better responses  
ollama pull llama3.1:latest    # 4.9GB - Previous generation, stable

# Verify models are downloaded
ollama list
```

### 3. Build Llamaware

```bash
# Clone repository
git clone https://github.com/bniladridas/llamaware.git
cd llamaware

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Verify build
ls bin/llamaware-agent
```

### 4. Online Mode Setup (Cloud AI Providers)

Online mode provides access to powerful cloud-based AI models with no local resource requirements.

#### Together AI Setup
Together AI offers free tier access to Meta's Llama models.

```bash
# 1. Sign up at https://api.together.xyz/
# 2. Navigate to Settings > API Keys
# 3. Create a new API key
# 4. Set environment variable
export TOGETHER_API_KEY="your-api-key-here"

# Make persistent
echo 'export TOGETHER_API_KEY="your-api-key-here"' >> ~/.bashrc
source ~/.bashrc

# Verify setup
echo $TOGETHER_API_KEY
```

**Together AI Features:**
- **Model**: `meta-llama/Llama-3.3-70B-Instruct-Turbo-Free`
- **Free Tier**: Available with rate limits
- **Performance**: High-quality responses, cloud-powered
- **Use Case**: Production applications, complex reasoning

#### Cerebras Setup  
Cerebras provides ultra-fast inference with their specialized hardware.

```bash
# 1. Sign up at https://cloud.cerebras.ai/
# 2. Navigate to Platform > API Keys  
# 3. Generate new API key
# 4. Set environment variable
export CEREBRAS_API_KEY="your-api-key-here"

# Make persistent
echo 'export CEREBRAS_API_KEY="your-api-key-here"' >> ~/.bashrc
source ~/.bashrc

# Verify setup
echo $CEREBRAS_API_KEY
```

**Cerebras Features:**
- **Model**: `llama-4-maverick-17b-128e-instruct`
- **Speed**: Ultra-fast inference (sub-second responses)
- **Streaming**: Real-time response streaming
- **Use Case**: Interactive applications, real-time chat

#### API Key Security
```bash
# Option 1: Use .env file (recommended for development)
echo "TOGETHER_API_KEY=your-key-here" > .env
echo "CEREBRAS_API_KEY=your-key-here" >> .env
echo ".env" >> .gitignore

# Option 2: System environment (recommended for production)
# Add to /etc/environment or systemd service files

# Option 3: Shell profile (convenient for personal use)
# Already shown above with ~/.bashrc
```

## Usage

### Starting Llamaware
```bash
cd llamaware
./build/bin/llamaware-agent
```

### Mode Selection

#### Mode 1: Online (Cloud AI)
- **Requirements**: API keys, internet connection
- **Advantages**: No local resources, latest models, high performance
- **Providers**: Together AI (free tier), Cerebras (ultra-fast)
- **Best For**: Production apps, complex tasks, limited local resources

#### Mode 2: Offline (Local AI)  
- **Requirements**: Ollama installed, models downloaded
- **Advantages**: Privacy, no internet needed, no API costs
- **Models**: Multiple Llama variants available locally
- **Best For**: Privacy-sensitive work, development, offline environments

### Provider/Model Selection

#### Online Providers
- **Together AI**: `meta-llama/Llama-3.3-70B-Instruct-Turbo-Free`
  - Free tier available
  - High-quality responses
  - Good for general use
  
- **Cerebras**: `llama-4-maverick-17b-128e-instruct`  
  - Ultra-fast inference
  - Real-time streaming
  - Best for interactive use

#### Offline Models
- **Option 1**: `llama3.2:3b` - Fast, lightweight (2GB)
- **Option 2**: `llama3.2:latest` - Balanced performance (4.7GB)  
- **Option 3**: `llama3.1:latest` - Stable, proven (4.9GB)

## Troubleshooting

### Common Issues

#### Online Mode Issues

**"API key not set" or "API service error"**
```bash
# Check if API key is set
echo $TOGETHER_API_KEY
echo $CEREBRAS_API_KEY

# If empty, set the key
export TOGETHER_API_KEY="your-key-here"

# Test API connection
curl -H "Authorization: Bearer $TOGETHER_API_KEY" \
     https://api.together.xyz/v1/models
```

**Rate limiting or quota exceeded**
- Check your usage at provider dashboard
- Upgrade to paid tier if needed
- Wait for rate limit reset (usually hourly/daily)

**Network/firewall issues**
- Check internet connection
- Verify firewall allows HTTPS (443) outbound
- Test with: `curl -I https://api.together.xyz`

#### Offline Mode Issues

**"Offline server not found"**
```bash
# Check if Ollama is running
curl http://localhost:11434/api/tags

# If not running, start Ollama
ollama serve
```

**"Model not found"**
```bash
# Check available models
ollama list

# Download missing model
ollama pull llama3.2:3b
```

#### Build Errors
```bash
# Clean and rebuild
make clean
cmake .. && make

# Check dependencies
cmake --version
pkg-config --exists nlohmann_json && echo "JSON library found"
```

#### Permission Issues
```bash
# After Ollama install, restart terminal or reload shell
source ~/.bashrc

# Check Ollama installation
which ollama
ollama --version
```

### Performance Tips

#### Model Selection
- **Development**: Use `llama3.2:3b` for faster responses
- **Production**: Use `llama3.2:latest` for better quality
- **Compatibility**: Use `llama3.1:latest` for maximum stability

#### System Resources
- **Minimum RAM**: 8GB (for 3b model)
- **Recommended RAM**: 16GB+ (for latest models)
- **Storage**: 10GB+ free space for models

#### Ollama Optimization
```bash
# Set memory limit (optional)
export OLLAMA_MAX_LOADED_MODELS=1

# Set number of threads (optional)
export OLLAMA_NUM_PARALLEL=4
```

## Verification

### Test Installation
```bash
# Quick test script
echo -e "2\n1\nversion\nexit" | ./build/bin/llamaware-agent

# Should show:
# - Llamaware banner
# - Mode selection (choosing offline)
# - Model selection (choosing 3b)
# - Version information
# - Clean exit
```

### Test Commands

#### Test Online Mode
```bash
# Start with online mode
./build/bin/llamaware-agent
# Select: Mode 1 (Online)
# Select: Provider 1 (Together AI) or 2 (Cerebras)

# Test commands:
> help                    # Show available commands  
> What is AI?             # Test AI chat functionality
> search:latest tech news # Test web search integration
> remember:using online   # Test memory system
```

#### Test Offline Mode  
```bash
# Start with offline mode
./build/bin/llamaware-agent
# Select: Mode 2 (Offline)
# Select: Model 1, 2, or 3

# Test commands:
> help                    # Show available commands
> Hello, how are you?     # Test local AI chat
> memory                  # Check memory system
> remember:using offline  # Test memory storage
```

## Next Steps

1. **Read Documentation**: Check [COMMANDS.md](COMMANDS.md) for usage
2. **Development**: See [DEVELOPMENT.md](DEVELOPMENT.md) for contributing
3. **Architecture**: Review [ARCHITECTURE.md](ARCHITECTURE.md) for internals

## Support

- **Issues**: Report bugs on GitHub Issues
- **Documentation**: Check `/docs` directory
- **Community**: Join discussions on GitHub Discussions