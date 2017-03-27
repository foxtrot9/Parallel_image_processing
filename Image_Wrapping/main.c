#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include "image.h"
#include "twist_transformation_serial.h"
#include "twist_transformation_parallel.h"

int main(int argc,char* argv[])
{
    if(argc<3)
        printf("Enter filename\n");

    char* filename = argv[1];
    int noOfThreads = atoi(argv[2]);

    PPMImage *inImage,*outImage;
    inImage = readPPM(filename); // readPPM already contains error detection.

    // Timer functions
    double startTime, endTime, endTime2;

    // Sequential algorithm.
    startTime = omp_get_wtime();
    outImage = twist_transformation_serial(inImage);
    endTime = omp_get_wtime() - startTime;

    printf("Elapsed Time for Sequential Twist Linear Transformation : %0.14f\n", endTime );
    writePPM("output_twisted_lenna_serial.ppm",outImage);

    // Parallel algorithm.
    startTime = omp_get_wtime();
    outImage = twist_transformation_parallel(inImage,noOfThreads);
    endTime2 = omp_get_wtime() - startTime;

    printf("No of threads would have been created = %d\n",noOfThreads);

    printf("Elapsed Time for Parallel Twist Linear Transformation : %0.14f\n", endTime2 );
    writePPM("output_twisted_lenna_parallel.ppm",outImage);

    return 0;
}