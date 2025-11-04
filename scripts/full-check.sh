#!/bin/bash

# Full check script: clean, build, test, preflight-quick, preflight

set -e  # Exit on any error

echo "Starting full check..."

echo "Step 1: Cleaning..."
make clean

echo "Step 2: Building..."
make build

echo "Step 3: Testing..."
make test

echo "Step 4: Quick preflight..."
make preflight-quick

echo "Step 5: Full preflight..."
make preflight

echo "Full check completed successfully!"
