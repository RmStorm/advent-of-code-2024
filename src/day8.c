#include "../include/utils.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

const int CHARS = 256;
const int MAX_MATCHES = 100;
char get_char(char *input, int rows, int cols, int x, int y) {
  if (x < 0 || x >= cols || y < 0 || y >= rows) {
    return '\0';
  }
  return input[y * (cols + 1) + x];
}

void set_char(char *input, int rows, int cols, int x, int y, char c) {
  if (get_char(input, rows, cols, x, y) == '\0') {
    fprintf(stderr, "Cannot set a char outside bounds! Check your inputs you "
                    "fool off a took!\n");
    exit(1);
  }
  input[y * (cols + 1) + x] = c;
}

void set_visited(char *input, int rows, int cols, uint8_t (*visited)[rows],
                 int x, int y) {
  if (get_char(input, rows, cols, x, y) == '\0')
    return;
  visited[x][y] = 1;
}

void find_anti_nodes(char freq, int rows, int cols, char *input,
                     uint8_t (*visited)[rows], int c_start, int copies) {
  int(*matches)[2] = malloc(MAX_MATCHES * sizeof(*matches));
  int m = 0;
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      char c = get_char(input, rows, cols, x, y);
      if (c == freq) {
        matches[m][0] = x;
        matches[m][1] = y;
        m++;
      }
    }
  }
  for (int i = 0; i < m; i++) {
    // store primary antenna coords:
    int px = matches[i][0], py = matches[i][1];
    for (int j = i + 1; j < m; j++) {
      // store secondary antenna coords:
      int sx = matches[j][0], sy = matches[j][1];
      int xdiff = px - sx, ydiff = py - sy;
      for (int c = c_start; c <= copies; c++) {
        set_visited(input, rows, cols, visited, px + (c * xdiff),
                    py + (c * ydiff));
        set_visited(input, rows, cols, visited, sx - (c * xdiff),
                    sy - (c * ydiff));
      }
    }
  }
}

void solve(char *input, int rows, int cols) {
  // First find the start
  char frequencies[CHARS];
  memset(frequencies, '\0', sizeof(frequencies));
  int start_x = -1, start_y = -1;
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      char c = get_char(input, rows, cols, x, y);
      if (!(c == '.' || strchr(frequencies, c)))
        frequencies[strlen(frequencies)] = c;
    }
  }
  printf("types: %s\n", frequencies);
  int ans1 = 0, ans2 = 0;

  uint8_t(*visited)[rows] = malloc(cols * sizeof(*visited));
  if (!visited) {
    perror("Failed to allocate memory");
    exit(1);
  }
  for (int x = 0; x < cols; x++)
    for (int y = 0; y < rows; y++)
      visited[x][y] = 0; // Initialize all cells to 0x00 (no directions visited)

  for (int i = 0; i < strlen(frequencies); i++)
    find_anti_nodes(frequencies[i], rows, cols, input, visited, 1, 1);

  for (int x = 0; x < cols; x++)
    for (int y = 0; y < rows; y++)
      ans1 += visited[x][y];

  for (int i = 0; i < strlen(frequencies); i++)
    find_anti_nodes(frequencies[i], rows, cols, input, visited, 0, 50);

  for (int x = 0; x < cols; x++)
    for (int y = 0; y < rows; y++)
      ans2 += visited[x][y];

  printf("ans1 = %d\n", ans1);
  printf("ans2 = %d\n", ans2);
}

int main() {
  long l;
  char *content = read_file_to_buffer("data/day8.txt", &l);
  if (content) {
    printf("Here we go!\n%s\n", content);
    int cols = strchr(content, '\n') - content;
    int rows = l / (cols + 1);

    solve(content, rows, cols);
    free(content);
  }
  return 0;
}
