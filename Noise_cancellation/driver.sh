#!/bin/sh
# Compilation
gcc main.c image_filtering_serial.c image_filtering_parallel.c image.c -fopenmp -lm
echo "(if you want to confirm just uncomment "DEBUG :" sections in image_filtering_parallel.c)"
# Input format : FILENAME HALFWIDTH NOOFTHREADS
./a.out noise1.ppm 3 1
./a.out noise1.ppm 3 2
./a.out noise1.ppm 3 3
./a.out noise1.ppm 3 4
./a.out noise1.ppm 3 5
./a.out noise1.ppm 3 6
./a.out noise1.ppm 3 7
./a.out noise1.ppm 3 8