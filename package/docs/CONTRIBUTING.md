# Contributing to Llamaware Agent

Thank you for your interest in contributing to Llamaware Agent! This document provides guidelines and information for contributors.

## Getting Started

### Development Setup

1. Fork the repository
2. Clone your fork: `git clone https://github.com/yourusername/Llamaware.git`
3. Set up the development environment: `make setup`
4. Install dependencies: `make install-deps-mac` or `make install-deps-ubuntu`
5. Build the project: `make build`

### Project Structure

```
src/
├── core/           # Main agent logic
├── services/       # Business logic modules
├── utils/          # Shared utilities
└── data/           # Data management
```

## Development Guidelines

### Code Style

* Follow modern C++17 standards
* Use meaningful variable and function names
* Add comments for complex logic
* Maintain consistent indentation (4 spaces)
* Use namespace organization

### Architecture Principles

* **Separation of Concerns**: Each module has a single responsibility
* **Dependency Injection**: Services are injected rather than hardcoded
* **Error Handling**: Comprehensive error management
* **Security First**: Always validate inputs and check for dangerous operations

### Adding New Features

#### 1. Services

New functionality should be added as services in `src/services/`:

```cpp
// include/services/new_service.h
namespace Services {
    class NewService {
    public:
        static std::string perform_action(const std::string& input);
    };
}
```

#### 2. Commands

Add new commands in the agent's command parser:

```cpp
else if (input.rfind("newcmd:", 0) == 0) {
    // Handle new command
}
```

#### 3. UI Elements

Use the UI utilities for consistent styling:

```cpp
Utils::UI::print_success("Operation completed");
Utils::UI::print_error("Something went wrong");
```

## Testing

### Running Tests

```bash
make test          # Quick functionality test
make build && ./build/bin/llamaware-agent  # Manual testing
```

### Test Coverage

* Test all new commands and features
* Verify error handling paths
* Test with both online and offline modes
* Validate security features

## Documentation

### Code Documentation

* Add header comments to new files
* Document public APIs
* Include usage examples for complex functions

### User Documentation

* Update README.md for new features
* Add examples to the usage section
* Update help text in the UI

## Security Considerations

### Command Safety

* All system commands must go through `CommandService`
* Add dangerous patterns to the safety check list
* Validate all file operations

### Input Validation

* Sanitize user inputs
* Check file paths for directory traversal
* Validate API responses

## Pull Request Process

1. **Create a Feature Branch**: `git checkout -b feature/your-feature-name`
2. **Make Changes**: Follow the coding guidelines
3. **Test Thoroughly**: Ensure all functionality works
4. **Update Documentation**: Add/update relevant docs
5. **Submit PR**: Include a clear description of changes

### PR Requirements

* [ ] Code follows project style guidelines
* [ ] All tests pass
* [ ] Documentation is updated
* [ ] Security considerations addressed
* [ ] No breaking changes (or clearly documented)

## Bug Reports

### Before Reporting

* Check existing issues
* Test with the latest version
* Gather system information

### Bug Report Template

```
**Environment:**
- OS: [e.g., macOS 14.0]
- Compiler: [e.g., clang 15.0]
- Mode: [Online/Offline]

**Steps to Reproduce:**
1. Step one
2. Step two
3. Step three

**Expected Behavior:**
What should happen

**Actual Behavior:**
What actually happens

**Additional Context:**
Any other relevant information
```

## Feature Requests

We welcome feature requests! Please:

* Check if the feature already exists
* Explain the use case
* Consider the impact on existing functionality
* Be open to discussion and iteration

## Getting Help

* **Issues**: Use GitHub issues for bugs and feature requests
* **Discussions**: Use GitHub discussions for questions
* **Documentation**: Check the README and code comments

## Recognition

Contributors will be recognized in:

* CHANGELOG.md for significant contributions
* README.md contributors section
* Release notes for major features

Thank you for helping make Llamaware Agent better!
