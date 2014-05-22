/* CREATED 5/21/2014 
 * File used for testing free and malloc.
 * It purposely contains bugs to test our error handling and slug_memstats().
*/

#include "slugmalloc.h"
#include <assert.h>

#define NUM_MALLOCS 1024

/*set this to 1 to test slug_malloc with a 0 size allocation*/
#define ZERO_ALLOC 0

/*set this to 1 to test slug_malloc with a big size*/
#define BIG_ALLOC 0

/*set this to 1 to test slug_free's error checking*/
#define FREE_ERROR_TESTING 0

/* main()
 * Purpose: Calls test malloc and formats the prints to make it easy to read.
 * Return Value: Returns 0 at the end of the function
 */
int main(int argc, char *argv[]) {
    int i; int *ptr;
    for (i = 0; i < 16; i++) {printf("=");}
    printf("\n");
    
    for (i = 0; i < NUM_MALLOCS; i++) {
        if (i % 2 == 0) {
            test_malloc_int(i);
        } else {
            test_malloc_char(i);
        }
        
    }

    for (i = 0; i < 16; i++) {printf("=");}
    printf("\n");

    #if ZERO_ALLOC
        ptr = (int *) malloc(0);
    #endif

    #if BIG_ALLOC
        ptr = (int *) malloc(9999*9999*9999*9999);
    #endif

    #if FREE_ERROR_TESTING
        ptr = (int *) malloc(sizeof(int));
        *ptr = 13;
        printf("ptr:%d\n", *ptr);
        free(ptr);
        free(ptr);
    #endif

    return 0;
}

/* test_malloc_*()
 * Purpose: 
 *      Function that points ptr to a malloc,
 *      initalizes it to init,
 *      prints it out,
 *      and free's every other ptr.
 * Parameters: 
 *      Takes in an int init which is the value we initialize ptr to.
 * Assumptions: 
 *      init is a number from 0-inf,
 *      test_malloc_int takes in even ints,
 *      test_malloc_char takes in odd ints.
 */
void test_malloc_int(int init) {
    int *ptr = (int *) malloc(sizeof(int));
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
