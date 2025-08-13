# Contributing

## Setup
1. Fork repository
2. Clone: `git clone https://github.com/yourusername/Llamaware.git`
3. Setup: `make setup`
4. Install deps: `make install-deps-mac` or `make install-deps-ubuntu`
5. Build: `make build`

## Structure
```
src/
├── core/           # Agent logic
├── services/       # Business modules
├── utils/          # Utilities
└── data/           # Data management
```

## Guidelines

### Code Style
- C++17 standards
- Meaningful names
- Comments for complex logic
- 4-space indentation
- Namespace organization

### Architecture
- Separation of concerns
- Dependency injection
- Comprehensive error handling
- Security-first validation

### Adding Features

**Services** (`src/services/`):
```cpp
namespace Services {
    class NewService {
    public:
        static std::string perform_action(const std::string& input);
    };
}
```

**Commands**:
```cpp
else if (input.rfind("newcmd:", 0) == 0) {
    // Handle new command
}
```

**UI**:
```cpp
Utils::UI::print_success("Operation completed");
Utils::UI::print_error("Something went wrong");
```

## Testing
```bash
make test          # Quick test
make build && ./build/bin/llamaware-agent  # Manual test
```

Test coverage: new commands, error handling, online/offline modes, security features

## Security
- Commands through `CommandService`
- Add dangerous patterns to safety checks
- Validate file operations
- Sanitize inputs
- Check path traversal
- Validate API responses

## Pull Requests
1. Create feature branch: `git checkout -b feature/name`
2. Make changes following guidelines
3. Test thoroughly
4. Update documentation
5. Submit PR with clear description

### Requirements
- Code follows style guidelines
- All tests pass
- Documentation updated
- Security addressed
- No breaking changes (or documented)

## Bug Reports
**Template:**
```
Environment: OS, Compiler, Mode
Steps to Reproduce: 1. 2. 3.
Expected Behavior: What should happen
Actual Behavior: What happens
Additional Context: Other info
```

## Feature Requests
- Check existing features
- Explain use case
- Consider impact
- Open to discussion

## Help
- Issues: GitHub issues
- Questions: GitHub discussions
- Documentation: README and code comments
