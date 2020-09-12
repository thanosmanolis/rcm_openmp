# Reverse Cuthill-McKee with OpenMP

## Description
Reverse Cuthill-McKee is an algorithm for generating a permutation for sparse matrices. This repo includes a sequential implementation of it, along with a parallel one using OpenMP.

## How to run it

1. If your gcc version is previous than gcc-7, then change it in the Makefile
2. Type ``make`` to compile all implementations, or ``make <exec_name>`` to compile
only one implementation
3. Execution:
    1. Sequential: ``./sequential arg1 arg2 arg3``
    2. OpenMP: ``./openmp arg1 arg2 arg3``

The three arguments, are:
* arg1: n, size of matrix (nxn)
* arg2: density, percentage of non-zero elements
* arg3: filename, write the input and output matrix to a file

The third argument is optional. If no third agument is given, then the program will only calculate the permutation derived from the RCM algorithm, and print the elapsed time. If a third argument is give, then the program apart from the permutation, will also calculate input and output bandwidth, and write the input and output matrices in two files (using the argument in the file names).

If no arguments are included at the run command, then the executable will run with default values (n=500, density=1%). 

