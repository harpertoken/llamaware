# Llamaware

[![llamaware-agent](https://github.com/harpertoken/llamaware/actions/workflows/llamaware-agent.yml/badge.svg)](https://github.com/harpertoken/llamaware/actions/workflows/llamaware-agent.yml)

**Llamaware**  is a professional AI agent for development, built for **stability, extensibility, and security**. It provides core features for file operations, session handling, extensions, AI provider support, and automated GitHub repository management.

## System Requirements

**Supported Platforms**

* Linux (Ubuntu 20.04+)
* macOS (11.0+)
* Windows (MSVC 2019+)

**Compiler**

* C++20 compatible (GCC 10+, Clang 12+, MSVC 2019 16.11+)

**Build System**

* CMake 3.14+

**Version Control**

* Git

**Dependencies**

* cpr 1.10.0+
* nlohmann-json 3.10.0+
* OpenSSL 1.1.1+
* libpqxx 7.0+ (optional, for PostgreSQL database support)

## Project Structure

* **src/core/** – central agent logic, command routing
* **src/services/** – AI, file, web, git, MCP, and more
* **src/utils/** – configuration, UI, validation helpers
* **tests/** – unit and E2E tests
* **scripts/** – build & report utilities
* **docker/** – Docker and containerization files

## Documentation

See [docs/](docs/) for detailed documentation.

## Installation & Build

### Common Steps

```bash
git clone https://github.com/harpertoken/llamaware.git
cd llamaware
```

### Install Dependencies

**Ubuntu/Debian**

```bash
# Use the automated script
./scripts/install-deps.sh

# Or install manually:
sudo apt update
sudo apt install -y build-essential cmake git libcurl4-openssl-dev
# Optional: for PostgreSQL database support
sudo apt install -y libpqxx-dev
make install-deps-ubuntu
```

**macOS**

```bash
# Use the automated script
./scripts/install-deps.sh

# Or install manually:
brew install cmake
# Optional: for PostgreSQL database support
brew install libpqxx
make install-deps-mac
```

**Windows**

```powershell
# Install Chocolatey, CMake, and vcpkg (see CI workflow)
# Optional: for PostgreSQL database support
vcpkg install libpqxx
```

### Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
```

## Running the Agent

```bash
./build/bin/llamaware-agent
```

**Basic Commands**

```text
version
search:query
cmd:command
read:/path/to/file
write:/path/to/file content
help
exit | quit
```

**Example Session**

```bash
docker exec -it llamaware-agent /bin/bash
llamaware-agent

# Inside agent
version
cmd:ls -la
search:current time
exit
```

**Persistent Data**

* Stored in Docker volumes

```bash
docker volume ls
```

**Environment Variables**

* `.env` file in project root for API keys

## Supported AI Providers

* **Together AI** – Meta Llama models
* **Cerebras** – Llama inference
* **Fireworks** – High-performance models
* **Groq** – Speed optimized
* **DeepSeek** – Advanced reasoning models
* **OpenAI** – GPT models
* **Offline** – Ollama (llama3.2:3b, latest)

## Database Support

Llamaware includes optional PostgreSQL database integration for persistent memory storage.

**Features**
* Conversation history persistence
* Fact and preference storage
* Session state management
* Automatic schema initialization

**Configuration**
Set environment variables for database connection:
```bash
DB_HOST=localhost
DB_PORT=5432
DB_NAME=llamaware
DB_USER=llamaware
DB_PASSWORD=your_password
```

**Note**: Database support is automatically enabled if libpqxx is available at build time. Without it, memory is stored in local files.

## GitHub Bot

Llamaware includes automated repository management features.

**Core Features**

* Health checks (build/tests/quality)
* TODO detection and sub-issue creation
* Milestone assignment based on PR type

**Configuration**

* `.llamaware-agent.yml` in repository root

```yaml
name: llamaware-agent
version: 1.0.0
features:
  - health_checks
  - todo_management
  - milestone_assignment
permissions:
  contents: read
  pull-requests: write
  issues: write
```

**Workflow**

* Triggers on PRs, issues, and comments
* Posts formatted progress reports on PRs

## CI/CD

The project uses GitHub Actions for continuous integration and deployment.

**Workflow Triggers**
- Push to `main` and `develop` branches
- Pull requests to any branch
- Published releases

**Repository Variables**
- `PUSH_BRANCHES`: JSON array of branches for push triggers (e.g., `["main","develop"]`)
- `PR_BRANCHES`: JSON array of branches for PR triggers (e.g., `["*"]`)
- `RELEASE_TYPES`: JSON array of release types (e.g., `["published"]`)

**Note**: Due to GitHub Actions limitations, trigger branches are hardcoded in the workflow. Repository variables are used for job configuration. For dynamic triggers, monitor GitHub updates.

**Features**
- Cross-platform builds (Ubuntu, macOS, Windows)
- Preflight checks
- Artifact uploads
- Code coverage reporting

## Testing

**Unit Tests**

```bash
cmake -S . -B build
cmake --build build
cd build && ctest --output-on-failure
```

**End-to-End (E2E) Tests**

```bash
docker-compose -f docker-compose.e2e.yml up -d
docker-compose -f docker-compose.e2e.yml logs -f e2e-tests
docker-compose -f docker-compose.e2e.yml down
```

## Development

**Debug Build**

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

**Pre-commit Setup**

Set up code quality checks and linting:

```bash
./scripts/setup-pre-commit.sh
```

This installs:
- Pre-commit hooks for code formatting
- YAML linting with yamllint
- Conventional commit message validation

**Workflow**

* Use `help` inside the agent to explore commands
* Run `make full-check` for comprehensive validation (clean, build, test, preflights)
* Follow CI/CD guides for contributing
* Run `pre-commit run --all-files` to check all files

## Conventional Commits

This project enforces conventional commit standards for consistent and meaningful commit messages.

### Setup

To enable commit message enforcement, copy the hook to your local `.git/hooks/` directory:

```bash
cp scripts/commit-msg .git/hooks/commit-msg
chmod +x .git/hooks/commit-msg
```

### Usage

Commit messages must follow these rules:

- Start with a type: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`, `perf`, `ci`, `build`, `revert`
- Be entirely lowercase
- First line must be ≤60 characters

Example valid commit messages:

```
feat: add user authentication
fix: resolve memory leak in agent service
docs: update installation instructions
```

### History Cleanup

To rewrite existing commit messages in the repository history:

```bash
git filter-branch --msg-filter 'bash scripts/rewrite_msg.sh' -- --all
git push --force-with-lease origin main
```

**Warning**: This rewrites history and requires force-pushing. Coordinate with team members.

## Troubleshooting

```bash
# Verify container is running
docker ps

# View logs
docker logs llamaware-agent

# Restart agent
docker-compose -f package/docker/docker-compose.yml restart llamaware-agent
```

## Versioning

This project follows [Semantic Versioning](https://semver.org/) (SemVer):

- **MAJOR**: Incompatible API changes
- **MINOR**: Backward-compatible functionality additions
- **PATCH**: Backward-compatible bug fixes

**Version Sources (in priority order):**
1. Git tags (e.g., `v0.0.8`)
2. `VERSION` file in project root
3. Default fallback version

**Current Version:** Check with `./build/bin/llamaware-agent version`

**Release Process:**
```bash
# Bump version (patch/minor/major)
./scripts/bump-version.sh patch

# Commit and trigger release
git add . && git commit -m "feat: release v0.0.9 [release]"
git push origin main
```

**Auto-Release Triggers:**
- Commit message contains `[release]` - triggers auto-tagging
- Add `[major]` or `[minor]` for version bump type (default: patch)

## License

MIT License
