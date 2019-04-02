#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

void process_image_simd(
        unsigned char threeshold,
        char **input_files,
        char **output_files,
        unsigned char number_images,
        unsigned int *sizes
        ) {
    // process each input image
    for (unsigned char i = 0; i < number_images; i++) {
        FILE* input = fopen(input_files[i], "rb");
        FILE* output = fopen(output_files[i], "wb");

        if (input != NULL && output != NULL) {

            // initialize source/destination/counter/threeshold variables
            unsigned char *ptrin = malloc(sizes[i]*sizes[i]* sizeof(unsigned char));
            unsigned char *ptrout = malloc(sizes[i]*sizes[i]* sizeof(unsigned char));
            unsigned int ii = sizes[i]*sizes[i]/16;
            unsigned char *mask = malloc(16 * sizeof(unsigned char));

            if (ptrin == NULL || ptrout == NULL || mask == NULL) {
                printf("Memory left!");
            }

            // load source
            fread(ptrin, sizeof(unsigned char), sizes[i]*sizes[i], input);

            // load mask
            for (unsigned char j = 0; j < 16; j++) {
                mask[j] = threeshold;
            }

            // process input data with threeshold
            __asm__ (
                    "mov        %1, %%esi\n"
                    "mov        %2, %%ecx\n"
                    "mov        %4, %%edi\n"
                    "mov        %3, %%eax\n"
                    "movdqu     (%%eax), %%xmm7\n"
                "l1: \n"
                    "movdqu     (%%esi), %%xmm0\n"
                    "pminub     %%xmm7, %%xmm0\n"
                    "pcmpeqb    %%xmm7, %%xmm0\n"
                    "movdqu     %%xmm0, (%%edi)\n"
                    "add        $16, %%edi\n"
                    "add        $16, %%esi\n"
                    "sub        $1,  %%ecx\n"
                "jnz l1 \n"
                    : "=m" (ptrout) // output
                    : "g" (ptrin), "g" (ii), "g" (mask), "g" (ptrout)  // inputs
                    : "esi", "ecx", "eax", "edi", "xmm7", "xmm0" // clobbers
            );

            // write buffer data into output
            fwrite(ptrout, sizeof(unsigned char), sizes[i]*sizes[i], output);

            free(ptrout);
            free(ptrin);
            free(mask);
            fclose(input);
            fclose(output);
        }
        else {
            printf("Can't open specified file!");
        }

    }
}

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

    // Initialize output files for C
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

    // Initialize output files for SIMD
    output_files_c[0] = "images/output/Escher_out_SIMD.raw";
    output_files_c[1] = "images/output/kid_out_SIMD.raw";
    output_files_c[2] = "images/output/lena_gray_out_SIMD.raw";

    // Process image with SIMD and calculate time
    tstart = clock();
    process_image_simd(threshold, input_files, output_files_c, number_images, sizes);
    tend = clock();
    time_simd = (float)(tend - tstart) / CLOCKS_PER_SEC;
    printf("\nTime spent: %f\n", time_simd);

    // Free memory
    free(input_files);
    free(output_files_c);
    free(sizes);

    (void)time_simd;
    (void)argc;
    (void)argv;
    return EXIT_SUCCESS;
}
