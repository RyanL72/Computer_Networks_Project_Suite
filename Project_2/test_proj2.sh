#!/bin/bash

# Function to run a single test
run_test() {
    test_description=$1
    command_to_run=$2
    expected_output=$3
    actual_output=$4

    echo "Running test: $test_description"
    eval "$command_to_run" > "$actual_output" 2>&1

    # Check if there is an expected output file to compare
    if [[ -n "$expected_output" ]]; then
        echo "Comparing outputs for $test_description:"
        diff -u "$expected_output" "$actual_output" | cat -v | tail -n +1 | sed 's/^/    /'
        
        if diff -q "$expected_output" "$actual_output" > /dev/null; then
            echo "Test Passed: $test_description"
        else
            echo "Test Failed: $test_description"
            echo "Expected output:"
            cat -v "$expected_output" | sed 's/^/    /'
            echo "Actual output:"
            cat -v "$actual_output" | sed 's/^/    /'
            echo "Differences (line by line):"
            diff -u "$expected_output" "$actual_output" | cat -v | sed 's/^/    /'
        fi
    fi
    echo
}

# Test cases
run_test "Sample 1" "./proj2 -u http://www.icir.org -i -w icir.html" "sample-1-i.txt" "actual_output_1.txt"
run_test "Sample 2" "./proj2 -q -u http://www.icir.org -w /dev/null" "sample-2-q.txt" "actual_output_2.txt"
run_test "Sample 3" "./proj2 -u http://www.icir.org -w /dev/null -a" "sample-3-a.txt" "actual_output_3.txt"
run_test "Sample 4" "./proj2 -u http://www.icir.org/mallman/test-files/small-test.dat -w sample-4.dat" "" ""
run_test "Sample 5" "./proj2 -u http://dnmmt.com/ -i -w dnmmt.html" "sample-5-i.txt" "actual_output_5.txt"
run_test "Sample 6" "./proj2 -q -u http://dnmmt.com/ -w dnmmt.html" "sample-6-q.txt" "actual_output_6.txt"
run_test "Sample 7" "./proj2 -u http://dnmmt.com/ -w dnmmt.html -a" "sample-7-a.txt" "actual_output_7.txt"
run_test "Sample 8" "./proj2 -w /dev/null -i -u http://www.icir.org/mallman/test-files/test.html" "sample-8-i.txt" "actual_output_8.txt"
run_test "Sample 9" "./proj2 -w /dev/null -u htTp://www.icir.org/mallman/test-files/test.html -q" "sample-9-q.txt" "actual_output_9.txt"
run_test "Sample 10" "./proj2 -a -w /dev/null -u HTTP://www.icir.org/mallman/test-files/test.html" "sample-10-a.txt" "actual_output_10.txt"

# End of script
