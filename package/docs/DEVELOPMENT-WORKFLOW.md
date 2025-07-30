# Development Workflow Guide

## Commands to Run When Making Changes

### Initial Setup (One Time)

```bash
# Setup development environment with Git hooks
make setup

# OR setup just the Git hooks
make setup-hooks
```

---

## Daily Development Workflow

### 1. While Coding (Frequent)

```bash
# Quick validation during development
make preflight-quick

# OR use smart detection (recommended)
make preflight
```

**When to use:** After making small changes, before taking a break

---

### 2. Before Committing

```bash
# Comprehensive check before commit
make preflight
```

**When to use:** Before `git commit` (also runs automatically via Git hook)

**What happens automatically:**

```bash
git commit -m "Your changes"
# → Triggers pre-commit hook
# → Runs quick preflight checks
# → Commits only if checks pass
```

---

### 3. Before Pushing

```bash
# Full validation before push
make preflight
```

**When to use:** Before `git push` (also runs automatically via Git hook)

**What happens automatically:**

```bash
git push origin feature-branch
# → Triggers pre-push hook  
# → Runs comprehensive preflight checks
# → Pushes only if checks pass
```

---

### 4. After Major Changes

```bash
# Clean rebuild and full validation
make clean
make preflight
```

**When to use:** After refactoring, adding dependencies, or structural changes

---

### 5. Before Creating PR

```bash
# Simulate CI environment
CI=true make preflight

# Create distribution package
make package

# Test Docker build (optional)
make docker-build
```

**When to use:** Before opening a pull request

---

## Command Reference

### Preflight Commands

| Command                | Speed  | Coverage     | Use Case            |
| ---------------------- | ------ | ------------ | ------------------- |
| `make preflight`       | Smart  | Auto-detects | Recommended default |
| `make preflight-quick` | Fast   | Essential    | During development  |
| `make preflight-full`  | Slow   | Complete     | Before releases     |
| `make preflight-ci`    | Medium | CI-optimized | Testing CI locally  |

### Build Commands

| Command        | Purpose               |
| -------------- | --------------------- |
| `make build`   | Build the project     |
| `make clean`   | Clean build artifacts |
| `make test`    | Run basic tests       |
| `make package` | Create distribution   |

### Development Commands

| Command            | Purpose                           |
| ------------------ | --------------------------------- |
| `make setup`       | Setup dev environment + Git hooks |
| `make setup-hooks` | Setup Git hooks only              |
| `make run`         | Build and run the agent           |

---

## Git Hooks (Automatic)

### Pre-commit Hook

* **Triggers:** `git commit`
* **Runs:** Quick preflight checks
* **Purpose:** Catch basic issues before commit

### Pre-push Hook

* **Triggers:** `git push`
* **Runs:** Comprehensive preflight checks
* **Purpose:** Ensure quality before sharing code

### Bypass Hooks (Use Carefully)

```bash
# Skip pre-commit checks
git commit --no-verify -m "Emergency fix"

# Skip pre-push checks  
git push --no-verify origin branch
```

---

## Error Scenarios & Solutions

### Build Fails

```bash
[FAIL] Build failed. Check build log:
```

**Solution:** Fix compilation errors, then run `make preflight`

### Tests Fail

```bash
[FAIL] Basic startup test failed
```

**Solution:** Debug runtime issues, test manually with `make run`

### Missing Files

```bash
[FAIL] Required file missing: README.md
```

**Solution:** Add missing files, then run `make preflight`

### Warnings

```bash
[WARN] TODO/FIXME comments found
```

**Solution:** Address TODOs or ignore if acceptable

---

## CI/CD Integration

### GitHub Actions (Automatic)

* PR Creation → Runs `CI=true make preflight`
* Push to main → Full CI pipeline
* Release tag → Build and publish

### Local CI Testing

```bash
# Test what CI will run
CI=true make preflight

# Test quick PR checks
CI=true QUICK=true make preflight
```

---

## Best Practices

### Do This

1. Run `make preflight` before every commit
2. Use Git hooks for automatic validation
3. Fix errors immediately, don't accumulate
4. Test CI locally before pushing
5. Keep commits small and focused

### Avoid This

1. Bypassing hooks without good reason
2. Committing broken code
3. Ignoring warnings long-term
4. Making large changes without testing
5. Skipping preflight checks

---

## Troubleshooting

### Hooks Not Working

```bash
# Reinstall hooks
make setup-hooks

# Check if hooks exist
ls -la .git/hooks/
```

### Preflight Too Slow

```bash
# Use quick mode during development
make preflight-quick

# Or set environment variable
QUICK=true make preflight
```

### CI Differences

```bash
# Test exact CI command locally
CI=true ./package/scripts/preflight.sh
```

---

## Summary: The ONE Command

For 99% of development work, just remember:

```bash
make preflight
```

This smart command will:

* Auto-detect your context (dev/CI/quick)
* Run appropriate checks
* Give clear feedback
* Ensure code quality