#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

int main(int argc, char* argv[]) {
	if (argc > 3 || argc < 3) {
		printf("Usage: ./simd <filename> <output>\n");
	}

	char* input_filename = argv[1];
	char* output_filename = argv[2];

	FILE* input = fopen(input_filename, "r");
	FILE* output = fopen(output_filename, "w");

	char buffer[255];
	int content[255];

	clock_t tstart, tend;
	tstart = clock();

	while (fgets(buffer, 255, input) != NULL) {
		int byte_found = 0;
		for (int i = 0; i < 255; ++i) {
			int start = i;
			char number_buf[3];
			while (isdigit(buffer[i])) {
				number_buf[i - start] = buffer[i];
				++i;
			}

			if (i != start) {
				content[byte_found] = atoi(number_buf);
				++byte_found;
			}
		}

		for (int i = 0; i < byte_found; ++i) {
			fprintf(output, "%i ", content[i]);
		}
	}

	tend = clock();
	float time = (float)(tend - tstart) / CLOCKS_PER_SEC;
	printf("Time spent: %f\n", time);
	return EXIT_SUCCESS;
}
