#include "../include/utils.h"
#include <pcre.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void match_all(const char *pattern, const char *subject) {
  const char *error;
  int error_offset;
  pcre *re;
  int rc;
  int ovector[30]; // Output vector to hold match

  // Compile the regex
  re = pcre_compile(pattern, 0, &error, &error_offset, NULL);
  if (!re) {
    fprintf(stderr, "Regex compilation failed at offset %d: %s\n", error_offset,
            error);
    return;
  }

  int ans1 = 0, ans2 = 0;
  int enabled = 1;

  int subject_length = strlen(subject);
  int start_offset = 0;
  while ((rc = pcre_exec(re, NULL, subject, subject_length, start_offset, 0,
                         ovector, 30)) >= 0) {
    // It's a do() or it's a don't()!
    if (rc == 2) {
      // Toggle do or don't on 'n'
      if (*(subject + ovector[2] + 2) == 'n') {
        enabled = 0;
      } else {
        enabled = 1;
      }
    }

    // It's a mul!
    if (rc == 4) {
      int a, b;
      sscanf(subject + ovector[4], "%d,%d", &a, &b);
      // Always add mul result to ans1, only to ans2 if enabled
      ans1 += a * b;
      if (enabled) {
        ans2 += a * b;
      }
    }

    // Advance start_offset to prevent infinite loops
    start_offset = ovector[1];
    if (ovector[0] == ovector[1]) { // Zero-length match safety
      start_offset++;
      if (start_offset >= subject_length)
        break;
    }
  }

  if (rc < PCRE_ERROR_NOMATCH) {
    fprintf(stderr, "Regex matching error: %d\n", rc);
  }

  printf("ans1: %d\n", ans1);
  printf("ans2: %d\n", ans2);

  pcre_free(re);
}

int compare(const void *a, const void *b) {
  return (*(int *)a - *(int *)b); // Ascending order
}

int main() {
  char *content = read_file_to_buffer("data/day3.txt");
  if (content) {
    const char *pattern = "(mul\\((\\d+),(\\d+)\\)|don't\\(\\)|do\\(\\))";
    match_all(pattern, content);
    free(content);
  }
  return 0;
}
