#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

int main(int argc, char* argv[]) {
    if (argc > 4 || argc < 4) {
        printf("Usage: ./sisd <filename> <output> <threshold>\n");
        return 1;
    }

    char* input_filename = argv[1];
    char* output_filename = argv[2];
    int threshold = atoi(argv[3]);

    FILE* input = fopen(input_filename, "r");
    FILE* output = fopen(output_filename, "w");

    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);
    fseek(input, 0, SEEK_SET);

    unsigned char *buffer = malloc(sizeof(unsigned char) * file_size);

    clock_t tstart, tend;
    tstart = clock();

    fread(buffer, sizeof(unsigned char), file_size, input);

    for (long i = 0; i < file_size; ++i) {
        if ((unsigned char) buffer[i] < threshold) {
            buffer[i] = 0;
        } else {
            buffer[i] = 255;
        }
    }

    fwrite(buffer, sizeof(char), file_size, output);

    tend = clock();
    float time = (float)(tend - tstart) / CLOCKS_PER_SEC;
    printf("Time spent: %f\n", time);

    free(buffer);
    fclose(input);
    fclose(output);

    return EXIT_SUCCESS;
}
