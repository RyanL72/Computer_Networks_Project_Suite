#!/bin/bash

# Step 1: Build the project
echo "Building the project..."
if ! make; then
    echo "Error: Build failed."
    exit 1
fi
echo "Build completed successfully."

# Step 2: Set directories and input arguments
SAMPLES_DIR="Proj4Samples"
TRACE_FILE="$1"      # First argument: name of the trace file
MODE="$2"            # Second argument: mode (-i|-s|-t|-m)
EXPECTED_FILE="$SAMPLES_DIR/$3"   # Third argument: expected output file (located in Proj4Samples)
CURRENT_DIR=$(pwd)

if [[ -z "$TRACE_FILE" || -z "$MODE" || -z "$EXPECTED_FILE" ]]; then
    echo "Usage: $0 <trace_file> <-i|-s|-t|-m> <expected_output_file>"
    exit 1
fi

TRACE_PATH="$SAMPLES_DIR/$TRACE_FILE"
EXECUTABLE="./proj4"
DIFFERENCES_FILE="differences.out" # Output differences file

# Step 3: Ensure the trace file exists
if [[ ! -f "$TRACE_PATH" ]]; then
    echo "Error: Trace file '$TRACE_FILE' not found in $SAMPLES_DIR."
    exit 1
fi

# Ensure the expected output file exists
if [[ ! -f "$EXPECTED_FILE" ]]; then
    echo "Error: Expected output file '$3' not found in $SAMPLES_DIR."
    exit 1
fi

# Step 4: Move the trace file to the current directory
echo "Moving $TRACE_FILE to the current directory..."
mv "$TRACE_PATH" "$CURRENT_DIR"

# Step 5: Run the program with the specified mode and redirect output to `a.out`
echo "Running the program with $TRACE_FILE in mode $MODE..."
if ! $EXECUTABLE -r "$TRACE_FILE" "$MODE" > a.out; then
    echo "Error: Program execution failed."
    mv "$CURRENT_DIR/$TRACE_FILE" "$SAMPLES_DIR" # Move trace file back on failure
    exit 1
fi
echo "Execution completed. Output saved to a.out."

# Step 6: Compare output with expected output
echo "Comparing output with expected output..."
DIFF_OUTPUT=$(diff a.out "$EXPECTED_FILE") # Unified diff format
if [[ -z "$DIFF_OUTPUT" ]]; then
    echo "Output matches expected output." | tee "$DIFFERENCES_FILE"
else
    echo "Output differs from expected output. Showing differences:" | tee "$DIFFERENCES_FILE"
    echo "$DIFF_OUTPUT" | tee -a "$DIFFERENCES_FILE"
fi

# Step 7: Move the trace file back to Proj4Samples
echo "Moving $TRACE_FILE back to $SAMPLES_DIR..."
mv "$CURRENT_DIR/$TRACE_FILE" "$SAMPLES_DIR"

# Step 8: Cleanup Proj4Samples (optional cleanup logic here)
echo "Running cleanup_samples.sh..."
if [[ -x cleanup_samples.sh ]]; then
    ./cleanup_samples.sh
else
    echo "Warning: cleanup_samples.sh not found or not executable."
fi

echo "All steps completed successfully."

