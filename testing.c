/* CREATED 5/21/2014 
 * File used for testing free and malloc.
 * It purposely contains bugs to test our error handling and slug_memstats().
*/

#include "slugmalloc.h"
#include <assert.h>

/* main()
 * Purpose: Calls test malloc and formats the prints to make it easy to read.
 * Return Value: Returns 0 at the end of the function
*/

int main(int argc, char *argv[]) {
    int i;
    for (i = 0; i < 64; i++) {printf("=");}
    printf("\n");
    
    test_malloc(0);

    for (i = 0; i < 64; i++) {printf("=");}
    printf("\n");
    return 0;
}

/* test_malloc()
 * Purpose: Function that contains buggy code to attempt to crash our slug_malloc()
 * and slug_free().
 * Return Value: Returns 0 at the end of the function.
 * Parameters: Takes in an int init which is the value we initialize ptr to.
 * Assumptions: Assumes init is an int (we don't check for it before hand)
*/

int test_malloc(int init) {
    int *ptr = (int *) malloc(sizeof(int));
    assert(ptr != NULL);

    *ptr = init;
    printf("ptr:%d\n", *ptr);

    return 0;
}
