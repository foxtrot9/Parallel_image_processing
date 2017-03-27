#include "image_filtering_serial.h"
#include "image.h"
#include <stdio.h>
#include <stdlib.h>

int bubbleSort(int *arr, int size)
{
    int i=0,j=0,swap=0;
    int lastIndex = size - 1;
    for (i = 0; i < lastIndex; ++i)
    {
        for (j = 0; j < (lastIndex - i); ++j)
        {
            if(arr[j]>arr[j+1])
            {
                swap = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = swap;
            }
        }
    }
    return (((size % 2) == 0) ? (arr[(lastIndex) / 2] + arr[size / 2]) / 2 : arr[size / 2] / 2);
}

void findMedian(PPMPixel *inPixel, PPMPixel *outPixel, int actualColumns, int actualRows, int x, int y, int halfWidth)
{
    int outX = x;
    int outY = y;
    x = x + halfWidth;
    y = y + halfWidth;
    // Variables for finding neighbourhood matrix
    int x1 = x - halfWidth;
    int x2 = x + halfWidth;
    int y1 = y - halfWidth;
    int y2 = y + halfWidth;
    int ans;
    int arrSize = (2*halfWidth+1)*(2*halfWidth+1);

    int arr[arrSize];
    // Describing input and output points.
    PPMPixel *outPointer = outPixel + outY + outX*(actualColumns-2*halfWidth);

    // Calculating median.
    PPMPixel *tempPointer;
    int i,j,temp;

    // Red component.
    temp = 0;
    for(i = x1;i <= x2;++i)
    {
        tempPointer = inPixel + y1 + x1*actualColumns;
        for (j = y1; j <= y2; ++j)
        {
            arr[temp] = tempPointer->red;
            temp++;
            tempPointer++;
        }
    }
    ans = bubbleSort(arr, arrSize);
    outPointer->red = (unsigned char) ans;

    // Green component.
    temp = 0;
    for(i = x1;i <= x2;++i)
    {
        tempPointer = inPixel + y1 + x1*actualColumns;
        for (j = y1; j <= y2; ++j)
        {
            arr[temp] = tempPointer->green;
            temp++;
            tempPointer++;
        }
    }
    ans = bubbleSort(arr, arrSize);
    outPointer->green = (unsigned char) ans;

    // Blue component.
    temp = 0;
    for(i = x1;i <= x2;++i)
    {
        tempPointer = inPixel + y1 + x1*actualColumns;
        for (j = y1; j <= y2; ++j)
        {
            arr[temp] = tempPointer->blue;
            temp++;
            tempPointer++;
        }
    }
    ans = bubbleSort(arr, arrSize);
    outPointer->blue = (unsigned char) ans;

}

PPMImage * image_filtering_serial(PPMImage *inImage,int halfWidth)
{
    // Rows and Columns are most needed variables.
    int actualColumns = inImage->noOfColumns;
    int actualRows = inImage->noOfRows;
    int rows = actualRows - 2*halfWidth;
    int columns = actualColumns - 2*halfWidth;

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

    // Loops variables.
    int loopx,loopy;

    for(loopx=0;loopx < rows;++loopx)
    {
        for (loopy=0; loopy < columns; ++loopy)
        {
            findMedian(inPixel, outPixel, actualColumns, actualRows, loopx, loopy, halfWidth);
        }
    }

    return outImage;
}

