#ifndef UTILS_H
#define UTILS_H

// Include necessary standard libraries
#include <stdio.h>
#include <stdlib.h>

// Function declarations
void print_array(int *array, int size);
char *read_file_to_buffer(const char *filename, long *out_length);

#endif // UTILS_H
