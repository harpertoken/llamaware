<p align="center">
  <img src="https://raw.githubusercontent.com/harpertoken/llamaware/main/.github/assets/thumbnail.png" alt="llamaware" width="100%">
</p>

# llamaware

Code with AI help in C++. Small dev tools.

## Scope

- `include/core` - agent and mode
- `include/data` - memory
- `include/services` - ai, auth, git, github, file, sandbox, context, database, and more
- `CMakeLists.txt` and `Makefile` - build files

## Start

```bash
cmake -S . -B build
cmake --build build
```

Or:

```bash
make
```

## Test

See `.github/workflows/ci.yml` for current checks.

## Contribute

- Report: open an issue for bugs or requests.
- Change: small pull requests preferred.
