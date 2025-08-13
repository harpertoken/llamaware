import React from 'react';
import { motion } from 'framer-motion';
import { Terminal as TerminalIcon, Copy } from 'lucide-react';

const Terminal = () => {


  const terminalSessions = [
    {
      title: "Agent Startup",
      commands: [
        {
          input: "./build/bin/llamaware-agent",
          output: `
██      ██       █████  ███    ███  █████  ██     ██  █████  ██████  ███████ 
██      ██      ██   ██ ████  ████ ██   ██ ██     ██ ██   ██ ██   ██ ██      
██      ██      ███████ ██ ████ ██ ███████ ██  █  ██ ███████ ██████  █████   
██      ██      ██   ██ ██  ██  ██ ██   ██ ██ ███ ██ ██   ██ ██   ██ ██      
███████ ███████ ██   ██ ██      ██ ██   ██  ███ ███  ██   ██ ██   ██ ███████ 
Llamaware
----------------------------------------

Enterprise Platform Ready
✓ 16 features loaded | linux | macos | windows
✓ Docker sandboxing available
✓ MCP server support enabled

Type 'help' for commands or 'quit' to exit`
        },
        {
          input: "help",
          output: `Commands
----------------------------------------
Files
  @file <path>             - Inject file
  @directory <path>        - Inject directory
  read:<file>              - Read file
  read:<file>:<start>:<count> - Read range
  write:<file> <content>   - Write file
  replace:<file>:<old>:<new> - Replace text
  grep:<pattern>[:dir[:filter]] - Search text

Sessions
  /save <name> [tags]       - Save session
  /resume <name>           - Resume session
  /sessions               - List sessions
  /compress               - Compress context
  remember:<fact>          - Remember fact
  memory                  - Show memory
  clear                   - Clear session
  forget                  - Clear memory

Web
  /fetch <url> [format]     - Fetch content
  search:<query>            - Search web
  /mcp servers             - List servers
  /mcp resources <server>   - List resources
  /mcp tools <server>       - List tools

Checkpoints
  /checkpoint <name>        - Create checkpoint
  /restore <name>          - Restore checkpoint
  /checkpoints             - List checkpoints

Themes
  /theme list              - List themes
  /theme set <name>         - Set theme
  /theme preview <name>     - Preview theme

Security
  /auth providers           - List providers
  /auth set <provider>      - Set provider
  /auth key <provider> <key> - Set key
  /sandbox run <command>    - Run sandboxed
  /sandbox status           - Check status

Errors
  /error report             - View errors
  /error recent <count>     - Show recent
  /error export <file>      - Export log

System
  cmd:<command>             - Run command
  /tools                   - Show tools
  !                        - Toggle shell
  help                     - Show help
  quit                     - Exit

Notes
Set theme with /theme set dark
Configure auth with /auth key
Create checkpoints before changes
Use /sandbox for safe execution
----------------------------------------`
        }
      ]
    },
    {
      title: "Status & Features",
      commands: [
        {
          input: "/status",
          output: `Status
----------------------------------------
Files (4/4)
   File injection
   Multi-file operations
   Context hierarchy
   Shell integration

Sessions (4/4)
   Session management
   Tool registry
   Configuration
   Context compression

Extensions (4/4)
   MCP servers
   Checkpoints
   Web fetch
   File filtering

Security (4/4)
   Themes
   Authentication
   Sandboxing
   Error handling

16 features active
----------------------------------------`
        }
      ]
    },
    {
      title: "Example Usage",
      commands: [
        {
          input: "cmd:ls -la",
          output: `[Executing]: ls -la
[Command Result]
total 48
drwxr-xr-x  12 user  staff   384 Jan 28 10:30 .
drwxr-xr-x   3 user  staff    96 Jan 28 10:25 ..
-rw-r--r--   1 user  staff   123 Jan 28 10:30 .env.example
-rw-r--r--   1 user  staff  1234 Jan 28 10:30 CMakeLists.txt
-rw-r--r--   1 user  staff   567 Jan 28 10:30 LICENSE
-rw-r--r--   1 user  staff  2345 Jan 28 10:30 Makefile
-rw-r--r--   1 user  staff  3456 Jan 28 10:30 README.md
drwxr-xr-x   4 user  staff   128 Jan 28 10:30 build
drwxr-xr-x   3 user  staff    96 Jan 28 10:30 data
drwxr-xr-x   5 user  staff   160 Jan 28 10:30 include
drwxr-xr-x   8 user  staff   256 Jan 28 10:30 package
drwxr-xr-x   6 user  staff   192 Jan 28 10:30 src`
        },
        {
          input: "write:test.txt Hello from Llamaware Agent!",
          output: `[Write Result]
File 'test.txt' written successfully (27 bytes)`
        },
        {
          input: "read:test.txt",
          output: `[File Content]
Hello from Llamaware Agent!`
        },
        {
          input: "/theme set dark",
          output: `Theme set to 'dark'`
        },
        {
          input: "/mcp servers",
          output: `[MCP] Available Servers:
- local (http://localhost:8000)
  Tools: web.fetch, files.list, sandbox.run`
        }
      ]
    }
  ];

  const copyToClipboard = (text) => {
    navigator.clipboard.writeText(text);
  };

  return (
    <section id="terminal" className="section">
      <div className="container">
        <motion.div
          initial={{ opacity: 0, y: 50 }}
          whileInView={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.8 }}
          viewport={{ once: true }}
        >
          <h2 className="section-title">See It In Action</h2>
          <p className="section-subtitle">
            Experience the power and elegance of the Llamaware Agent terminal interface
          </p>
        </motion.div>

        <div className="terminal-showcase">
          {terminalSessions.map((session, sessionIndex) => (
            <motion.div
              key={sessionIndex}
              className="terminal-window"
              initial={{ opacity: 0, y: 50 }}
              whileInView={{ opacity: 1, y: 0 }}
              transition={{ duration: 0.8, delay: sessionIndex * 0.2 }}
              viewport={{ once: true }}
            >
              <div className="terminal-header">
                <div className="terminal-controls">
                  <div className="control-button close"></div>
                  <div className="control-button minimize"></div>
                  <div className="control-button maximize"></div>
                </div>
                <div className="terminal-title">
                  <TerminalIcon size={16} />
                  {session.title}
                </div>
                <div className="terminal-actions">
                  <button 
                    className="action-button"
                    onClick={() => copyToClipboard(session.commands.map(cmd => `$ ${cmd.input}\n${cmd.output}`).join('\n\n'))}
                  >
                    <Copy size={16} />
                  </button>
                </div>
              </div>
              
              <div className="terminal-content">
                {session.commands.map((command, cmdIndex) => (
                  <div key={cmdIndex} className="command-block">
                    <div className="command-input">
                      <span className="prompt">$</span>
                      <span className="command">{command.input}</span>
                    </div>
                    <div className="command-output">
                      <pre>{command.output}</pre>
                    </div>
                  </div>
                ))}
              </div>
            </motion.div>
          ))}
        </div>

        <motion.div
          className="terminal-features"
          initial={{ opacity: 0, y: 50 }}
          whileInView={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.8, delay: 0.6 }}
          viewport={{ once: true }}
        >
          <h3 className="features-title">Terminal Features</h3>
          <div className="features-grid">
            <div className="feature-item">
              <div className="feature-icon">🎨</div>
              <div className="feature-text">
                <h4>Rich UI</h4>
                <p>Beautiful ASCII art, colors, and animations</p>
              </div>
            </div>
            <div className="feature-item">
              <div className="feature-icon">⚡</div>
              <div className="feature-text">
                <h4>Fast Response</h4>
                <p>Optimized performance with instant feedback</p>
              </div>
            </div>
            <div className="feature-item">
              <div className="feature-icon">🔒</div>
              <div className="feature-text">
                <h4>Safe Execution</h4>
                <p>Built-in security checks for all commands</p>
              </div>
            </div>
            <div className="feature-item">
              <div className="feature-icon">📝</div>
              <div className="feature-text">
                <h4>Smart Help</h4>
                <p>Contextual help and command suggestions</p>
              </div>
            </div>
          </div>
        </motion.div>
      </div>

      <style jsx>{`
        .terminal-showcase {
          display: grid;
          gap: 40px;
          margin-top: 60px;
        }

        .terminal-window {
          background: #1a1a1a;
          border: 1px solid #333;
          border-radius: 12px;
          overflow: hidden;
          box-shadow: 0 20px 40px rgba(0, 0, 0, 0.5);
        }

        .terminal-header {
          background: #2a2a2a;
          padding: 12px 20px;
          display: flex;
          align-items: center;
          justify-content: space-between;
          border-bottom: 1px solid #333;
        }

        .terminal-controls {
          display: flex;
          gap: 8px;
        }

        .control-button {
          width: 12px;
          height: 12px;
          border-radius: 50%;
        }

        .control-button.close {
          background: #ff5f57;
        }

        .control-button.minimize {
          background: #ffbd2e;
        }

        .control-button.maximize {
          background: #28ca42;
        }

        .terminal-title {
          display: flex;
          align-items: center;
          gap: 8px;
          color: #ccc;
          font-size: 14px;
          font-weight: 500;
        }

        .terminal-actions {
          display: flex;
          gap: 8px;
        }

        .action-button {
          background: none;
          border: none;
          color: #888;
          cursor: pointer;
          padding: 4px;
          border-radius: 4px;
          transition: all 0.2s ease;
        }

        .action-button:hover {
          color: #00d4ff;
          background: rgba(0, 212, 255, 0.1);
        }

        .terminal-content {
          padding: 20px;
          font-family: 'JetBrains Mono', monospace;
          font-size: 13px;
          line-height: 1.5;
          max-height: 500px;
          overflow-y: auto;
        }

        .command-block {
          margin-bottom: 20px;
        }

        .command-input {
          display: flex;
          align-items: center;
          gap: 8px;
          margin-bottom: 8px;
        }

        .prompt {
          color: #00d4ff;
          font-weight: bold;
        }

        .command {
          color: #e6e6e6;
        }

        .command-output {
          color: #ccc;
          margin-left: 16px;
        }

        .command-output pre {
          margin: 0;
          white-space: pre-wrap;
          word-wrap: break-word;
        }

        .terminal-features {
          margin-top: 60px;
          text-align: center;
        }

        .features-title {
          font-family: 'Helvetica Neue', Helvetica, Arial, sans-serif;
          font-size: 2rem;
          font-weight: bold;
          color: white;
          margin-bottom: 40px;
        }

        .features-grid {
          display: grid;
          grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
          gap: 30px;
        }

        .feature-item {
          display: flex;
          align-items: center;
          gap: 15px;
          padding: 20px;
          background: rgba(255, 255, 255, 0.05);
          border: 1px solid rgba(255, 255, 255, 0.1);
          border-radius: 12px;
          backdrop-filter: blur(10px);
        }

        .feature-icon {
          font-size: 2rem;
        }

        .feature-text h4 {
          font-family: 'Helvetica Neue', Helvetica, Arial, sans-serif;
          color: white;
          font-size: 1.1rem;
          font-weight: bold;
          margin-bottom: 5px;
        }

        .feature-text p {
          color: #ccc;
          font-size: 0.9rem;
        }

        @media (max-width: 768px) {
          .terminal-header {
            padding: 10px 15px;
          }

          .terminal-content {
            padding: 15px;
            font-size: 12px;
          }

          .features-grid {
            grid-template-columns: 1fr;
            gap: 20px;
          }

          .feature-item {
            padding: 15px;
          }
        }
      `}</style>
    </section>
  );
};

export default Terminal;