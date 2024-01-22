#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

#define IMAGESIZE 54

#pragma pack(push, 2)
typedef struct {
    char sign;
    int size;
    int notused;
    int data;
    int headwidth;
    int width;
    int height;
    short numofplanes;
    short bitpix;
    int method;
    int arraywidth;
    int horizresol;
    int vertresol;
    int colnum;
    int basecolnum;
} img;
#pragma pop

struct timeval start_time, stop_time, elapsed_time;

char* openImg(int inputFileNumber, img* in);
void displayImageDetails(img* bmp);
void generateImg(char* imgdata, img* out);
int setBoundary(int i, int min, int max);

int main(int argc, char *argv[]) {
    int radius = atoi(argv[1]);
    unsigned char* imgdata;
    img* bmp = (img*) malloc (IMAGESIZE);
    int inputFileNumber = atoi(argv[2]);   
    imgdata = openImg(inputFileNumber, bmp);

    // Display image details
    displayImageDetails(bmp);

    int width = bmp->width;
    int height = bmp->height;

    unsigned char* red;
    unsigned char* green;
    unsigned char* blue;
    red = (unsigned char*) malloc (width * height);
    green = (unsigned char*) malloc(width * height);
    blue = (unsigned char*) malloc(width * height);
   
    int i, j;
    int pos = 0;

    int rgb_width = width * 3;
    if ((width * 3 % 4) != 0) {
        rgb_width += (4 - (width * 3 % 4));
    }

    for (i = 0; i < height; i++) {
        for (j = 0; j < width * 3; j += 3, pos++) {
            red[pos] = imgdata[i * rgb_width + j];
            green[pos] = imgdata[i * rgb_width + j + 1];
            blue[pos] = imgdata[i * rgb_width + j + 2];
        }
    }

    double total_execution_time = 0.0;

    printf("| Matrix Size | Processs | Execution Time (s) |\n");
    printf("|-------------|-----------|---------------------|\n");
     
    for (int size = 100; size <= 1000; size += 100) {
        double process_execution_time = 0.0;

        for (int k = 0; k < 10; k++) {
            gettimeofday(&start_time, NULL);

            double redSum = 0;
            double greenSum = 0;
            double blueSum = 0;
            double weightSum = 0;

            for (i = 0; i < height; i++) {
                for (j = 0; j < width; j++) {
                    for (double row = i - radius; row <= i + radius; row++) {
                        for (double col = j - radius; col <= j + radius; col++) {
                            int x = setBoundary(col, 0, width - 1);
                            int y = setBoundary(row, 0, height - 1);
                            int tempPos = y * width + x;

                            double square = (col - j) * (col - j) + (row - i) * (row - i);
                            double sigma = radius * radius;
                            double weight = exp(-square / (2 * sigma)) / (3.14 * 2 * sigma);

                            redSum += red[tempPos] * weight;
                            greenSum += green[tempPos] * weight;
                            blueSum += blue[tempPos] * weight;
                            weightSum += weight;
                        }
                    }
                    red[i * width + j] = round(redSum / weightSum);
                    green[i * width + j] = round(greenSum / weightSum);
                    blue[i * width + j] = round(blueSum / weightSum);

                    redSum = 0;
                    greenSum = 0;
                    blueSum = 0;
                    weightSum = 0;
                }
            }

            gettimeofday(&stop_time, NULL);
            timersub(&stop_time, &start_time, &elapsed_time);
            process_execution_time += elapsed_time.tv_sec + elapsed_time.tv_usec / 1000000.0;

            printf("| %dx%d\t  |", size, size);
            printf("%d\t      |", k + 1);
            printf(" %f\t\t |\n", elapsed_time.tv_sec + elapsed_time.tv_usec / 1000000.0);

            pos = 0;
            for (i = 0; i < height; i++) {
                for (j = 0; j < width * 3; j += 3, pos++) {
                    imgdata[i * rgb_width + j] = red[pos];
                    imgdata[i * rgb_width + j + 1] = green[pos];
                    imgdata[i * rgb_width + j + 2] = blue[pos];
                }
            }
            generateImg(imgdata, bmp);
        }
        total_execution_time += process_execution_time;
        printf("|-------------|-----------|---------------------|\n");
    }

    printf("| Total Execution Time: %.3f seconds\n", total_execution_time);
    displayImageDetails(bmp);

    free(bmp);
    free(red);
    free(green);
    free(blue);
    free(imgdata);
    return 0;
}

char* openImg(int inputFileNumber, img* in) {
    char inPutFileNameBuffer[32];
    sprintf(inPutFileNameBuffer, "%d.bmp", inputFileNumber);

    FILE* file;
    if (!(file = fopen(inPutFileNameBuffer, "rb"))) {
        printf("File not found!");
        free(in);
        exit(1);
    }
    fread(in, 54, 1, file);
    if (in->bitpix != 24) {
        free(in);
        printf("Need 24 bit bmp file!");
        exit(1);
    }
    char* data = (char*) malloc (in->arraywidth);
    fseek(file, in->data, SEEK_SET);
    fread(data, in->arraywidth, 1, file);
    fclose(file);
    return data;
}

void displayImageDetails(img* bmp) {
    printf("Image Details:\n");
    printf("  Image Width: %d\n", bmp->width);
    printf("  Image Height: %d\n", bmp->height);
    printf("  Number of Matrices: %d\n", bmp->numofplanes);
    printf("  Bits per Pixel: %d\n", bmp->bitpix);
    printf("  Compression Method: %d\n", bmp->method);
    printf("  Horizontal Resolution: %d pixels per meter\n", bmp->horizresol);
    printf("  Vertical Resolution: %d pixels per meter\n", bmp->vertresol);
    printf("  Number of Colors: %d\n", bmp->colnum);
    printf("  Important Colors: %d\n", bmp->basecolnum);
    printf("\n");
}

void generateImg(char* imgdata, img* out) {
    FILE* file;
    time_t now;
    time(&now);
    char fileNameBuffer[64];
    sprintf(fileNameBuffer, "%ld.bmp", now);
    file = fopen(fileNameBuffer, "wb");
    fwrite(out, IMAGESIZE, 1, file);
    fseek(file, out->data, SEEK_SET);
    fwrite(imgdata, out->arraywidth, 1, file);
    fclose(file);
}

int setBoundary(int i, int min, int max) {
    if (i < min) return min;
    else if (i > max) return max;
    return i;
}

