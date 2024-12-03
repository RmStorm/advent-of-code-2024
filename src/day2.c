#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 40

int check_valid(int *nums, int len, int skip) {
  // initialize prev as -1 since there are no negative levels
  // initialize dir as -2 since it should be -1 or 1
  int i, valid = 1, cur, prev = -1, dir = -2;
  for (i = 0; i < len; i++) {
    if (skip == i) {
      continue;
    }
    cur = nums[i];
    if (prev != -1) {
      int diff = prev - cur;
      int cur_dir = (diff == abs(diff)) ? 1 : -1;
      if (dir == -2) {
        dir = cur_dir;
      } else if (dir != cur_dir) {
        valid = 0;
        break;
      }
      if (abs(diff) != 1 && abs(diff) != 2 && abs(diff) != 3) {
        valid = 0;
        break;
      }
    }
    prev = cur;
  }
  return valid;
}

int main() {
  FILE *file = fopen("data/day2.txt", "r");
  if (!file) {
    perror("Error opening file");
    return 1;
  }

  char line[MAX_LINE_LENGTH];
  int ans1 = 0, ans2 = 0;

  while (fgets(line, sizeof(line), file)) {
    int i = 0;
    int nums[10];
    char *cursor = line;

    while (sscanf(cursor, "%d", &nums[i]) == 1) {
      cursor += snprintf(NULL, 0, "%d ", nums[i]);
      i++;
    }
    int valid = check_valid(nums, i, -1);

    ans1 += valid;
    ans2 += valid;

    if (valid == 0) {
      for (int j = 0; j < i; j++) {
        if (check_valid(nums, i, j) == 1) {
          ans2 += 1;
          break;
        }
      }
    }
  }

  if (fclose(file) != 0) {
    perror("Error closing file");
  }

  printf("ans1: %d\n", ans1);
  printf("ans2: %d\n", ans2);

  return 0;
}
