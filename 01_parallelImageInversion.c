#include <stdio.h>
#include <stdlib.h>
#include <FreeImage.h>
#include <omp.h>

void printImageInfo(FIBITMAP *image) {
    int width = FreeImage_GetWidth(image);
    int height = FreeImage_GetHeight(image);
    int bpp = FreeImage_GetBPP(image);

    printf("Image Information:\n");
    printf("  Size: %dx%d pixels\n", width, height);
    printf("  Bits Per Pixel: %d\n", bpp);
}

void invertImage(FIBITMAP *input, FIBITMAP *output, int width, int height) {
    #pragma omp parallel for collapse(2)
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

    // Print input image information
    printImageInfo(input_image);

    int width = FreeImage_GetWidth(input_image);
    int height = FreeImage_GetHeight(input_image);

    // Create the output image
    FIBITMAP *output_image = FreeImage_Allocate(width, height, 24, 0, 0, 0);
    if (!output_image) {
        fprintf(stderr, "Error: Unable to allocate memory for the output image.\n");
        FreeImage_Unload(input_image);
        FreeImage_DeInitialise();
        return 1;
    }

    // Loop to run the inversion with an increasing number of threads
    for (int num_threads = 4; num_threads <= 40; num_threads += 4) {
        // Set the number of threads
        omp_set_num_threads(num_threads);

        // Measure start time
        double start_time = omp_get_wtime();

        // Invert the image
        invertImage(input_image, output_image, width, height);

        // Measure end time
        double end_time = omp_get_wtime();

        // Print the result for the current number of threads
        printf("Time with %d Threads: %.6f seconds\n", num_threads, end_time - start_time);
    }

    // Save the output image
    if (!FreeImage_Save(FIF_JPEG, output_image, output_filename, JPEG_DEFAULT)) {
        fprintf(stderr, "Error: Unable to save the output image.\n");
    }

    // Unload images and deinitialize FreeImage
    FreeImage_Unload(input_image);
    FreeImage_Unload(output_image);
    FreeImage_DeInitialise();

    return 0;
}