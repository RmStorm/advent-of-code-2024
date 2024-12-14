#include "../include/utils.h"
#include <gmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_NUMS 15

void check_ans1(mpz_t *nums, int len, mpz_t cur, mpz_t goal, int *x) {
  if ((*x == 1) || (mpz_cmp(cur, goal) > 0))
    return;
  if (len == 0) {
    if (mpz_cmp(cur, goal) == 0)
      *x = 1;
  } else {
    mpz_t acc;
    mpz_init(acc);

    mpz_add(acc, cur, nums[0]);
    check_ans1(nums + 1, len - 1, acc, goal, x);

    mpz_mul(acc, cur, nums[0]);
    check_ans1(nums + 1, len - 1, acc, goal, x);
  }
}

void check_ans2(mpz_t *nums, int len, mpz_t cur, mpz_t goal, int *x) {
  if ((*x == 1) || (mpz_cmp(cur, goal) > 0))
    return;
  if (len == 0) {
    if (mpz_cmp(cur, goal) == 0)
      *x = 1;
  } else {
    mpz_t acc;
    mpz_init(acc);

    mpz_add(acc, cur, nums[0]);
    check_ans2(nums + 1, len - 1, acc, goal, x);

    mpz_mul(acc, cur, nums[0]);
    check_ans2(nums + 1, len - 1, acc, goal, x);

    char concat[100];
    gmp_snprintf(concat, sizeof(concat), "%Zd%Zd", cur, nums[0]);
    mpz_set_str(acc, concat, 10);
    check_ans2(nums + 1, len - 1, acc, goal, x);
  }
}

int main() {
  FILE *file = fopen("data/day7.txt", "r");
  if (!file) {
    perror("Error opening file");
    return 1;
  }

  char line[MAX_LINE_LENGTH];
  // Initialize all the bigints we'll need
  mpz_t ans1, ans2;
  mpz_init(ans1);
  mpz_init(ans2);
  mpz_t nums[MAX_NUMS];
  for (int i = 0; i < MAX_NUMS; i++) {
    mpz_init(nums[i]);
  }
  mpz_t acc; // accumulator for recursive func!
  mpz_init(acc);

  // Start parsing
  while (fgets(line, sizeof(line), file)) {
    char *colon_pos = strchr(line, ':');
    *colon_pos = ' ';

    int i = 0;
    char *cursor = line;
    while (gmp_sscanf(cursor, "%Zd", nums[i]) == 1) {
      cursor += gmp_snprintf(NULL, 0, "%Zd", nums[i]);
      while (*cursor == ' ')
        cursor++;
      i++;
    }

    int x = 0;
    mpz_set(acc, nums[1]);
    check_ans1(nums + 2, i - 2, acc, nums[0], &x);

    if (x == 1) {
      mpz_add(ans1, ans1, nums[0]);
    }
    x = 0;
    mpz_set(acc, nums[1]);
    check_ans2(nums + 2, i - 2, acc, nums[0], &x);

    if (x == 1) {
      mpz_add(ans2, ans2, nums[0]);
    }
  }

  if (fclose(file) != 0) {
    perror("Error closing file");
  }

  gmp_printf("ans1: %Zd\n", ans1);
  gmp_printf("ans2: %Zd\n", ans2);

  return 0;
}
