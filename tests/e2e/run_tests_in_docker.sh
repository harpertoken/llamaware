#!/bin/bash
set -e
set -o pipefail

mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
make test
echo 'Running E2E tests...'
cd ../tests/e2e
./run_e2e_tests.sh
echo 'E2E tests completed'