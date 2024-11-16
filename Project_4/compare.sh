#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Usage: $0 file1 file2"
    exit 1
fi

file1=$1
file2=$2

# Sort the files
sort "$file1" > sorted1
sort "$file2" > sorted2

# Compare sorted files
echo "Comparing sorted files..."
diff sorted1 sorted2 > differences.out

if [[ $? -eq 0 ]]; then
    echo "Files contain the same lines."
else
    echo "Differences found:"
    cat differences.out
fi

# Clean up sorted files
rm sorted1 sorted2
