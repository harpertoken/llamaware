# Distribution Package

Packaging, distribution, and deployment configurations.

## Structure
```
package/
├── binary/           # Binary distribution
├── docker/           # Container images
├── docs/             # Distribution docs
├── managers/         # Package managers
├── scripts/          # Build automation
└── dist/             # Generated files
```

## Distribution Methods
- Binary: Pre-compiled binaries with installers
- Container: Docker images and orchestration
- Package Managers: Homebrew, APT, RPM, Snap
- Configuration: Templates and examples
- Documentation: Changelog and guides
- Automation: Build and release scripts

## Commands
```bash
make package          # Build and package
make install          # Install locally
make docker-build     # Build Docker image
./package/scripts/release.sh  # Create release
```