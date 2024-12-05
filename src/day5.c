#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

#define RULES 21
#define UPDATES 6
#define BEFORE 0
#define AFTER 1

void insert_rule(int rules[][2][101], int before, int after) {
  int cur_before_len = rules[before][AFTER][0];
  int cur_after_len = rules[after][BEFORE][0];
  rules[before][AFTER][cur_before_len + 1] = after;
  rules[after][BEFORE][cur_after_len + 1] = before;
  rules[before][AFTER][0]++;
  rules[after][BEFORE][0]++;
}

void print_rules(int rules[][2][101]) {
  for (int i = 0; i < 100; i++) {
    for (int k = 0; k < rules[i][BEFORE][0]; k++) {
      printf("I [%d] want to be after = %d\n", i, rules[i][BEFORE][k + 1]);
    }
    for (int k = 0; k < rules[i][AFTER][0]; k++) {
      printf("I [%d] want to be before [%d] \n", i, rules[i][AFTER][k + 1]);
    }
  }
}

int check_not_before(int rules[][2][101], int cur, int after) {
  // printf("%d must not be before %d\n", cur, after);
  for (int k = 0; k < rules[cur][BEFORE][0]; k++) {
    // printf("I [%d] want to be after = %d\n", cur, rules[cur][BEFORE][k + 1]);
    if (rules[cur][BEFORE][k + 1] == after) {
      return 0;
    }
  }
  return 1;
}

int check_valid(int rules[][2][101], int *nums, int len) {
  int i, valid = 1, cur;
  for (i = 0; i < len; i++) {
    // printf("checking not before rules for %d\n", nums[i]);
    for (int j = i + 1; j < len; j++) {
      if (check_not_before(rules, nums[i], nums[j]) == 0) {
        // printf("NOT VALID, found %d before  %d\n", nums[i], nums[j]);
        return 0;
      }
    }
  }
  return 1;
}

// for every number store an array with the numbers that must be before [0] it
// and after [1] it Use the first number in the before and after array to keep
// track of their actualy length
int rules[100][2][101]; // Declare globally for access in compare

int compare(const void *a, const void *b) {
  int x = *(int *)a;
  int y = *(int *)b;
  if (check_not_before(rules, x, y) == 0) {
    return 1;
  }
  return -1;
}

int main() {
  for (int i = 0; i < 100; i++) {
    rules[i][0][0] = 0;
    rules[i][1][0] = 0;
  }

  FILE *file = fopen("data/day5.txt", "r");
  if (!file) {
    perror("Error opening file");
    exit(1);
  }

  int before, after;
  while (fscanf(file, "%d|%d", &before, &after) == 2) {
    printf("testing please: %d, %d\n", before, after);
    insert_rule(rules, before, after);
  }

  // The final failed fscanf consumed a %d so rewind two chars
  fseek(file, -2, SEEK_CUR);

  char line[100];
  int ans1 = 0, ans2 = 0;
  while (fgets(line, sizeof(line), file)) {
    int i = 0;
    int nums[40]; // There's never more numbers then like 30?
    char *cursor = line;

    while (sscanf(cursor, "%d", &nums[i]) == 1) {
      cursor += snprintf(NULL, 0, "%d ", nums[i]);
      i++;
    }

    if (check_valid(rules, nums, i) == 1) {
      // print_array(nums, i);
      ans1 += nums[(i) / 2];
    } else {
      qsort(nums, i, sizeof(int), compare);
      ans2 += nums[(i) / 2];
    }
  }

  if (fclose(file) != 0) {
    perror("Error closing file");
  }
  printf("\nans1: %d\n", ans1);
  printf("ans2: %d\n", ans2);
  return 0;
}
