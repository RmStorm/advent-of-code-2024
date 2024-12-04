#include <stdio.h>
#include <stdlib.h>

void print_array(int *array, int size) {
  printf("[");
  for (int i = 0; i < size; i++) {
    printf("%d", array[i]);
    if (i < size - 1) {
      printf(", "); // Add comma between elements
    }
  }
  printf("]\n");
}

char *read_file_to_buffer(const char *filename, long *out_length) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Failed to open file");
    return NULL;
  }

  // Get file size
  fseek(file, 0, SEEK_END);
  long filesize = ftell(file);
  rewind(file);

  // Allocate buffer
  char *buffer = malloc(filesize + 1); // +1 for null terminator
  if (!buffer) {
    perror("Failed to allocate memory");
    fclose(file);
    return NULL;
  }

  // Read file into buffer
  fread(buffer, 1, filesize, file);
  buffer[filesize] = '\0'; // Null-terminate the buffer

  fclose(file);
  // Set output parameter
  if (out_length) {
    *out_length = filesize;
  }
  return buffer;
}
