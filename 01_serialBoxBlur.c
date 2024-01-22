#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <omp.h>
#include <string.h>


// Function to apply a box blur filter to the image
void applyBoxBlur(const unsigned char *input, unsigned char *output, int width, int height, int channels) {
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            for (int c = 0; c < channels; c++) {
                int sum = 0;
                for (int j = -1; j <= 1; j++) {
                    for (int i = -1; i <= 1; i++) {
                        sum += input[((y + j) * width + (x + i)) * channels + c];
                    }
                }
                output[(y * width + x) * channels + c] = sum / 9;
            }
        }
    }
}

int main() {
    const char *input_file = "input.jpg"; // Change this to the path of your input image
    const char *output_file = "output_serial.jpg"; // Change this to the desired output path

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    FILE *infile = fopen(input_file, "rb");
    if (!infile) {
        fprintf(stderr, "Error opening input file\n");
        return 1;
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    int width = cinfo.output_width;
    int height = cinfo.output_height;
    int channels = cinfo.output_components;

    unsigned char *image = (unsigned char *)malloc(width * height * channels);
    JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, width * channels, 1);

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                image[(cinfo.output_scanline - 1) * width * channels + x * channels + c] = buffer[0][x * channels + c];
            }
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    unsigned char *tempImage = (unsigned char *)malloc(width * height * channels);

    // Measure computation time
    double start_time = omp_get_wtime();

    // Apply the box blur filter
    applyBoxBlur(image, tempImage, width, height, channels);

    memcpy(image, tempImage, width * height * channels);
    // Calculate and print the computation time
    double end_time = omp_get_wtime();
    printf("Serial Computation Time: %f seconds\n", end_time - start_time);

    FILE *outfile = fopen(output_file, "wb");
    if (!outfile) {
        fprintf(stderr, "Error opening output file\n");
        return 1;
    }

    struct jpeg_compress_struct cinfo_out;
    struct jpeg_error_mgr jerr_out;

    cinfo_out.err = jpeg_std_error(&jerr_out);
    jpeg_create_compress(&cinfo_out);
    jpeg_stdio_dest(&cinfo_out, outfile);

    cinfo_out.image_width = width;
    cinfo_out.image_height = height;
    cinfo_out.input_components = channels;
    cinfo_out.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo_out);
    jpeg_start_compress(&cinfo_out, TRUE);

    while (cinfo_out.next_scanline < cinfo_out.image_height) {
        JSAMPROW row_pointer = &image[cinfo_out.next_scanline * width * channels];
        jpeg_write_scanlines(&cinfo_out, &row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo_out);
    jpeg_destroy_compress(&cinfo_out);
    fclose(outfile);


    return 0;
}
