#!/bin/bash
set -e
set -o pipefail

# Print environment for debugging
echo "=== Environment ==="
printenv | sort
echo "=================="

# Enable core dumps for debugging segfaults
ulimit -c unlimited

# Set test data directory
export TEST_DATA_DIR="/tmp/llamaware_test_data"

# Clean up any previous builds
echo "=== Cleaning previous builds ==="
rm -rf /tmp/app
mkdir -p /tmp/app

# Copy only the necessary files using cp
echo "=== Copying application files ==="
cp -r /app/. /tmp/app/
rm -rf /tmp/app/build
rm -rf /tmp/app/.git

# Verify expect is installed
echo "=== Verifying test dependencies ==="
command -v expect >/dev/null 2>&1 || { echo >&2 "expect is required but not installed. Aborting."; exit 1; }
command -v python3 >/dev/null 2>&1 || { echo >&2 "python3 is required but not installed. Aborting."; exit 1; }

# Build the project
echo "=== Building the project ==="
cd /tmp/app
mkdir -p build
cd build
cmake ..
make -j1

# Set binary path for E2E tests
export BINARY_PATH="/usr/local/bin/llamaware-agent"

# Run unit tests
echo "=== Running unit tests ==="
ctest --output-on-failure

# Run E2E tests
echo "=== Running E2E tests ==="
cd ../tests/e2e
chmod +x ./run_e2e_tests.sh

# Create test data directory
mkdir -p "$TEST_DATA_DIR"

# Set environment variables for debugging
export EXPECT_DEBUG=1

# Run tests with a longer timeout and better error handling
set +e
start_time=$(date +%s)
timeout 600s ./run_e2e_tests.sh > >(tee -a e2e_test.log) 2>&1
result=$?
end_time=$(date +%s)
duration=$((end_time - start_time))
set -e

# Copy all test artifacts to the host
cp -r e2e_test.log "$TEST_DATA_DIR/"* /app/tests/e2e/ 2>/dev/null || true

# Check test results
if [ $result -eq 0 ]; then
    echo "=== E2E tests completed successfully in ${duration}s ==="
    echo "=== Test logs saved to /app/tests/e2e/e2e_test.log ==="
    touch /app/tests/e2e/.e2e_success
    exit 0
else
    echo "=== E2E tests failed after ${duration}s (exit code: $result) ==="
    echo "=== Last 50 lines of test output ==="
    tail -n 50 e2e_test.log || true
    echo "=== Expect debug logs ==="
    cat "$TEST_DATA_DIR/"expect_*.log 2>/dev/null || echo "No debug logs available"
    echo "========================="
    echo "=== Full test log available in /app/tests/e2e/e2e_test.log ==="
    exit 1
fi
