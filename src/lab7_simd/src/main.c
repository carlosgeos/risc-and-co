#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

int main(int argc, char* argv[]) {
    if (argc > 4 || argc < 4) {
        printf("Usage: ./sisd <filename> <output> <treshold>\n");
        return 1;
    }

    char* input_filename = argv[1];
    char* output_filename = argv[2];
    int treshold = atoi(argv[3]);

    FILE* input = fopen(input_filename, "r");
    FILE* output = fopen(output_filename, "w");

    char buffer[256] = {0};

    clock_t tstart, tend;
    tstart = clock();

    while (fgets(buffer, 256, input) != NULL) {
        for (int i = 0; i < 255; ++i) {
            unsigned char curr = (unsigned char) buffer[i];
            if ((unsigned char) curr < treshold) {
                curr = 0;
            } else {
                curr = 255;
            }
            fwrite(&curr, sizeof(curr), 1, output);
        }
    }

    tend = clock();
    float time = (float)(tend - tstart) / CLOCKS_PER_SEC;
    printf("Time spent: %f\n", time);
    return EXIT_SUCCESS;
}
