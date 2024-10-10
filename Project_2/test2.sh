#!/usr/bin/bash

# test 11
echo "Running Test 11: HTML file comparison"
./proj2 -u http://www.icir.org/mallman/test-files/test.html -w my_test.html
wget -q -O wget_test.html http://www.icir.org/mallman/test-files/test.html
cmp my_test.html wget_test.html
diff my_test.html wget_test.html

# test 12
echo "Running Test 12: GIF file comparison"
./proj2 -u http://www.icir.org/mallman/test-files/ICSI_color.gif -w icsi.gif
wget -q -O wget_icsi.gif http://www.icir.org/mallman/test-files/ICSI_color.gif
cmp icsi.gif wget_icsi.gif
diff icsi.gif wget_icsi.gif

# test 13
echo "Running Test 13: Medium text file comparison"
./proj2 -u http://www.icir.org/mallman/test-files/medium.txt -w medium.txt
wget -q -O wget_medium.txt http://www.icir.org/mallman/test-files/medium.txt
cmp medium.txt wget_medium.txt
diff medium.txt wget_medium.txt

# test 14
echo "Running Test 14: Medium binary file comparison"
./proj2 -u http://www.icir.org/mallman/test-files/medium.dat -w medium.dat
wget -q -O wget_medium.dat http://www.icir.org/mallman/test-files/medium.dat
cmp medium.dat wget_medium.dat
diff medium.dat wget_medium.dat

# test 15
echo "Running Test 15: Large binary file comparison"
./proj2 -a -u http://www.icir.org/mallman/test-files/big.dat -w big.dat > output.txt
wget -q -O wget_big.dat http://www.icir.org/mallman/test-files/big.dat
cmp big.dat wget_big.dat
diff big.dat wget_big.dat
grep -v '^RSP: Date: ' output.txt > cleaned.txt
grep -v '^RSP: Date: ' sample-15-a.txt > a_cleaned.txt
cmp cleaned.txt a_cleaned.txt
diff cleaned.txt a_cleaned.txt

# test 16
echo "Running Test 16: Web page comparison"
./proj2 -a -u http://giga2025.com/ -w giga.out > output.txt
wget -q -O wget_giga.out http://giga2025.com/
cmp giga.out wget_giga.out
diff giga.out wget_giga.out
grep -v '^RSP: Date: ' output.txt > cleaned.txt
grep -v '^RSP: Date: ' sample-16-a.txt > a_cleaned.txt
cmp cleaned.txt a_cleaned.txt
diff cleaned.txt a_cleaned.txt

# Cleanup
echo "Cleaning up temporary files"
rm output.txt cleaned.txt a_cleaned.txt my_test.html wget_test.html icsi.gif wget_icsi.gif medium.txt wget_medium.txt medium.dat wget_medium.dat
rm big.dat wget_big.dat giga.out wget_giga.out