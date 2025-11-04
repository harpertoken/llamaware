#!/bin/bash

# Read the entire commit message
msg=$(cat)

# Get the first line
first_line=$(echo "$msg" | head -n1)

# Rewrite first line: lowercase and truncate to 60 chars
first_line=$(echo "$first_line" | tr '[:upper:]' '[:lower:]')
first_line=${first_line:0:60}

# Output the rewritten first line
echo "$first_line"

# Output the rest of the message unchanged
echo "$msg" | tail -n +2
