#include <stdio.h>
#include <stdlib.h>
#include "../include/utils.h"

#define INPUT_SIZE 1000

void read_file_formatted(const char *filename, int *lh, int *rh) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    int i = 0, a, b;
    while (fscanf(file, "%d %d", &a, &b) == 2) {
        lh[i] = a;
        rh[i] = b;
        i++;
    }

    fclose(file);
}

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b); // Ascending order
}

// int main(int argc, char **argv)
int main()
{
    int lh_list[INPUT_SIZE];
    int rh_list[INPUT_SIZE];

    read_file_formatted("data/day1.txt", lh_list, rh_list);
    // print_array(lh_list, INPUT_SIZE);

    qsort(lh_list, INPUT_SIZE, sizeof(int), compare);
    qsort(rh_list, INPUT_SIZE, sizeof(int), compare);
    
    int ans1 = 0, i;
    for (i = 0; i<INPUT_SIZE;i++ ) {
        ans1 += abs(lh_list[i]-rh_list[i]);
    }
    printf("ans1: %d\n", ans1);

    int ans2 = 0, ii;
    for (i = 0; i<INPUT_SIZE;i++ ) {
        for (ii = 0; ii<INPUT_SIZE;ii++ ) {
            if (lh_list[i]==rh_list[ii]) {
                ans2 += lh_list[i];
            }
        }
    }
    printf("ans1: %d\n", ans2);
    
    return 0;
}
