#include "slugmalloc.h"

int main(int argc, char *argv[]) {
    int *ptr_one, i;

    for (i = 0; i < 8; i++) {
        ptr_one = (int *) malloc(sizeof(int));

        if (ptr_one == NULL) {
            printf("ERROR: Out of memory\n");
            return 1;
        }

        *ptr_one = i;
        printf("%d\n", *ptr_one);

        free(ptr_one);
    }

    return 0;
}