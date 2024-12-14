#include "../include/utils.h"
#include <gmp.h>
#include <pcre.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_BLOCK_DSCR = 300000;

typedef struct {
  int len;
  int id;
} bd; // block_descriptor

void print_blocks(bd arr[MAX_BLOCK_DSCR], int n) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < arr[i].len; j++)
      if (arr[i].id == -1)
        printf(".");
      else
        printf("%d", arr[i].id);
  printf("\n");
}

void print_block_descriptors(bd arr[MAX_BLOCK_DSCR], int n) {
  for (int i = 0; i < n; i++)
    printf("%d", arr[i].len);
  printf("\n");
  for (int i = 0; i < n; i++)
    if (arr[i].id == -1)
      printf(".");
    else
      printf("%d", arr[i].id);
  printf("\n");
}

int fragment_disk_ans1(bd arr[MAX_BLOCK_DSCR], int i, int first_empty) {
  for (int j = i - 1; j >= 0; j--) {
    if (arr[j].id == -1) {
      continue;
    }
    if (j <= first_empty) {
      break;
    }
    while (1 == 1) {
      if (arr[j].len < arr[first_empty].len) {
        // This branch sucks, we must split a block descriptor in 2!
        // Costly operation, must shift all block descriptors to the right..
        // j++; // increment j since the index will be moved to the right by
        // one..
        for (int k = i; k > first_empty; k--) {
          arr[k + 1] = arr[k];
        }
        // First correctly set the newly created empty block
        arr[first_empty + 1].len = arr[first_empty].len - arr[j + 1].len;
        arr[first_empty + 1].id = -1;
        // Then fill up the originial free block
        arr[first_empty].len = arr[j + 1].len;
        arr[first_empty].id = arr[j + 1].id;
        first_empty++;
        if (i > first_empty)
          i--;
        break;
      } else if (arr[j].len == arr[first_empty].len) {
        arr[first_empty].id = arr[j].id;
        first_empty += 2;
        if (i > first_empty) {
          i -= 2;
          j--;
        }
        break;
      } else if (arr[j].len > arr[first_empty].len) {
        arr[first_empty].id = arr[j].id;
        arr[j].len -= arr[first_empty].len;
        first_empty += 2;
      }
    }
  }
  return i;
}

int fragment_disk_ans2(bd arr[MAX_BLOCK_DSCR], int i, int first_empty) {
  for (int j = i - 1; j >= 0; j--) {
    if (arr[j].id == -1) {
      continue;
    }
    if (j <= first_empty) {
      break;
    }
    for (int e = first_empty; e < j; e++) {
      if (arr[e].id != -1)
        continue;
      if (arr[j].len < arr[e].len) {
        // This branch sucks, we must split a block descriptor in 2!
        // Costly operation, must shift all block descriptors to the right..
        i++, j++;
        // one..
        for (int k = i; k > e; k--) {
          arr[k + 1] = arr[k];
        }
        // First correctly set the newly created empty block
        arr[e + 1].len = arr[e].len - arr[j].len;
        arr[e + 1].id = -1;
        // Then fill up the originial free block
        arr[e].len = arr[j].len;
        arr[e].id = arr[j].id;
        arr[j].id = -1;
        if (e == first_empty)
          first_empty++;
        break;
      } else if (arr[j].len == arr[e].len) {
        arr[e].id = arr[j].id;
        if (e == first_empty)
          first_empty++;
        arr[j].id = -1;
        break;
      }
    }
  }
  return i;
}

void check_sum(bd arr[MAX_BLOCK_DSCR], int n, mpz_t result) {
  mpz_t cur_pos, tmp;
  mpz_init_set_ui(result, 0);
  mpz_init_set_ui(cur_pos, 0);
  mpz_init(tmp);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < arr[i].len; j++) {
      if (arr[i].id != -1) {
        mpz_mul_ui(tmp, cur_pos, arr[i].id);
        mpz_add(result, result, tmp);
      }
      mpz_add_ui(cur_pos, cur_pos, 1);
    }
  }

  mpz_clear(cur_pos);
  mpz_clear(tmp);
}

int main() {
  char *content = read_file_to_buffer("data/day9.txt", NULL);
  if (content) {
    // int ans1 = 0, ans2 = 0;
    bd arr[MAX_BLOCK_DSCR];

    // Fill the disklayout into the block_descriptors array
    int n = 0, i = 0, first_empty = -1;
    for (i = 0; i < strlen(content) - 1; i++) {
      arr[i].len = content[i] - '0';
      if (i % 2 == 0) {
        arr[i].id = n;
        n++;
      } else {
        if (first_empty == -1)
          first_empty = i;
        arr[i].id = -1;
      }
    }

    i = fragment_disk_ans2(arr, i, first_empty);

    mpz_t ans;
    mpz_init_set_ui(ans, 0);
    check_sum(arr, i, ans);
    gmp_printf("ans: %Zd", ans);

    free(content);
  }
  return 0;
}
