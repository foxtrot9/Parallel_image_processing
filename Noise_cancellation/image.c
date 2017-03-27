#include "image.h"
#include<stdio.h>
#include<stdlib.h>

#define RGB_COMPONENT_COLOR 255

void writePPMGS(const char *filename, PPMImageGS *img)
{
    FILE *fp;
    //open file for output
    fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }

    //write the header file
    //image format
    fprintf(fp, "P5\n");

    //image size
    fprintf(fp, "%d %d\n",img->x,img->y);

    // rgb component depth
    fprintf(fp, "%d\n",RGB_COMPONENT_COLOR);

    // pixel data
    fwrite(img->data, img->x, img->y, fp);
    fclose(fp);
}

PPMImageGS * changeImage(PPMImage * im)
{
    int rows = im->noOfRows;
    int cols = im->noOfColumns;
    int i,j;

    PPMImageGS *im2 = (PPMImageGS *) malloc(sizeof(PPMImageGS));
    im2->x = rows;
    im2->y = cols;
    im2->data = (PPMPixelGS *) malloc(rows*cols*sizeof(PPMPixelGS));
    int r,g,b,idx;
    for(i=0;i<rows;i++)
    {
        for(j=0; j<cols; j++)
        {
            idx = rows*i + j;
            PPMPixel *temp = im->data + idx;
            r = temp->red;
            g = temp->green;
            b = temp->blue;
            PPMPixelGS *temp2 = im2->data + idx;
            temp2->gs = 0.21*r+0.71*g+0.07*b;
        }
    }

    return im2;
}

PPMImage *readPPM(const char *filename)
{
    char buff[16];
    PPMImage *img;
    FILE *fp;
    int c, rgb_comp_color;
    //open PPM file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }

    //read image format
    if (!fgets(buff, sizeof(buff), fp)) {
        perror(filename);
        exit(1);
    }

    //check the image format
    if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Invalid image format (must be 'P6')\n");
        exit(1);
    }

    //alloc memory form image
    img = (PPMImage *)malloc(sizeof(PPMImage));
    if (!img) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }

    //check for comments
    c = getc(fp);
    while (c == '#') {
        while (getc(fp) != '\n') ;
        c = getc(fp);
    }

    ungetc(c, fp);
    //read image size information
    if (fscanf(fp, "%d %d", &img->noOfRows, &img->noOfColumns) != 2) {
        fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
        exit(1);
    }

    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
        fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
        exit(1);
    }

    //check rgb component depth
    if (rgb_comp_color!= RGB_COMPONENT_COLOR) {
        fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
        exit(1);
    }

    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    img->data = (PPMPixel*)malloc(img->noOfRows * img->noOfColumns * sizeof(PPMPixel));

    //read pixel data from file
    if (fread(img->data, 3 * img->noOfRows, img->noOfColumns, fp) != img->noOfColumns) {
        fprintf(stderr, "Error loading image '%s'\n", filename);
        exit(1);
    }

    fclose(fp);
    return img;
}

PPMImage *readPPMwithPad(const char *filename,int halfWidth)
{
    char buff[16];
    PPMImage *img;
    FILE *fp;
    int c, rgb_comp_color;
    //open PPM file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }

    //read image format
    if (!fgets(buff, sizeof(buff), fp)) {
        perror(filename);
        exit(1);
    }

    //check the image format
    if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Invalid image format (must be 'P6')\n");
        exit(1);
    }

    //alloc memory form image
    img = (PPMImage *)malloc(sizeof(PPMImage));
    if (!img) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }

    //check for comments
    c = getc(fp);
    while (c == '#') {
        while (getc(fp) != '\n') ;
        c = getc(fp);
    }

    ungetc(c, fp);
    //read image size information
    if (fscanf(fp, "%d %d", &img->noOfRows, &img->noOfColumns) != 2) {
        fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
        exit(1);
    }

    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
        fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
        exit(1);
    }

    //check rgb component depth
    if (rgb_comp_color!= RGB_COMPONENT_COLOR) {
        fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
        exit(1);
    }

    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    int actualRows = img->noOfRows + 2*halfWidth;
    int actualColumns = img->noOfColumns + 2*halfWidth;
    img->data = (PPMPixel*)malloc(actualRows * actualColumns * sizeof(PPMPixel));
    PPMPixel *temp = img->data + halfWidth*actualColumns;
    //read pixel data from file
    int count;
    for(count = 0; count < img->noOfRows; ++count)
    {
        temp += halfWidth;
        if (fread(temp, 3, img->noOfColumns, fp) != img->noOfColumns)
        {
            fprintf(stderr, "Error loading image '%s'\n", filename);
            exit(1);
        }
        temp += img->noOfColumns + halfWidth;
    }

    int i,j;
    // Filling up parts that are left.
    temp = img->data;
    PPMPixel *temp_const = img->data;

    // Top part mid.
    for(i = 0;i < halfWidth; ++i)
    {
        temp += halfWidth;
        temp_const = img->data + halfWidth*actualColumns + halfWidth;
        for(j = 0;j < img->noOfColumns;j++)
        {
            *temp = *temp_const;
            temp++;
            temp_const++;
        }
        temp +=halfWidth;
    }

    // Bottom part mid.
    temp = img->data + (halfWidth+img->noOfRows)*actualColumns;
    for(i = 0;i < halfWidth; ++i)
    {
        temp += halfWidth;
        temp_const = img->data + ((halfWidth+img->noOfRows)-1)*actualColumns + halfWidth;
        for(j = 0;j < img->noOfColumns;j++)
        {
            *temp = *temp_const;
            temp++;
            temp_const++;
        }
        temp +=halfWidth;
    }

    // Left part all and right part all.
    temp = img->data;
    for(i = 0; i < actualRows; ++i)
    {
        temp_const = temp + halfWidth;
        for(j = 0; j < halfWidth; j++)
        {
            *temp = *temp_const;
            temp++;
        }
        temp +=img->noOfColumns;
        temp_const = temp - 1;
        for(j = 0; j < halfWidth; j++)
        {
            *temp = *temp_const;
            temp++;
        }
    }

    // update noOfRows and noOfColumns.
    img->noOfColumns = actualColumns;
    img->noOfRows = actualRows;
    fclose(fp);
    return img;
}

void writePPM(const char *filename, PPMImage *img)
{
    FILE *fp;
    //open file for output
    fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }

    //write the header file
    //image format
    fprintf(fp, "P6\n");

    //comments


    //image size
    fprintf(fp, "%d %d\n",img->noOfRows,img->noOfColumns);

    // rgb component depth
    fprintf(fp, "%d\n",255);

    // pixel data
    fwrite(img->data, 3 * img->noOfRows, img->noOfColumns, fp);
    fclose(fp);
}

// Usages of functions mentioned in this files.
int test_image_function(int argc, char* argv[])
{
    if(argc<2)
        printf("Enter filename\n");
    char* filename=argv[1];

    PPMImage *image;
    clock_t start, end;

    image = readPPM(filename);

    // start=clock();
    /* float start_omp=omp_get_wtime(); */
    PPMImageGS * x = changeImage(image);
    /* float stop_omp=omp_get_wtime(); */
    // end=clock();

    // printf("Time: %0.10f\n", (end-start)/(double)CLOCKS_PER_SEC );
    /* printf("Time: %0.10f\n", (stop_omp-start_omp) ); */
    writePPMGS("lenags.ppm",x);
}