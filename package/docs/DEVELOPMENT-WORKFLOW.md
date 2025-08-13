# Development Workflow

## Setup (One Time)
```bash
make setup          # Dev environment + Git hooks
make setup-hooks    # Git hooks only
```

## Daily Workflow

### While Coding
```bash
make preflight-quick    # Quick validation
make preflight          # Smart detection (recommended)
```

### Before Committing
```bash
make preflight          # Comprehensive check
```
Auto-runs via Git hook: `git commit` → pre-commit hook → quick checks

### Before Pushing
```bash
make preflight          # Full validation
```
Auto-runs via Git hook: `git push` → pre-push hook → comprehensive checks

### After Major Changes
```bash
make clean && make preflight
```

### Before PR
```bash
CI=true make preflight
make package
make docker-build       # Optional
```

## Commands

### Preflight
| Command | Speed | Coverage | Use Case |
|---------|-------|----------|----------|
| `make preflight` | Smart | Auto-detects | Default |
| `make preflight-quick` | Fast | Essential | Development |
| `make preflight-full` | Slow | Complete | Releases |
| `make preflight-ci` | Medium | CI-optimized | Local CI test |

### Build
| Command | Purpose |
|---------|---------|
| `make build` | Build project |
| `make clean` | Clean artifacts |
| `make test` | Basic tests |
| `make package` | Distribution |

### Development
| Command | Purpose |
|---------|---------|
| `make setup` | Dev environment + hooks |
| `make setup-hooks` | Git hooks only |
| `make run` | Build and run |

## Git Hooks
- **Pre-commit**: `git commit` → quick checks
- **Pre-push**: `git push` → comprehensive checks

### Bypass (Use Carefully)
```bash
git commit --no-verify -m "Emergency fix"
git push --no-verify origin branch
```

## Troubleshooting

### Build Fails
Fix compilation errors, run `make preflight`

### Tests Fail
Debug runtime issues, test with `make run`

### Missing Files
Add missing files, run `make preflight`

### Hooks Not Working
```bash
make setup-hooks
ls -la .git/hooks/
```

### Preflight Too Slow
```bash
make preflight-quick
QUICK=true make preflight
```

## CI Integration
- PR Creation → `CI=true make preflight`
- Push to main → Full CI pipeline
- Release tag → Build and publish

### Local CI Testing
```bash
CI=true make preflight
CI=true QUICK=true make preflight
```

## Best Practices
**Do:** Run `make preflight` before commits, use Git hooks, fix errors immediately, test CI locally, keep commits small

**Avoid:** Bypassing hooks, committing broken code, ignoring warnings, large untested changes

## The ONE Command
```bash
make preflight
```
Auto-detects context, runs appropriate checks, ensures code quality