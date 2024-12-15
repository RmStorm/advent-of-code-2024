#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long ull;
typedef struct {
  ull val;
  ull n;
} stone;

void print_stones(stone *array, int size) {
  printf("[");
  for (int i = 0; i < size; i++) {
    printf("%llu*%llu", array[i].n, array[i].val);
    if (i < size - 1) {
      printf(", "); // Add comma between elements
    }
  }
  printf("]\n");
}

int read_stones(const char *filename, stone *stones) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Error opening file");
    exit(1);
  }

  int i = 0;
  while (fscanf(file, "%llu", &stones[i].val) == 1) {
    stones[i].n = 1;
    i++;
  }

  if (fclose(file) != 0) {
    perror("Error closing file");
    exit(1);
  }
  return i;
}

int compare(const void *a, const void *b) {
  return (((stone *)a)->val - ((stone *)b)->val);
}

int count_unique(stone *arr, int size) {
  // Sort the array
  qsort(arr, size, sizeof(stone), compare);

  // Count unique numbers
  ull current = arr[0].val;
  ull count = arr[0].n;

  int i = 0;
  for (int ii = 1; ii < size; ii++) {
    if (arr[ii].val == current) {
      count += arr[ii].n;
    } else {
      arr[i].val = current;
      arr[i].n = count;
      i++;
      current = arr[ii].val;
      count = arr[ii].n;
    }
  }
  arr[i].val = current;
  arr[i].n = count;
  i++;
  return i;
}

int step(stone *stones, int n) {
  int N = n;
  for (int i = 0; i < n; i++) {
    // keep track of the number on the stone..
    ull s = stones[i].val;
    // Count the digits in the stone
    char buffer[32];
    sprintf(buffer, "%llu", s);
    int d = strlen(buffer);
    // Choose the correct mutation
    if (s == 0) {
      stones[i].val = 1;
    } else if (d % 2 == 0) {
      stones[N] = stones[i];
      sscanf(buffer + d / 2, "%llu", &stones[i].val);
      buffer[d / 2] = '\0', sscanf(buffer, "%llu", &stones[N].val);
      N++;
    } else {
      stones[i].val = s * 2024;
    }
  }
  return count_unique(stones, N);
}

int main() {
  stone stones[100000];

  int i = read_stones("data/day11.txt", stones);
  for (int ii = 0; ii < 75; ii++) {
    i = step(stones, i);
    printf("at iter: %d len is %d\n", ii, i);
    // print_stones(stones, i);
  }
  ull ans = 0;
  for (int ii = 0; ii < i; ii++) {
    ans += stones[ii].n;
  }
  printf("ans: %llu", ans);
}
