#include "../include/utils.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define U 0
#define R 1
#define D 2
#define L 3

const int DIRS[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

typedef struct {
  int found[10000][2];
  int len;
  int ans1;
} trails;

void store_in_patch(trails *t, int x, int y) {
  if (t->ans1 == 1) {
    for (int i = 0; i < t->len; i++) {
      if (t->found[i][0] == x && t->found[i][1] == y)
        return;
    }
  }
  t->found[t->len][0] = x, t->found[t->len][1] = y;
  t->len++;
}

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

void traverse(char *input, int rows, int cols, int x, int y, int g, trails *t) {
  for (int dir = 0; dir < 4; dir++) {
    int new_x = x + DIRS[dir][0], new_y = y + DIRS[dir][1];
    if (get_char(input, rows, cols, new_x, new_y) - '0' == g) {
      if (g == 9)
        store_in_patch(t, new_x, new_y);
      else if (g < 9)
        traverse(input, rows, cols, new_x, new_y, g + 1, t);
    }
  }
}
int count_trails(char *input, int rows, int cols, int x, int y, int ans1) {
  trails t = {0};
  t.ans1 = ans1;
  traverse(input, rows, cols, x, y, 1, &t);
  return t.len;
}

void solve(char *input, int rows, int cols) {
  int ans1 = 0, ans2 = 0;
  for (int y = 0; y < rows; y++)
    for (int x = 0; x < cols; x++)
      if (get_char(input, rows, cols, x, y) == '0') {
        ans1 += count_trails(input, rows, cols, x, y, 1);
        ans2 += count_trails(input, rows, cols, x, y, 0);
      }
  printf("ans1: %d\n", ans1);
  printf("ans2: %d\n", ans2);
}

int main() {
  long l;
  char *content = read_file_to_buffer("data/day10.txt", &l);
  if (content) {
    printf("Here we go!\n%s\n", content);
    int cols = strchr(content, '\n') - content;
    int rows = l / (cols + 1);

    solve(content, rows, cols);
    free(content);
  }
  return 0;
}
