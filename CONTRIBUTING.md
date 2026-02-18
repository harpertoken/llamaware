# Contributing

This project is currently maintained by collaborators. External contributions are not accepted at this time.

## For Collaborators

1. Fork the repository and create a feature branch
2. Follow Conventional Commits: `feat:`, `fix:`, `docs:`, `chore:`
3. Run `make preflight` before pushing
4. Create a PR for review

## Building

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
cmake --build build --target test
```
