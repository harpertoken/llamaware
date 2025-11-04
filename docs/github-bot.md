# GitHub Bot Documentation

## Overview

Llamaware includes an automated GitHub bot that enhances repository management through intelligent automation and progress reporting.

## Features

### Health Checks
- Automated build verification on pull requests
- Quality assurance with TODO/FIXME detection
- Comprehensive test validation
- ASCII progress reporting

### TODO Management
- Automatic detection of TODO items in issue descriptions
- Creation of sub-issues for task tracking
- Organized project management

### Milestone Assignment
- Intelligent milestone assignment based on PR content
- Keyword-based categorization (feature, bug, refactor)
- Automated project organization

## Configuration

### Bot Configuration File

Create `.llamaware-agent.yml` in your repository root:

```yaml
name: llamaware-agent
description: automated health checks and issue management
version: 1.0.0

features:
  - health_checks
  - todo_management
  - milestone_assignment

permissions:
  contents: read
  pull-requests: write
  issues: write

triggers:
  push_branches: [main, develop, master]
  pull_request_branches: [main]
  release_types: [published]
```

### Workflow Setup

The bot uses GitHub Actions workflow (`.github/workflows/llamaware-agent.yml`) with the following triggers:
- Pull request events (opened, synchronize, reopened)
- Issue events (opened, edited)
- Issue comment events (created)

## Progress Reports

The bot generates consistent ASCII progress reports:

```
## Llamaware Agent

**Status**
██████████ 100% (Operational)

**Build**
██████████ 100% (Clean)

**Tests**
██████████ 100% (Passing)

**Quality**
██████████ 100% (Clean)

**Checks**
██████████ 100% (Passed)
```

## Commands

The agent supports GitHub-related commands:

- `github:repo:owner/repo` - Get repository information
- `github:issues:owner/repo` - List open issues
- `github:health:owner/repo` - Run health check

## Local Development

## API Integration

The bot integrates with GitHub API for:
- Repository information retrieval
- Issue and PR management
- Milestone and label operations
- Webhook management

## Security

- Uses GitHub App permissions with minimal required access
- Token-based authentication
- Secure API communication

## Troubleshooting

### Bot Not Responding
- Check repository permissions
- Verify `.llamaware-agent.yml` configuration
- Ensure workflow files are present

### Progress Reports Not Showing
- Confirm CI/CD pipeline completion
- Check workflow run logs
- Verify ASCII report generation

### TODO Management Issues
- Ensure issues contain "TODO:" prefix
- Check bot permissions for issue creation
- Verify workflow triggers

## Contributing

When contributing to the bot:
1. Update configuration schema if needed
2. Test workflow triggers
3. Verify progress report generation
4. Update this documentation
