# Llamaware

[![CI/CD](https://github.com/harpertoken/llamaware/actions/workflows/ci.yml/badge.svg)](https://github.com/harpertoken/llamaware/actions/workflows/ci.yml)

**Llamaware** is a modular AI agent for development — designed for **stability**, **extensibility**, and **security**.
It provides core systems for file management, sessions, extensions, AI providers, and GitHub automation.

---

## System Requirements

**Supported Platforms**

* Linux (Ubuntu 20.04+)
* macOS (15.0+)
* Windows (MSVC 2019+)

**Compiler**

* C++20 (GCC 10+, Clang 12+, MSVC 2019 16.11+)

**Build System**

* CMake 3.14+

**Version Control**

* Git

**Dependencies**

* cpr ≥ 1.10.0
* nlohmann-json ≥ 3.10.0
* OpenSSL ≥ 1.1.1
* libpqxx ≥ 7.0 (optional, for PostgreSQL)

---

## Project Structure

```
src/core/         → agent logic, command routing
src/services/     → AI, file, web, git, MCP, etc.
src/utils/        → config, UI, validation helpers
tests/            → unit & E2E tests
scripts/          → build & reporting tools
package/docker/   → containerization setup
```

---

## Installation

```bash
git clone https://github.com/harpertoken/llamaware.git
cd llamaware
```

### Ubuntu/Debian

```bash
./scripts/install-deps.sh  # or run manually:
sudo apt update
sudo apt install -y build-essential cmake git libcurl4-openssl-dev libpqxx-dev
```

### macOS

```bash
./scripts/install-deps.sh  # or run manually:
brew install cmake libpqxx
```

### Windows

```powershell
# Install Chocolatey, CMake, and vcpkg (see CI workflow)
vcpkg install libpqxx
```

---

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
```

---

## Run

```bash
./build/bin/llamaware-agent
```

**Basic Commands**

```
version
search:query
cmd:command
read:/path/to/file
write:/path/to/file content
help
exit
```

---

## AI Providers

* Together AI (Llama models)
* Cerebras (Llama inference)
* Fireworks (performance-optimized)
* Groq (speed-focused)
* DeepSeek (reasoning models)
* OpenAI (GPT models)
* Ollama (offline support)

---

## Database Integration (Optional)

**Features**

* Persistent memory and session state
* Conversation history
* Automatic schema initialization

**Config**

```bash
DB_HOST=localhost
DB_PORT=5432
DB_NAME=llamaware
DB_USER=llamaware
DB_PASSWORD=your_password
```

Enabled automatically when `libpqxx` is detected at build time.

---

## GitHub Bot

**Features**

* Health checks (build/tests/quality)
* TODO tracking and sub-issue creation
* Automatic milestone tagging
```

---

## CI/CD

**Workflows**

* Build and test (Linux, macOS, Windows)
* Preflight checks
* Artifact uploads
* Code coverage

**Triggers**

* Push to `main` or `develop`
* Pull requests
* Release publication

**Repository Variables**

```
PUSH_BRANCHES   = ["main","develop"]
PR_BRANCHES     = ["*"]
RELEASE_TYPES   = ["published"]
```

---

## Testing

**Unit Tests**

```bash
cmake -S . -B build
cmake --build build
cd build && ctest --output-on-failure
```

**E2E Tests**

```bash
docker compose -f docker-compose.e2e.yml up -d
docker compose -f docker-compose.e2e.yml logs -f e2e-tests
docker compose -f docker-compose.e2e.yml down
```

---

## Development

**Debug Build**

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

**Code Quality**

```bash
./scripts/setup-pre-commit.sh
make clang-tidy
make lint-all
```

Run full checks:

```bash
make full-check
pre-commit run --all-files
```

---

## Conventional Commits

**Commit Format**

```
<type>: <description>
```

**Allowed Types**
`feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`, `perf`, `ci`, `build`, `revert`

**Examples**

```
feat: add user authentication
fix: resolve memory leak
docs: update installation guide
```

**Setup Hook**

```bash
cp scripts/commit-msg .git/hooks/
chmod +x .git/hooks/commit-msg
```

**Rewriting History**

```bash
git filter-branch --msg-filter 'bash scripts/rewrite_msg.sh' -- --all
git push --force-with-lease origin main
```

---

## Troubleshooting

```bash
docker ps                # check containers
docker logs llamaware-agent
docker-compose -f package/docker/docker-compose.yml restart llamaware-agent
```

---

## Versioning

Follows [Semantic Versioning](https://semver.org/):

| Type  | Description      |
| ----- | ---------------- |
| MAJOR | Breaking changes |
| MINOR | New features     |
| PATCH | Bug fixes        |

**Version Detection Order**

1. Git tag (e.g., `v0.0.8`)
2. `VERSION` file
3. Default fallback

**Release Process**

```bash
./scripts/bump-version.sh patch
git add . && git commit -m "feat: release v0.0.9 [release]"
git push origin main
```

Auto-releases trigger on `[release]`, with `[major]` or `[minor]` for version bump type.

---

## License

MIT License
