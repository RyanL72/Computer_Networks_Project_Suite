#!/usr/bin/bash
##test script for proj2
echo "test 1"
./proj2 -i -u http://www.icir.org/ -i -w icir.html > output.txt
cmp output.txt sample-1-i.txt
diff output.txt sample-1-i.txt

echo "test 2"
./proj2 -q -u http://www.icir.org/ -w /dev/null > output.txt
cmp output.txt sample-2-q.txt
diff output.txt sample-2-q.txt

echo "test 3"
./proj2 -u http://www.icir.org/ -w /dev/null -a > output.txt
#cmp output.txt sample-3-a.txt
#diff output.txt sample-3-a.txt
grep -v '^RSP: Date: ' output.txt > cleaned.txt
grep -v '^RSP: Date: ' sample-3-a.txt > a_cleaned.txt
cmp cleaned.txt a_cleaned.txt
diff cleaned.txt a_cleaned.txt

echo "test 4"
./proj2 -u http://dnmmt.com/ -i -w dnmmt.html > output.txt
cmp output.txt sample-5-i.txt
diff output.txt sample-5-i.txt

echo "test 5"
./proj2 -q -u http://dnmmt.com/ -w dnmmt.html > output.txt
cmp output.txt sample-6-q.txt
diff output.txt sample-6-q.txt

echo "test 7"
./proj2 -u http://dnmmt.com/ -w dnmmt.html -a > output.txt
#cmp output.txt sample-3-a.txt
#diff output.txt sample-3-a.txt
grep -v '^RSP: Date: ' output.txt > cleaned.txt
grep -v '^RSP: Date: ' sample-7-a.txt > a_cleaned.txt
cmp cleaned.txt a_cleaned.txt
diff cleaned.txt a_cleaned.txt

echo "test 8"
./proj2 -w /dev/null -i -u http://www.icir.org/mallman/test-files/test.html > output.txt
cmp output.txt sample-8-i.txt
diff output.txt sample-8-i.txt

echo "test 9"
./proj2 -w /dev/null -u htTp://www.icir.org/mallman/test-files/test.html -q > output.txt
cmp output.txt sample-9-q.txt
diff output.txt sample-9-q.txt

echo "test 10"
./proj2 -a -w /dev/null -u HTTP://www.icir.org/mallman/test-files/test.html > output.txt
grep -v '^RSP: Date: ' output.txt > cleaned.txt
grep -v '^RSP: Date: ' sample-10-a-alt.txt > a_cleaned.txt
cmp cleaned.txt a_cleaned.txt
diff cleaned.txt a_cleaned.txt


rm output.txt cleaned.txt a_cleaned.txt icir.html dnmmt.html