#!bin/bash

reset
mpicc -o matrix_mult matrix_mult100_100.c -lm
mpirun -np 10 matrix_mult
