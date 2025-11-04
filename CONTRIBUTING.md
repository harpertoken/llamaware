# Contributing

## Setup
1. Fork and clone repo
2. Install deps: `make install-deps-ubuntu` or mac
3. Build: `cmake -S . -B build && make`

## Guidelines
- C++17, meaningful names, comments
- Separation of concerns, error handling
- Test coverage for new features

## PR Requirements
- Code style followed
- Tests pass (run `make test` and `make test-e2e-docker`)
- CI/CD passes on all platforms
- Docs updated
- Security checked

## Issues
- Use templates for bugs/features
- Provide clear reproduction steps
