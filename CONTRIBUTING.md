# Contributing to Llamaware

Thank you for your interest in contributing to **Llamaware**! This guide outlines how to set up your environment, follow coding standards, and submit high-quality contributions.

---

## Development Setup

### Prerequisites

* **C++20** compiler (GCC 10+, Clang 12+, MSVC 2019 16.11+)
* **CMake** 3.20+
* **Git** with pre-commit hook support

### Platform Dependencies

**Ubuntu/Debian**

```bash
sudo apt update
sudo apt install -y nlohmann-json3-dev cmake build-essential \
  libcurl4-openssl-dev libpqxx-dev expect clang-tidy
```

**macOS**

```bash
brew install cmake nlohmann-json cpr libpqxx expect llvm
```

**Windows**

```powershell
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
C:\vcpkg\vcpkg.exe install cpr nlohmann-json libpqxx --triplet x64-windows
```

### Building

```bash
# Configure
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Run tests
cmake --build build --target test

# Run preflight
make preflight  # Linux/macOS
cmake --build build --target preflight --config Release  # Windows
```

---

## Development Guidelines

### Code Standards

* Use **C++20** features appropriately
* Follow **Conventional Commits** (`feat:`, `fix:`, `docs:`, etc.)
* Apply **RAII** and modern C++ idioms
* Use **forward declarations** to reduce dependencies
* Prefer **meaningful identifiers** and clear comments
* Maintain **exception safety** and robust error handling

### Architecture

* Modular design with clear separation of concerns
* **Service-oriented** structure (`Services` namespace)
* Use **PIMPL** for ABI stability when needed
* Apply **dependency injection** for testability

### Commit Format

```
type(scope): description

feat(core): add new agent mode
fix(ci): resolve windows build issue
docs(readme): update installation guide
```

Keep subject lines under **60 characters**.

---

## Pull Request Process

### Before Submitting

1. Fork the repository and create a feature branch
2. Add or update tests for your changes
3. Run all pre-commit checks
4. Verify builds across platforms
5. Update documentation if applicable

### PR Checklist

* [ ] Code follows project style guidelines
* [ ] All tests pass locally
* [ ] Pre-commit hooks pass
* [ ] CI/CD pipeline passes (Linux/macOS/Windows)
* [ ] Documentation updated
* [ ] Security considerations reviewed

### Review Process

* PRs require maintainer review
* Address feedback promptly
* Keep PRs focused and scoped
* Rebase on `main` before merging

---

## Testing

**Unit Tests**

```bash
cmake --build build --target test
```

**E2E Tests**

```bash
docker compose -f docker-compose.e2e.yml up --abort-on-container-exit
```

**Coverage**

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target coverage
```

---

## Issue Reporting

### Bug Reports

* Use the bug report template
* Include OS and compiler details
* Provide minimal reproduction steps
* Attach logs or error outputs

### Feature Requests

* Use the feature request template
* Explain motivation and use case
* Consider complexity and alternatives

---

## Security

* Report vulnerabilities via **GitHub Security Advisories**
* Follow secure coding best practices
* Validate all inputs and handle errors gracefully
* Use static analysis tools (`clang-tidy` configured)

---

## Getting Help

* Check existing issues and discussions
* Review documentation and examples
* Ask focused, well-researched questions

---

## License

By contributing, you agree that your work is licensed under the same terms as the project.
