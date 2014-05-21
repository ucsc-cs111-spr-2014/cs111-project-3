/* CREATED 5/21/2014 
 * File used for testing free and malloc.
 * It purposely contains bugs to test our error handling and slug_memstats().
*/

#include "slugmalloc.h"
#include <assert.h>

#define NUM_MALLOCS 8

/* main()
 * Purpose: Calls test malloc and formats the prints to make it easy to read.
 * Return Value: Returns 0 at the end of the function
*/
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

/* test_malloc()
 * Purpose: Function that contains buggy code to attempt to crash our slug_malloc()
 * and slug_free().
 * Return Value: Returns 0 at the end of the function.
 * Parameters: Takes in an int init which is the value we initialize ptr to.
 * Assumptions: Assumes init is an int (we don't check for it before hand)
*/
 
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
