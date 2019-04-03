#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"


// FUNCTION DECLARATION:

/**
 * @param input_files 
 * @param output_files 
 * @param number_images 
 * @param sizes 
 */
void process_image_simd(
        char **input_files,
        char **output_files,
        unsigned char number_images,
        unsigned int *sizes
        );

/**
 * @param input_files 
 * @param output_files 
 * @param number_images 
 * @param sizes 
 */
void process_images_c(
        char **input_files,
        char **output_files,
        unsigned char number_images,
        unsigned int *sizes,
        unsigned int window_size
        );



// Function implementation
void process_image_simd(
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

            // initialize source/destination/counter variables
            unsigned char *ptrin = malloc(sizes[i]*sizes[i]* sizeof(unsigned char));
            unsigned char *ptrout = malloc(sizes[i]*sizes[i]* sizeof(unsigned char));
            unsigned long ii = (sizes[i]*sizes[i]-(2*sizes[i]))/14 ;

            // validate that there is enough memory
            if (ptrin == NULL || ptrout == NULL) {
                printf("Memory left!");
            }

            // load source
            fread(ptrin, sizeof(unsigned char), sizes[i]*sizes[i], input);

            __asm__ (
                "start: \n"
                    "mov        %1, %%rsi\n"
                    "mov        %2, %%rcx\n"
                    "mov        %3, %%rdi\n"
                "l1: \n"
                    "movdqu     (%%rsi), %%xmm0\n"
                    "movdqu     %%xmm0, %%xmm3\n"
                    "movdqu     1024(%%rsi), %%xmm1\n"
                    "movdqu     2048(%%rsi), %%xmm2\n"
                    "pmaxub     %%xmm1, %%xmm0\n" // Calculate max
                    "pmaxub     %%xmm2, %%xmm0\n"
                    "pminub     %%xmm1, %%xmm3\n" // Calculate Min
                    "pminub     %%xmm2, %%xmm3\n"
                    "movdqu     %%xmm0, %%xmm6\n" // Put max in aux registers
                    "movdqu     %%xmm0, %%xmm7\n"
                    "movdqu     %%xmm3, %%xmm4\n" // Put min in aux registers
                    "movdqu     %%xmm3, %%xmm5\n"
                    "pslldq     $1, %%xmm6\n" // Shift max
                    "pslldq     $2, %%xmm7\n"
                    "psrldq     $1, %%xmm4\n" // Shift min
                    "psrldq     $2, %%xmm5\n"
                    "pmaxub     %%xmm7, %%xmm6\n" // Get max between the 3 rows
                    "pmaxub     %%xmm0, %%xmm6\n"
                    "pminub     %%xmm5, %%xmm4\n" // Get the min between the 3 rows
                    "pminub     %%xmm3, %%xmm4\n"
                    "psubusb      %%xmm4, %%xmm6\n" // Make subtraction
                    "movdqu     %%xmm6, (%%rdi)\n"
                    "add        $14, %%rdi\n"
                    "add        $14, %%rsi\n"
                    "sub        $1,  %%rcx\n"
                "jnz l1 \n"
                    : "=m" (ptrout) // output
                    : "g" (ptrin), "g" (ii), "g" (ptrout) // inputs
                    : "rsi", "rcx", "rax", "rdi", "xmm7", "xmm0", "xmm1", "xmm2", "xmm6" // clobbers
            );

            // write buffer data into output
            fwrite(ptrout, sizeof(unsigned char), sizes[i]*sizes[i], output);

            free(ptrout);
            free(ptrin);
            fclose(input);
            fclose(output);
        }
        else {
            printf("Can't open specified file!");
        }

    }
}

void process_images_c(
        char **input_files,
        char **output_files,
        unsigned char number_images,
        unsigned int *sizes,
        unsigned int window_size
        ) {
    // process each input image
    for (unsigned char i = 0; i < number_images; i++) {
        FILE* input = fopen(input_files[i], "rb");
        FILE* output = fopen(output_files[i], "w");
        unsigned int counter_buffer_out = 0;
        unsigned int buffer_out_size = (sizes[i]-2)*(sizes[i]-2);

        // put input data into the buffer
        unsigned char *buffer = malloc(sizes[i]*sizes[i]* sizeof(unsigned char));
        unsigned char *buffer_out = malloc(buffer_out_size* sizeof(unsigned char));
        fread(buffer, sizeof(unsigned char), sizes[i]*sizes[i], input);
        unsigned char *window_elements = malloc(sizeof(unsigned char)*window_size*window_size);
        // process input data with threshold
        for (unsigned int k = 0; k < (sizes[i]*sizes[i]); k++) {
              for (unsigned int m = 0; m < window_size*window_size;m++){
               window_elements[m] = buffer[(int)(floor(m/window_size)*sizes[i]) + m%window_size + k];
              }
              quicksort(window_elements,0,(window_size*window_size)-1);

              buffer_out[counter_buffer_out] = window_elements[(window_size*window_size)-1] - window_elements[0];
              counter_buffer_out++;

        }

        // write buffer data into output
        fwrite(buffer_out, sizeof(unsigned char), buffer_out_size, output);

        // free memory and close files
        free(buffer);
        fclose(input);
        fclose(output);
    }
}

/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {
    float time_c, time_simd;
    unsigned char number_images = 3;
    char **input_files = malloc(number_images * sizeof( char *));
    char **output_files_c = malloc(number_images * sizeof( char *));
    unsigned int *sizes = malloc(number_images * sizeof(unsigned int));

    clock_t tstart, tend;

    // Verify that there is enough memory.
    if (input_files == NULL || output_files_c == NULL || sizes == NULL) {
        printf("Out of memory");
        exit(1);
    }

    // Initialize input files
    input_files[0] = "Escher.raw";
    input_files[1] = "kid.raw";
    input_files[2] = "lena_gray.raw";

    // Initialize output files for C
    output_files_c[0] = "Escher_out_C.raw";
    output_files_c[1] = "kid_out_C.raw";
    output_files_c[2] = "lena_gray_out_C.raw";

    // Initialize size of files
    sizes[0] = 1024;
    sizes[1] = 1024;
    sizes[2] = 512;

    // Process image with pure C and calculate time
    tstart = clock();
    process_images_c(input_files, output_files_c, number_images, sizes,5);
    tend = clock();
    time_c = (float)(tend - tstart) / CLOCKS_PER_SEC;
    printf("\nTime spent C: %f\n", time_c);

    // Initialize output files for SIMD
    output_files_c[0] = "Escher_out_SIMD.raw";
    output_files_c[1] = "kid_out_SIMD.raw";
    output_files_c[2] = "lena_gray_out_SIMD.raw";

    // Process image with SIMD and calculate time
    tstart = clock();
    process_image_simd(input_files, output_files_c, number_images, sizes);
    tend = clock();
    time_simd = (float)(tend - tstart) / CLOCKS_PER_SEC;
    printf("\nTime spent SIMD: %f\n", time_simd);

    // Free memory
    free(input_files);
    free(output_files_c);
    free(sizes);

    (void)time_c;
    (void)argc;
    (void)argv;
    return EXIT_SUCCESS;
}
