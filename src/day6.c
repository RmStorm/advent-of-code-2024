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
const char DIR_CHARS[4] = {'^', '>', 'V', '<'};
const uint8_t DIR_MASKS[4] = {1, 2, 4, 8};

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

void move_cursor(char *input, int rows, int cols, int *cur_x, int *cur_y,
                 int next_x, int next_y, char cur) {
  // Mark current position as visited
  set_char(input, rows, cols, *cur_x, *cur_y, 'X');
  // Move to next position
  *cur_x = next_x;
  *cur_y = next_y;
  // Set the direction character at the new position
  set_char(input, rows, cols, *cur_x, *cur_y, cur);
}

int check_loop(char *input, int rows, int cols, int x, int y, int dir,
               int block_x, int block_y) {
  // Place the obstacle
  char temp_block = get_char(input, rows, cols, block_x, block_y);
  if (temp_block == '#' || temp_block == '\0') {
    return 0;
  }
  set_char(input, rows, cols, block_x, block_y, '#');

  // Alllocate an array for holding visited states
  uint8_t(*visited)[rows] = malloc(cols * sizeof(*visited));
  if (!visited) {
    perror("Failed to allocate memory");
    exit(1);
  }
  for (int x = 0; x < cols; x++) {
    for (int y = 0; y < rows; y++) {
      visited[x][y] = 0; // Initialize all cells to 0x00 (no directions visited)
    }
  }

  // Start traversing, at some point we either exit or revisit a pos!
  while (1 == 1) {
    if ((visited[x][y] & DIR_MASKS[dir]) != 0) {
      free(visited);
      set_char(input, rows, cols, block_x, block_y, temp_block);
      return 1;
    }
    visited[x][y] |= DIR_MASKS[dir];

    char next = get_char(input, rows, cols, x + DIRS[dir][0], y + DIRS[dir][1]);
    if (next == '\0') {
      free(visited);
      set_char(input, rows, cols, block_x, block_y, temp_block);
      return 0;
    }
    if (next == '#') {
      dir = (dir + 1) % 4;
      continue;
    }
    x += DIRS[dir][0];
    y += DIRS[dir][1];
  }
  fprintf(stderr, "WAT err(42)\n");
  exit(1);
}

void solve(char *input, int rows, int cols) {
  // First find the start
  int start_x = -1, start_y = -1;
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      char c = get_char(input, rows, cols, x, y);
      if (c == '^') {
        start_x = x;
        start_y = y;
        goto found_start;
      }
    }
  }
found_start:
  if (start_x == -1 || start_y == -1) {
    fprintf(stderr, "Could not find start, erroring out\n");
    exit(1);
  } else {
    printf("Found start at [%d, %d]\n", start_x, start_y);
  }

  int cur_x = start_x, cur_y = start_y;
  int ans1 = 1, ans2 = 0;
  int traversing = 1, next_x, next_y;
  while (traversing == 1) {
    char cur = get_char(input, rows, cols, cur_x, cur_y);
    int dir;

    if (cur == '^') {
      dir = U;
    } else if (cur == '>') {
      dir = R;
    } else if (cur == 'V') {
      dir = D;
    } else if (cur == '<') {
      dir = L;
    } else {
      fprintf(stderr, "Unexpected direction character '%c'\n", cur);
      exit(1);
    }
    next_x = cur_x + DIRS[dir][0];
    next_y = cur_y + DIRS[dir][1];

    char next = get_char(input, rows, cols, next_x, next_y);

    if (next == '\0') {
      // Stop when exiting map
      traversing = 0;
    } else if (next == '.') {
      // Uncharted territory for the gaurd! Add a square
      ans1 += 1;
      move_cursor(input, rows, cols, &cur_x, &cur_y, next_x, next_y, cur);
    } else if (next == 'X') {
      // Already charted territory for the gaurd.. proceed
      move_cursor(input, rows, cols, &cur_x, &cur_y, next_x, next_y, cur);
    } else if (next == '#') {
      // Tuuuuurn to the right!
      set_char(input, rows, cols, cur_x, cur_y, DIR_CHARS[(dir + 1) % 4]);
    } else {
      fprintf(stderr, "Unexpected character '%c' at next position\n", next);
      exit(1);
    }
  }
  for (int x = 0; x < cols; x++) {
    for (int y = 0; y < rows; y++) {
      if (x == start_x && y == start_y)
        continue;
      ans2 += check_loop(input, rows, cols, start_x, start_y, 0, x, y);
    }
  }
  printf("%s%d\n\n", input, ans1);
  printf("ans2 = %d\n", ans2);
}

int main() {
  long l;
  char *content = read_file_to_buffer("data/day6.txt", &l);
  if (content) {
    printf("Here we go!\n%s\n", content);
    int cols = strchr(content, '\n') - content;
    int rows = l / (cols + 1);

    solve(content, rows, cols);
    free(content);
  }
  return 0;
}
