# Llamaware Agent Makefile
# Professional AI Agent with Command Execution

.PHONY: all build clean test test-e2e test-e2e-docker package install docker-build preflight preflight-quick preflight-full preflight-ci setup install-deps-mac install-deps-ubuntu help

# Default target
all: build

# Build the project
build:
	@echo "Building Llamaware Agent..."
	@mkdir -p build
	@cd build && cmake .. && make

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf build
	@rm -rf package/dist
	@echo "Clean complete."

# Run basic tests
test: build
	@echo "Running basic tests..."
	@printf "2\\nexit\\n" | ./build/bin/llamaware-agent

# Run E2E tests locally
test-e2e: build
	@echo "Running E2E tests locally..."
	@./tests/e2e/run_e2e_tests.sh

# Run E2E tests in Docker
test-e2e-docker:
	@echo "Running E2E tests in Docker..."
	@docker-compose -f docker-compose.e2e.yml up --build --abort-on-container-exit

# Create distribution package
package: build
	@echo "Creating distribution package..."
	@mkdir -p package/dist
	@cp build/bin/llamaware-agent package/dist/
	@cp README.md package/dist/
	@cp LICENSE package/dist/
	@cp .env.example package/dist/
	@echo "Package created in package/dist/"

# Install locally
install: build
	@echo "Installing Llamaware Agent locally..."
	@sudo cp build/bin/llamaware-agent /usr/local/bin/
	@echo "Installation complete. Run 'llamaware-agent' to start."

# Build Docker image
docker-build:
	@echo "Building Docker image..."
	@docker build -t llamaware/agent -f package/docker/Dockerfile .

# Run preflight checks (smart detection)
preflight:
	@echo "Running preflight checks..."
	@./package/scripts/preflight.sh

# Run quick preflight checks
preflight-quick:
	@echo "Running quick preflight checks..."
	@QUICK=true ./package/scripts/preflight.sh

# Run full preflight checks
preflight-full:
	@echo "Running full preflight checks..."
	@QUICK=false ./package/scripts/preflight.sh

# Run CI-style preflight checks
preflight-ci:
	@echo "Running CI-style preflight checks..."
	@CI=true ./package/scripts/preflight.sh

# Run CI locally (simulate GitHub Actions)
ci:
	@echo "Running local CI simulation..."
	@make clean && make build && make test && make preflight-ci

# Setup development environment
setup:
	@echo "Setting up development environment..."
	@./package/scripts/setup-git-hooks.sh

# Install dependencies on macOS
install-deps-mac:
	@echo "Installing dependencies on macOS..."
	@brew install cpr nlohmann-json cmake

# Install dependencies on Ubuntu
install-deps-ubuntu:
	@echo "Installing dependencies on Ubuntu..."
	@sudo apt update
	@sudo apt install -y nlohmann-json3-dev cmake build-essential git libcurl4-openssl-dev
	@rm -rf /tmp/cpr
	@git clone --depth=1 https://github.com/libcpr/cpr.git /tmp/cpr
	@cmake -S /tmp/cpr -B /tmp/cpr/build -DBUILD_CPR_TESTS=OFF -DCPR_USE_SYSTEM_CURL=ON -DCMAKE_BUILD_TYPE=Release
	@sudo cmake --build /tmp/cpr/build --target install
	@sudo ldconfig
	@rm -rf /tmp/cpr

# Show help
help:
	@echo "Llamaware Agent - Available Make Targets:"
	@echo ""
	@echo "Build Commands:"
	@echo "  build              Build the project"
	@echo "  clean              Clean build artifacts"
	@echo "  test               Run basic tests"
	@echo "  test-e2e           Run E2E tests locally"
	@echo "  test-e2e-docker    Run E2E tests in Docker"
	@echo "  package            Create distribution package"
	@echo ""
	@echo "Installation:"
	@echo "  install            Install locally to /usr/local/bin"
	@echo "  install-deps-mac   Install dependencies on macOS"
	@echo "  install-deps-ubuntu Install dependencies on Ubuntu"
	@echo ""
	@echo "Development:"
	@echo "  setup              Setup development environment"
	@echo "  preflight          Run preflight checks (smart)"
	@echo "  preflight-quick    Run quick preflight checks"
	@echo "  preflight-full     Run full preflight checks"
	@echo "  preflight-ci       Run CI-style preflight checks"
	@echo ""
	@echo "Docker:"
	@echo "  docker-build       Build Docker image"
	@echo ""
	@echo "Other:"
	@echo "  help               Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make build         # Build the project"
	@echo "  make test-e2e      # Run E2E tests"
	@echo "  make preflight     # Run preflight checks"
	@echo "  make package       # Create distribution"
	@echo "  make install       # Install locally"