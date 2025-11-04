# Commands

16 features across 4 categories.

## Files

| Command | Function |
|---------|----------|
| `@file <path>` | Inject file |
| `@directory <path>` | Inject directory |
| `read:<file>` | Read file |
| `read:<file>:<start>:<count>` | Read range |
| `write:<file> <content>` | Write file |
| `replace:<file>:<old>:<new>` | Replace text |
| `grep:<pattern>[:dir[:filter]]` | Search files |
| `/context show` | View context |
| `/context refresh` | Refresh context |
| `/context create` | Create LLAMAWARE.md |
| `!` | Toggle shell |
| `cmd:<command>` | Execute command |

## Sessions

| Command | Function |
|---------|----------|
| `/save <name>` | Save session |
| `/resume <name>` | Resume session |
| `/sessions` | List sessions |
| `/chat save <tag>` | Save conversation |
| `/chat resume <tag>` | Resume conversation |
| `/chat list` | List conversations |
| `/tools` | Show tools |
| `/compress` | Compress context |
| `remember:<fact>` | Save fact |
| `memory` | Show memories |
| `clear` | Clear session |
| `forget` | Clear memory |

## Extensions

| Command | Function |
|---------|----------|
| `/mcp servers` | List servers |
| `/mcp resources <server>` | List resources |
| `/mcp tools <server>` | List tools |
| `/mcp prompts <server>` | List prompts |
| `/mcp add <name> <exe>` | Add server |
| `/mcp remove <name>` | Remove server |
| `/checkpoint <name>` | Create checkpoint |
| `/restore <name>` | Restore checkpoint |
| `/checkpoints` | List checkpoints |
| `/fetch <url> [format]` | Fetch content |
| `search:<query>` | Web search |

## Security

| Command | Function |
|---------|----------|
| `/theme list` | List themes |
| `/theme set <name>` | Set theme |
| `/theme preview <name>` | Preview theme |
| `/auth providers` | List providers |
| `/auth set <provider>` | Set provider |
| `/auth key <provider> <key>` | Set key |
| `/auth backup` | Backup credentials |
| `/auth restore <file>` | Restore credentials |
| `/sandbox run <command>` | Run sandboxed |
| `/sandbox status` | Check status |
| `/sandbox config <name>` | Configure sandbox |
| `/error report` | View errors |
| `/error recent <count>` | Recent errors |
| `/error export <file>` | Export log |

## System

| Command | Function |
|---------|----------|
| `help` | Show help |
| `version` | Show version |
| `quit` / `exit` | Exit |

## Examples

**Files:**
```bash
> @file src/main.cpp
> @directory src/
> read:config.json
> write:output.txt Hello
> replace:app.js:old:new
> grep:TODO:src:*.js
```

**Sessions:**
```bash
> /save project
> /resume project
> /compress
> remember:TypeScript strict
```

**Web:**
```bash
> /fetch https://api.github.com/user json
> search:React patterns
> /mcp servers
> /tools
```

**Advanced:**
```bash
> /checkpoint v1
> /restore v1
> /theme set dark
> /auth set together
> /sandbox run "npm test"
> !
```

**Memory:**
```bash
> /context show
> memory
> clear
> /error report
```

## Summary

16 features across 4 categories:
- **Files**: Injection, operations, context, shell
- **Sessions**: Management, tools, config, compression
- **Extensions**: MCP, checkpoints, web, filtering
- **Security**: Themes, auth, sandbox, errors

## Documentation

- [Setup](SETUP.md)
- [Architecture](ARCHITECTURE.md)
- [Development](DEVELOPMENT.md)
- [README](../README.md)
