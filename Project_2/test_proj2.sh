#!/bin/bash

# Function to run a single test
run_test() {
    sample_number=$1
    command_to_run=$2
    expected_output_file=$3
    actual_output_file="actual_output_${sample_number}.txt"

    echo "Running Sample $sample_number..."
    
    # Run the command and save its output to a file
    eval "$command_to_run" > "$actual_output_file" 2>&1

    # Compare the actual output with the expected output
    if diff -q "$expected_output_file" "$actual_output_file" > /dev/null; then
        echo "Sample $sample_number - pass"
    else
        echo "Sample $sample_number - fail"
        echo "Differences:"
        diff --ignore-matching-lines="RSP: Date:" -u "$expected_output_file" "$actual_output_file" | grep '^[+-]' | sed 's/^/    /' | cat -vte
    fi
    echo
}

# Test cases (excluding commands with -r options)
run_test 1 "./proj2 -u http://www.icir.org -i -w icir.html" "sample-1-i.txt"
run_test 2 "./proj2 -q -u http://www.icir.org -w /dev/null" "sample-2-q.txt"
run_test 3 "./proj2 -u http://www.icir.org -w /dev/null -a" "sample-3-a.txt"
run_test 4 "./proj2 -u http://www.icir.org/mallman/test-files/small-test.dat -w sample-4.dat" "sample-4.dat"
run_test 5 "./proj2 -u http://dnmmt.com/ -i -w dnmmt.html" "sample-5-i.txt"
run_test 6 "./proj2 -q -u http://dnmmt.com/ -w dnmmt.html" "sample-6-q.txt"
run_test 7 "./proj2 -u http://dnmmt.com/ -w dnmmt.html -a" "sample-7-a.txt"
run_test 8 "./proj2 -w /dev/null -i -u http://www.icir.org/mallman/test-files/test.html" "sample-8-i.txt"
run_test 9 "./proj2 -w /dev/null -u http://www.icir.org/mallman/test-files/test.html -q" "sample-9-q.txt"
run_test 10 "./proj2 -a -w /dev/null -u http://www.icir.org/mallman/test-files/test.html" "sample-10-a.txt"
run_test 15 "./proj2 -a -u http://www.icir.org/mallman/test-files/big.dat -w big.dat" "sample-15-a.txt"
run_test 16 "./proj2 -a -u http://giga2025.com/ -w giga.out" "sample-16-a.txt"
run_test 20 "./proj2 -a -w net7.out -u http://net7.net" "sample-20-a.txt"

echo "Testing complete."
