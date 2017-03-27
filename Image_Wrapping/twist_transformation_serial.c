#include "twist_transformation_serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void bilinearlyInterpolate(PPMPixel *inPixel,PPMPixel *outPixel, int columns, int rows, double x, double y,int outX,int outY)
{
    // Interpolate algorithm variables.
    int x1 = (int) x;
    int x2 = x1 + 1;
    int y1 = (int) y;
    int y2 = y1 + 1;
    double m1 = (x2 - x)/(x2 - x1);
    double m2 = (x - x1)/(x2 - x1);
    double p1 = (y2 - y)/(y2 - y1);
    double p2 = (y - y1)/(y2 - y1);
    int q11,q12,q21,q22;
    double temp,temp2;
    int ans;

    // Describing input and output points.
    PPMPixel *outPointer = outPixel + outY + outX*columns;
    PPMPixel *inPointer1 = inPixel + y1 + x1*columns; // Q11
    PPMPixel *inPointer2 = inPixel + y2 + x1*columns; // Q12
    PPMPixel *inPointer3 = inPixel + y1 + x2*columns; // Q21
    PPMPixel *inPointer4 = inPixel + y2 + x2*columns; // Q22

    // Red element of a pixel.
    q11 = inPointer1->red;
    q12 = inPointer2->red;
    q21 = inPointer3->red;
    q22 = inPointer4->red;
    temp = (m1 * q11) + (m2 * q12);
    temp2 = (m1 * q21) + (m2 * q22);
    ans = (int) ( (p1 * temp)+ (p2 * temp2));
    outPointer->red = (unsigned char) ans;

    // Green element of a pixel.
    q11 = inPointer1->green;
    q12 = inPointer2->green;
    q21 = inPointer3->green;
    q22 = inPointer4->green;
    temp = (m1 * q11) + (m2 * q12);
    temp2 = (m1 * q21) + (m2 * q22);
    ans = (int) ( (p1 * temp)+ (p2 * temp2));
    outPointer->green = (unsigned char) ans;

    // Blue element of a pixel.
    q11 = inPointer1->blue;
    q12 = inPointer2->blue;
    q21 = inPointer3->blue;
    q22 = inPointer4->blue;
    temp = (m1 * q11) + (m2 * q12);
    temp2 = (m1 * q21) + (m2 * q22);
    ans = (int) ( (p1 * temp)+ (p2 * temp2));
    outPointer->blue = (unsigned char) ans;
}

PPMImage * twist_transformation_serial(PPMImage *inImage)
{
    double PI = 3.141592;
	// Rows and Columns are most needed variables.
    int rows = inImage->noOfRows;
    int columns = inImage->noOfColumns;

    // Initialising a output Image.
    PPMImage *outImage = (PPMImage *)malloc(sizeof(PPMImage));
    outImage->noOfRows = rows;
    outImage->noOfColumns = columns;
    // Initialising PPMPixel array.  outPixel and inPixel represents pointer to respective image's PPMPixel array.
    PPMPixel *outPixel = (PPMPixel *)malloc(rows*columns*sizeof(PPMPixel));
    if(!outPixel)
        printf("Output Pixel array cannot be allocated. Location of error: twist_transformation_serial function.\n");
    outImage->data = outPixel;

    PPMPixel *inPixel = inImage->data;

    // Variables needed for computation.
    int centerX=rows/2;
    int centerY=columns/2;

    int newX,newY; // New co-ordinates

    double theta; // Angle in radian of a element from center in Input PPMPixel array.
    double radius; // Radius from center of image.

    // Loops variables.
    int loopx,loopy;

    for(loopx=0;loopx < rows;++loopx)
    {
        for (loopy=0; loopy < columns; ++loopy)
        {
            radius = sqrt((loopy-centerY)*(loopy-centerY)+(loopx-centerX)*(loopx-centerX));
            theta = (radius/2.0) * (PI/180);

            newX = (int)( (cos(theta)*(loopy-centerY)) - (sin(theta)*(loopx-centerX)) + centerY );
            newY = (int)( (cos(theta)*(loopx-centerX)) + (sin(theta)*(loopy-centerY)) + centerX );

            if( (newX < 0) || (newX >= rows) || (newY < 0) || (newY >= columns) )
                continue;

            bilinearlyInterpolate(inPixel,outPixel,columns,rows, newX, newY,loopx,loopy);
        }
    }

    return outImage;
}
