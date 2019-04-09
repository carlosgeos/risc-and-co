/**
 * @param input_files
 * @param output_files
 * @param number_images
 * @param sizes
 */
void process_image_simd(char **input_files, char **output_files, unsigned char number_images, unsigned int *sizes);

/**
 * @param input_files
 * @param output_files
 * @param number_images
 * @param sizes
 */
void process_images_c(char **input_files, char **output_files, unsigned char number_images, unsigned int *sizes, unsigned int window_size);
