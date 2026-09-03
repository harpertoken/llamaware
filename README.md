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
