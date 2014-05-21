#include "slugmalloc.h"
#include <assert.h>

int main(int argc, char *argv[]) {
    int i;
    for (i = 0; i < 64; i++) {printf("=");}
    printf("\n");
    
    test_malloc(0);

    for (i = 0; i < 64; i++) {printf("=");}
    printf("\n");
    return 0;
}

int test_malloc(int init) {
    int *ptr = (int *) malloc(sizeof(int));
    assert(ptr != NULL);

    *ptr = init;
    printf("ptr:%d\n", *ptr);

	return 0;
}
