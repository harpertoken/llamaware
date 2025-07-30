# Llamaware Agent - Distribution Package

This directory contains all packaging, distribution, and deployment configurations.

## Directory Structure

```
package/
├── binary/           # Binary distribution and installers
├── config/           # Configuration templates and examples
├── docker/           # Container images and orchestration
├── docs/             # Distribution documentation
├── managers/         # Package manager configurations
├── scripts/          # Build and release automation
└── dist/             # Generated distribution files
```

## Available Distribution Methods

### 1. Binary Distribution (`binary/`)
- Pre-compiled binaries for different platforms
- Installation scripts with dependency management
- Platform-specific runtime configurations

### 2. Container Distribution (`docker/`)
- Docker images for consistent deployment
- Docker Compose for multi-service setups
- Kubernetes manifests for orchestration

### 3. Package Managers (`managers/`)
- Homebrew formula (macOS)
- APT package (Ubuntu/Debian) 
- RPM package (RedHat/CentOS)
- Snap package (Universal Linux)

### 4. Configuration (`config/`)
- Environment configuration templates
- Default settings and examples
- Platform-specific configurations

### 5. Documentation (`docs/`)
- Changelog and release notes
- Contributing guidelines
- Distribution-specific documentation

### 6. Automation (`scripts/`)
- Build automation scripts
- Release management tools
- CI/CD pipeline configurations

## Quick Start

```bash
# Build and package
make package

# Install locally
make install

# Build Docker image
make docker-build

# Create release
./package/scripts/release.sh
```