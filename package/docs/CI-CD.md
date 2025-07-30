# CI/CD Pipeline Documentation

## Overview

Llamaware Agent includes a comprehensive CI/CD pipeline that ensures code quality, security, and reliable deployments.

## Pipeline Structure

### Continuous Integration (CI)

**Triggers:**

* Push to `main` or `develop` branches
* Pull requests to `main`
* Release tags (`v*`)

**Jobs:**

1. **Build & Test** (Multi-platform)

   * Ubuntu Latest + macOS Latest
   * Debug + Release builds
   * Dependency installation
   * Preflight checks
   * Comprehensive testing

2. **Security Scan**

   * CodeQL static analysis
   * Vulnerability detection
   * Security best practices validation

3. **Code Quality**

   * Structure validation
   * Documentation checks
   * TODO/FIXME detection

### Continuous Deployment (CD)

**Release Pipeline:**

1. **Multi-platform Builds**

   * Linux x64 binary
   * macOS x64 binary
   * Automated testing

2. **Package Creation**

   * Distribution packages
   * Docker images
   * Release artifacts

3. **Deployment**

   * GitHub Releases
   * Docker Hub publishing
   * Production deployment

## Local Development Workflow

### Pre-commit Checks

```bash
# Quick validation
make preflight-quick

# Comprehensive checks
make preflight

# CI-style checks
./package/scripts/ci-preflight.sh
```

### Release Preparation

```bash
# Full preflight
make preflight

# Create packages
make package

# Build Docker image
make docker-build

# Run release script
./package/scripts/release.sh
```

## CI/CD Commands

### GitHub Actions Integration

```yaml
# In your workflow
- name: Run preflight checks
  run: ./package/scripts/ci-preflight.sh

- name: Build and test
  run: |
    cmake -S . -B build
    cmake --build build --target check
```

### Available Targets

* `make preflight` - Full preflight checks
* `make preflight-quick` - Essential checks only
* `cmake --build build --target check` - CMake testing
* `cmake --build build --target preflight` - CMake preflight

## Security Features

### Automated Security Scanning

* **CodeQL Analysis**: Static code analysis
* **Dependency Scanning**: Dependabot integration
* **Vulnerability Detection**: Security issue identification

### Security Checks Include:

* Direct system() call detection
* Input validation verification
* Memory safety analysis
* Dependency vulnerability scanning

## Deployment Environments

### Staging

* Automatic deployment on `develop` branch
* Integration testing
* Performance validation

### Production

* Manual approval required
* Release tag deployment
* Rollback capabilities
* Health monitoring

## Monitoring & Alerts

### Build Status

* GitHub Actions status badges
* Email notifications on failures
* Slack/Discord integration (configurable)

### Performance Metrics

* Build time tracking
* Test execution time
* Binary size monitoring
* Memory usage analysis

## Configuration

### Required Secrets

```
DOCKER_USERNAME     # Docker Hub username
DOCKER_PASSWORD     # Docker Hub password
GITHUB_TOKEN        # GitHub API token (auto-provided)
```

### Optional Secrets

```
SLACK_WEBHOOK       # Slack notifications
DISCORD_WEBHOOK     # Discord notifications
DEPLOY_KEY          # Production deployment key
```

## Troubleshooting

### Common Issues

**Build Failures:**

* Check dependency installation
* Verify CMake version compatibility
* Review compiler errors

**Test Failures:**

* Run local preflight checks
* Check environment variables
* Validate input/output handling

**Deployment Issues:**

* Verify secrets configuration
* Check deployment permissions
* Review artifact generation

### Debug Commands

```bash
# Local debugging
make preflight 2>&1 | tee preflight.log

# CI debugging
./package/scripts/ci-preflight.sh --verbose

# Build debugging
cmake -S . -B build --debug-output
```

## Best Practices

### Development

1. Run `make preflight-quick` before commits
2. Use feature branches for development
3. Write descriptive commit messages
4. Update documentation with changes

### Releases

1. Run full preflight checks
2. Update version numbers
3. Create comprehensive release notes
4. Test deployment in staging first

### Security

1. Regular dependency updates
2. Security scan reviews
3. Minimal privilege principles
4. Secure secret management

## Future Enhancements

### Planned Features

* Multi-architecture builds (ARM64)
* Performance benchmarking
* Automated changelog generation
* Integration testing suite
* Load testing capabilities

### Monitoring Improvements

* Real-time performance metrics
* User analytics integration
* Error tracking and reporting
* Automated rollback triggers
