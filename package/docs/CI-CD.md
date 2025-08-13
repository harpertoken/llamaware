# CI/CD Pipeline

## Pipeline Structure

### Continuous Integration
**Triggers**: Push to main/develop, PRs, release tags

**Jobs**:
- Build & Test: Multi-platform (Ubuntu/macOS), Debug/Release
- Security Scan: CodeQL analysis, vulnerability detection
- Code Quality: Structure validation, documentation checks

### Continuous Deployment
**Release Pipeline**:
- Multi-platform builds (Linux/macOS x64)
- Package creation (Docker images, artifacts)
- Deployment (GitHub Releases, Docker Hub)

## Local Workflow

### Pre-commit
```bash
make preflight-quick    # Quick validation
make preflight          # Comprehensive checks
./package/scripts/ci-preflight.sh  # CI-style checks
```

### Release
```bash
make preflight && make package && make docker-build
./package/scripts/release.sh
```

## Commands

### GitHub Actions
```yaml
- run: ./package/scripts/ci-preflight.sh
- run: cmake -S . -B build && cmake --build build --target check
```

### Targets
- `make preflight` - Full checks
- `make preflight-quick` - Essential only
- `cmake --build build --target check` - CMake testing

## Security
- CodeQL static analysis
- Dependency scanning (Dependabot)
- Vulnerability detection
- Direct system() call detection
- Input validation verification
- Memory safety analysis

## Environments
- **Staging**: Auto-deploy on develop, integration testing
- **Production**: Manual approval, release tags, rollback support

## Configuration

### Required Secrets
```
DOCKER_USERNAME
DOCKER_PASSWORD
GITHUB_TOKEN
```

### Optional Secrets
```
SLACK_WEBHOOK
DISCORD_WEBHOOK
DEPLOY_KEY
```

## Troubleshooting
```bash
make preflight 2>&1 | tee preflight.log
./package/scripts/ci-preflight.sh --verbose
cmake -S . -B build --debug-output
```

## Best Practices
- Run `make preflight-quick` before commits
- Use feature branches
- Update documentation with changes
- Test staging before production
