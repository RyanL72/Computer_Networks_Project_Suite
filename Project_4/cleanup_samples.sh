#!/bin/bash

# Define paths
SAMPLES_DIR="Proj4Samples"
PROJECT_DIR="Project_4"
TAR_FILE="$SAMPLES_DIR/p4-all-samples-v7.tar.gz"

# Move the tar.gz file to Project_4 folder
cd "$SAMPLES_DIR" || exit
mv "$TAR_FILE" "$PROJECT_DIR"

# Change to the samples directory and delete everything except the tar.gz file
find . -mindepth 1 -not -name "p4-all-samples-v7.tar.gz" -exec rm -rf {} +

# Move the tar.gz file back to Proj4Samples
cd .. || exit
mv "$PROJECT_DIR/p4-all-samples-v7.tar.gz" "$SAMPLES_DIR"

echo "Cleanup completed."

