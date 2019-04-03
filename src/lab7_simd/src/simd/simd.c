#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

#define SIM_BYTES 16


int main(int argc, char* argv[]) {
    if (argc > 4 || argc < 4) {
        printf("Usage: ./sisd <filename> <output> <treshold>\n");
        return 1;
    }

    // Receive arguments from command line
    char* input_filename = argv[1];
    char* output_filename = argv[2];
    int threshold = atoi(argv[3]);

    // Get input and output varaibles
    FILE* input = fopen(input_filename, "r");
    FILE* output = fopen(output_filename, "w");


    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);
    fseek(input, 0, SEEK_SET);

    // Memory allocation for buffer
    unsigned char *buffer = malloc(sizeof(unsigned char) * (file_size + ((-file_size) % SIM_BYTES)));

    // Initialize thresholds to act as a mask
    unsigned char thresholds[SIM_BYTES];
    for (int i = 0; i < SIM_BYTES; ++i) {
        thresholds[i] = threshold; // The data need to be put in an array to work with SIMD instructions
    }

    clock_t tstart, tend;
    tstart = clock();

    fread(buffer, sizeof(unsigned char), file_size, input);

    // Make a loop and increment counter by 16
    for (long i = 0; i < file_size; i += SIM_BYTES) {
        unsigned char *buffer_position = (unsigned char *) (buffer + i); // Get the current position to transverse the image
        __asm__ __volatile__ (
            "movdqu (%2), %%xmm0;"
            "movdqu (%1), %%xmm1;"
            "pminub %%xmm0, %%xmm1;"
            "pcmpeqb %%xmm0, %%xmm1;"
            "movdqu %%xmm1, (%0);"
            : "=r" (buffer_position)
            : "0" (buffer_position), "r" (thresholds)
            : "%xmm0", "%xmm1"
            );
    }

    // Write the buffer into the output file
    fwrite(buffer, sizeof(char), file_size, output);

    tend = clock();
    float time = (float)(tend - tstart) / CLOCKS_PER_SEC;
    printf("Time spent: %f\n", time);

    free(buffer);
    fclose(input);
    fclose(output);

    return EXIT_SUCCESS;
}
