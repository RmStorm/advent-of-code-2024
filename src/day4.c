#include "../include/utils.h"
#include <stdio.h>
#include <string.h>

char get_char(char *input, int rows, int cols, int x, int y) {
  if (x < 0 || x > cols || y < 0 || y > rows) {
    return '\0';
  }
  return input[y * (cols + 1) + x];
}

#define XMAS_DIRECTIONS 8

const int ds[XMAS_DIRECTIONS][6] = {
    {1, 2, 3, 0, 0, 0},    {1, 2, 3, 1, 2, 3},    {0, 0, 0, 1, 2, 3},
    {-1, -2, -3, 1, 2, 3}, {-1, -2, -3, 0, 0, 0}, {-1, -2, -3, -1, -2, -3},
    {0, 0, 0, -1, -2, -3}, {1, 2, 3, -1, -2, -3}};

#define X_MAS_DIRECTIONS 4

const int xs[X_MAS_DIRECTIONS][8] = {
    {-1, 1, -1, -1, 1, -1, 1, 1},
    {-1, 1, 1, 1, -1, -1, 1, -1},
    {1, 1, 1, -1, -1, -1, -1, 1},
    {1, -1, -1, -1, -1, 1, 1, 1},
};

void find_xmas(char *input, int rows, int cols) {
  int ans1 = 0, ans2 = 0;
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      char c = get_char(input, rows, cols, x, y);
      printf("%c", c);
      if (c == 'X') {
        for (int i = 0; i < XMAS_DIRECTIONS; i++) {
          int mdx = ds[i][0], adx = ds[i][1], sdx = ds[i][2];
          int mdy = ds[i][3], ady = ds[i][4], sdy = ds[i][5];
          char m = get_char(input, rows, cols, x + mdx, y + mdy);
          char a = get_char(input, rows, cols, x + adx, y + ady);
          char s = get_char(input, rows, cols, x + sdx, y + sdy);
          if (m == 'M' && a == 'A' && s == 'S') {
            ans1 += 1;
          }
        }
      }
      if (c == 'A') {
        for (int i = 0; i < X_MAS_DIRECTIONS; i++) {
          int m1dx = xs[i][0], m1dy = xs[i][1], m2dx = xs[i][2],
              m2dy = xs[i][3];
          int s1dx = xs[i][4], s1dy = xs[i][5], s2dx = xs[i][6],
              s2dy = xs[i][7];
          char m1 = get_char(input, rows, cols, x + m1dx, y + m1dy);
          char s1 = get_char(input, rows, cols, x + s1dx, y + s1dy);
          char m2 = get_char(input, rows, cols, x + m2dx, y + m2dy);
          char s2 = get_char(input, rows, cols, x + s2dx, y + s2dy);
          if (m1 == 'M' && s1 == 'S' && m2 == 'M' && s2 == 'S') {
            ans2 += 1;
          }
        }
      }
    }
    printf("\n");
  }
  printf("ans1: %d\n", ans1);
  printf("ans2: %d\n", ans2);
}

int main() {
  long l;
  char *content = read_file_to_buffer("data/day4.txt", &l);
  if (content) {
    int cols = strchr(content, '\n') - content;
    int rows = l / (cols + 1);

    find_xmas(content, rows, cols);
    free(content);
  }
  return 0;
}
