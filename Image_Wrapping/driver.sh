#!/bin/sh
# Compilation
gcc main.c twist_transformation_serial.c twist_transformation_parallel.c image.c -fopenmp -lm
echo "(if you want to confirm just uncomment "DEBUG :" sections in twist_transformation_parallel.c)"
# Input format : FILENAME HALFWIDTH NOOFTHREADS
./a.out lena512.ppm 1
./a.out lena512.ppm 2
./a.out lena512.ppm 3
./a.out lena512.ppm 4
./a.out lena512.ppm 5
./a.out lena512.ppm 6
./a.out lena512.ppm 7
./a.out lena512.ppm 8