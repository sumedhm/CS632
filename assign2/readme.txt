Files - 

1. matrix_mult.c   	-  	c file containing complete code.
2. run.sh			-  	script to run code.
3. matrix_mult		- 	executable created by run.sh.

How to run?

Just run run.sh script using command-
$ bash run.sh

This creates the executable and also executes it.

The program will first ask to enter 4 digits which are m1,n1,m2,n2 for m1*n1 and m2*n2 matrices.

Then, enter matrix 1 as n1 elements in each of m1 rows, and then similarly matrix 2 to get the product matrix.

You can change the number of processors used in run.sh, 5th line. Currently it is set to 10. Also tested on 5, and 17.
Assumption - No.of processors are (P+1), 1 master, and P is a perfect square.