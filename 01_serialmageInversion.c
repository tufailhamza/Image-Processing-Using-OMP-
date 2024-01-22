#include <stdio.h>
#include <stdlib.h>
#include <FreeImage.h>
#include <omp.h>

void invertImage(FIBITMAP *input, FIBITMAP *output, int width, int height) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            RGBQUAD pixel;
            FreeImage_GetPixelColor(input, x, y, &pixel);

            pixel.rgbRed = 255 - pixel.rgbRed;
            pixel.rgbGreen = 255 - pixel.rgbGreen;
            pixel.rgbBlue = 255 - pixel.rgbBlue;

            FreeImage_SetPixelColor(output, x, y, &pixel);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s input_image output_image\n", argv[0]);
        return 1;
    }

    const char *input_filename = argv[1];
    const char *output_filename = argv[2];

    FreeImage_Initialise(TRUE);

    FIBITMAP *input_image = FreeImage_Load(FIF_JPEG, input_filename, JPEG_DEFAULT);
    if (!input_image) {
        fprintf(stderr, "Error: Unable to open input image.\n");
        FreeImage_DeInitialise();
        return 1;
    }

    int width = FreeImage_GetWidth(input_image);
    int height = FreeImage_GetHeight(input_image);

    FIBITMAP *output_image = FreeImage_Allocate(width, height, 24, 0, 0, 0);
    if (!output_image) {
        fprintf(stderr, "Error: Unable to allocate memory for the output image.\n");
        FreeImage_Unload(input_image);
        FreeImage_DeInitialise();
        return 1;
    }

    // Measure start time
    double start_time = omp_get_wtime();

    invertImage(input_image, output_image, width, height);

    // Measure end time
    double end_time = omp_get_wtime();

    // Print image information
    printf("Image Format: %s\n", FreeImage_GetFormatFromFIF(FreeImage_GetFIFFromFilename(input_filename)));

    // Print printing time for each processor calculation
    printf("Printing Time per Processor Calculation: %.10f seconds\n", (end_time - start_time) / (width * height));

    // Print total calculation time
    printf("Total Calculation Time: %f seconds\n", end_time - start_time);

    // Print size of image matrix
    printf("Size of Image Matrix: %dx%d\n", width, height);

    if (!FreeImage_Save(FIF_JPEG, output_image, output_filename, JPEG_DEFAULT)) {
        fprintf(stderr, "Error: Unable to save the output image.\n");
    }

    FreeImage_Unload(input_image);
    FreeImage_Unload(output_image);
    FreeImage_DeInitialise();

    return 0;
}

