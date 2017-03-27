#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include "image.h"
#include "image_filtering_serial.h"
#include "image_filtering_parallel.h"

int main(int argc,char* argv[])
{
    if(argc<4)
        printf("Enter filename halfwidth numberOfthreads:\n");

    char* filename=argv[1];
    int halfWidth = atoi(argv[2]);
    int noOfThreads = atoi(argv[3]);

    PPMImage *inImage,*outImage;
    inImage = readPPMwithPad(filename,halfWidth); // readPPMwithPad already contains error detection and adds padding.

    // Timer functions
    double startTime, endTime, endTime2;

    // Sequential algorithm.
    startTime = omp_get_wtime();
    outImage = image_filtering_serial(inImage,halfWidth);
    endTime = omp_get_wtime() - startTime;

    printf("Elapsed Time for Sequential Twist Linear Transformation : %0.14f\n", endTime );
    writePPM("output_image_filtering_serial.ppm",outImage);

    // Parallel algorithm.
    startTime = omp_get_wtime();
    outImage = image_filtering_parallel(inImage,halfWidth,noOfThreads);
    endTime2 = omp_get_wtime() - startTime;

    printf("No of threads would have been created = %d\n",noOfThreads);

    printf("Elapsed Time for Parallel Twist Linear Transformation : %0.14f\n", endTime2 );
    writePPM("output_image_filtering_parallel.ppm",outImage);


    return 0;
}