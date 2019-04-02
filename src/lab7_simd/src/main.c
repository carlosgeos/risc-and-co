#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>


void process_images(
        unsigned char threeshold,
        char **input_files,
        char **output_files,
        unsigned char number_images,
        unsigned int *sizes
        ) {

    // process each input image
    for (unsigned char i = 0; i < number_images; i++) {
        FILE* input = fopen(input_files[i], "rb");
        FILE* output = fopen(output_files[i], "w");

        // put input data into the buffer
        unsigned char *buffer = malloc(sizes[i]*sizes[i]* sizeof(unsigned char));
        fread(buffer, sizeof(unsigned char), sizes[i]*sizes[i], input);

        // process input data with threeshold
        for (unsigned int k = 0; k < sizes[i] * sizes[i]; k++) {
            if (buffer[k] < threeshold) {
                buffer[k] = 0;
            }
            else {
                buffer[k] = 255;
            }
        }

        // write buffer data into output
        fwrite(buffer, sizeof(unsigned char), sizes[i]*sizes[i], output);

        // free memory and close files
        free(buffer);
        fclose(input);
        fclose(output);
    }
}

int main(int argc, char* argv[]) {
    float time_c, time_simd;
    unsigned char number_images = 3;
    char **input_files = malloc(number_images * sizeof( char *));
    char **output_files_c = malloc(number_images * sizeof( char *));
    unsigned int *sizes = malloc(number_images * sizeof(unsigned int));
    unsigned char threshold = 127;

    clock_t tstart, tend;

    // Verify that the there is memory.
    if (input_files == NULL || output_files_c == NULL || sizes == NULL) {
        printf("Out of memory");
        exit(1);
    }

    // Initialize input files
    input_files[0] = "images/input/Escher.raw";
    input_files[1] = "images/input/kid.raw";
    input_files[2] = "images/input/lena_gray.raw";

    // Initialize output files
    output_files_c[0] = "images/output/Escher_out_C.raw";
    output_files_c[1] = "images/output/kid_out_C.raw";
    output_files_c[2] = "images/output/lena_gray_out_C.raw";

    // Initialize size of files
    sizes[0] = 1024;
    sizes[1] = 1024;
    sizes[2] = 512;

    // Process image with pure C and calculate time
    tstart = clock();
    process_images(threshold, input_files, output_files_c, number_images, sizes);
    tend = clock();
    time_c = (float)(tend - tstart) / CLOCKS_PER_SEC;
    printf("\nTime spent: %f\n", time_c);

    // Process image with SIMD and calculate time


    // Free memory
    free(input_files);
    free(output_files_c);
    free(sizes);

    (void)time_simd;
    (void)argc;
    (void)argv;
    return EXIT_SUCCESS;
}
