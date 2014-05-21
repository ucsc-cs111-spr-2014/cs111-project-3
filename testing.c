#include "slugmalloc.h"
#include <assert.h>

#define NUM_MALLOCS 8

int main(int argc, char *argv[]) {
    int i;
    for (i = 0; i < 64; i++) {printf("=");}
    printf("\n");
    
    for (i = 0; i < NUM_MALLOCS; i++) {
        if (i % 2 == 0) {
            test_malloc_int(i);
        } else {
            test_malloc_char(i);
        }
        
    }

    for (i = 0; i < 64; i++) {printf("=");}
    printf("\n");
    return 0;
}

void test_malloc_int(int init) {
    int *ptr = (int *) malloc(sizeof(int *));
    assert(ptr != NULL);

    *ptr = init;
    printf("ptr:%d\n", *ptr);

    if (init % 4 == 0) {
        free(ptr);
    }
}

void test_malloc_char(int init) {
    char *ptr = (char *) malloc(sizeof(char));
    assert(ptr != NULL);

    *ptr = (char)init;
    printf("ptr:%s\n", ptr);

    if (init % 4 == 1) {
        free(ptr);
    }
}
