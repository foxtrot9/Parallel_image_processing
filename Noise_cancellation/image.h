#ifndef LAB3_IMAGE_H
#define LAB3_IMAGE_H

typedef struct {
    unsigned char red,green,blue;
} PPMPixel;

typedef struct {
    int noOfRows, noOfColumns;
    PPMPixel *data;
} PPMImage;

typedef struct {
    unsigned char gs;
} PPMPixelGS;


typedef struct {
    int x, y;
    PPMPixelGS *data;
} PPMImageGS;

void writePPMGS(const char *filename, PPMImageGS *img);
PPMImageGS *changeImage(PPMImage * im);
PPMImage *readPPM(const char *filename);
PPMImage *readPPMwithPad(const char *filename,int halfWidth);
void writePPM(const char *filename, PPMImage *img);
// int test_image_function(int argc, char* argv[]);

#endif
