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
    clock_t tstart, tend;
    tstart = clock();
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

        fclose(input);
        fclose(output);
    }
    tend = clock();
    float time = (float)(tend - tstart) / CLOCKS_PER_SEC;
    printf("\nTime spent: %f\n", time);
}

int main(int argc, char* argv[]) {

    unsigned char number_images = 3;
    char **input_files = malloc(number_images * sizeof( char *));
    char **output_files = malloc(number_images * sizeof( char *));
    unsigned int *sizes = malloc(number_images * sizeof(unsigned int));

    input_files[0] = "../images/input/Escher.raw";
    input_files[1] = "../images/input/kid.raw";
    input_files[2] = "../images/input/lena_gray.raw";

    output_files[0] = "../images/output/Escher.raw";
    output_files[1] = "../images/output/kid.raw";
    output_files[2] = "../images/output/lena_gray.raw";

    sizes[0] = 1024;
    sizes[1] = 1024;
    sizes[2] = 512;

    process_images(127, input_files, output_files, number_images, sizes);

    (void)argc;
    (void)argv;
    return EXIT_SUCCESS;
}
