#include "../include/utils.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// https://stackoverflow.com/questions/3585846/color-text-in-terminal-applications-in-unix
#define KNRM "\x1B[0m"
#define KGRN "\x1B[32m"
#define KRED "\x1B[31m"

#define U 0
#define R 1
#define D 2
#define L 3

const int DIRS[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

typedef struct {
  char goal;
  int found[1000][2];
  int area;
  int fences;
  int extant[4]; // min-x, max-x, min-y, max-y
} patch;

int in_patch(patch *p, int x, int y) {
  for (int i = 0; i < p->area; i++) {
    if (p->found[i][0] == x && p->found[i][1] == y)
      return 1;
  }
  return 0;
}

void store_in_patch(patch *p, int x, int y) {
  if (in_patch(p, x, y) == 1)
    return;
  p->found[p->area][0] = x, p->found[p->area][1] = y;
  if (p->extant[0] > x)
    p->extant[0] = x;
  if (p->extant[1] < x)
    p->extant[1] = x;
  if (p->extant[2] > y)
    p->extant[2] = y;
  if (p->extant[3] < y)
    p->extant[3] = y;
  p->area++;
}

char get_char(char *input, int rows, int cols, int x, int y) {
  if (x < 0 || x >= cols || y < 0 || y >= rows) {
    return '\0';
  }
  return input[y * (cols + 1) + x];
}

void traverse(char *input, int rows, int cols, int x, int y, patch *p) {
  store_in_patch(p, x, y);
  for (int dir = 0; dir < 4; dir++) {
    int new_x = x + DIRS[dir][0], new_y = y + DIRS[dir][1];
    if (get_char(input, rows, cols, new_x, new_y) == p->goal) {
      if (in_patch(p, new_x, new_y) == 0)
        traverse(input, rows, cols, new_x, new_y, p);
    } else {
      p->fences++;
    }
  }
}

void get_patch(char *input, int rows, int cols, uint8_t (*visited)[rows], int x,
               int y, patch *p) {
  p->extant[0] = cols;
  p->extant[2] = rows;
  traverse(input, rows, cols, x, y, p);
  for (int i = 0; i < p->area; i++) {
    visited[p->found[i][0]][p->found[i][1]] = 1;
  }
}

void print_patch(char *input, int rows, int cols, patch *p) {
  for (int e = 0; e < 4; e++) {
    printf("%d ", p->extant[e]);
  }
  printf("\n");

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      if (in_patch(p, x, y) == 1)
        printf("%s%c%s", KRED, get_char(input, rows, cols, x, y), KNRM);
      else
        printf("%s%c%s", KGRN, get_char(input, rows, cols, x, y), KNRM);
    }
    printf("\n");
  }
  printf("\n");
}

int find_straight_fences(char *input, int rows, int cols, patch *p) {
  int fences = 0;
  int direction = 0, prev_direction = 0;
  // Find fences along x direction
  for (int y = p->extant[2]; y <= p->extant[3] + 1; y++) {
    for (int x = p->extant[0]; x <= p->extant[1] + 1; x++) {
      prev_direction = direction;
      direction = in_patch(p, x, y - 1) - in_patch(p, x, y);
      if (direction != 0 && (prev_direction != direction))
        fences++;
    }
  }

  direction = 0, prev_direction = 0;
  // Find fences along y direction
  for (int x = p->extant[0]; x <= p->extant[1] + 1; x++) {
    for (int y = p->extant[2]; y <= p->extant[3] + 1; y++) {
      prev_direction = direction;
      direction = in_patch(p, x - 1, y) - in_patch(p, x, y);
      if (direction != 0 && (prev_direction != direction))
        fences++;
    }
  }
  return fences;
}

void solve(char *input, int rows, int cols) {
  int ans1 = 0, ans2 = 0;

  uint8_t(*visited)[rows] = malloc(cols * sizeof(*visited));
  if (!visited) {
    perror("Failed to allocate memory");
    exit(1);
  }
  for (int x = 0; x < cols; x++)
    for (int y = 0; y < rows; y++)
      visited[x][y] = 0; // Initialize all cells to 0x00

  for (int y = 0; y < rows; y++)
    for (int x = 0; x < cols; x++)
      if (visited[x][y] == 0) {
        patch p = {0};
        p.goal = get_char(input, rows, cols, x, y);
        get_patch(input, rows, cols, visited, x, y, &p);
        // Print the big patches for coolness!
        if (p.area > 200) {
          print_patch(input, rows, cols, &p);
        }
        ans1 += p.area * p.fences;
        int fences = find_straight_fences(input, rows, cols, &p);
        ans2 += p.area * fences;
      }

  printf("ans1 = %d\n", ans1);
  printf("ans2 = %d\n", ans2);
}

int main() {
  long l;
  char *content = read_file_to_buffer("data/day12.txt", &l);
  if (content) {
    printf("Here we go!\n%s\n", content);
    int cols = strchr(content, '\n') - content;
    int rows = l / (cols + 1);

    solve(content, rows, cols);
    free(content);
  }
  return 0;
}
