# Changelog

All notable changes to Llamaware Agent will be documented in this file.

## \[2.0.0] - 2025-07-28

### Major Release - Professional Edition

#### Added

* **Modular Architecture**: Complete restructure into professional modules

  * `core/`: Main agent logic and orchestration
  * `services/`: Business logic for AI, commands, files, and web search
  * `utils/`: Shared utilities for UI, configuration, and versioning
  * `data/`: Memory and data management
* **Enhanced Command System**: Safe command execution with built-in security
* **Professional UI**: Improved terminal interface with status indicators
* **File Operations**: Comprehensive file read/write capabilities
* **Version Management**: Built-in version tracking and display
* **Build System**: Professional CMake configuration and Makefile
* **Documentation**: Comprehensive README and project documentation

#### Enhanced

* **Memory System**: Advanced context management with size limits
* **Error Handling**: Comprehensive error management and user feedback
* **Security**: Built-in dangerous command detection
* **Performance**: Optimized build configuration

#### Technical Improvements

* Modern C++17 standards and practices
* Namespace organization for better code structure
* Comprehensive error handling and logging
* Professional build and installation system

### Breaking Changes

* Complete API restructure (not backward compatible with v1.x)
* New command syntax and structure
* Updated configuration format

---

## \[1.0.0] - 2025-01-XX

### Initial Release

* Basic chat functionality with Llama models
* Online (Together API) and offline (Ollama) support
* Simple web search integration
* Basic memory system
* Monolithic architecture
