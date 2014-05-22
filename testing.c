/* CREATED 5/21/2014 
 * File used for testing free and malloc.
 * It purposely contains bugs to test our error handling and slug_memstats().
*/

#include "slugmalloc.h"
#include <assert.h>

/* This number is the number of malloc calls will be executed
 * NOTE: may not be the same as the number of frees
 *      ie: there may be fewer frees.
 *      (see: PARTIAL_FREE)
 */
#define NUM_MALLOCS 8

/* Set this to 0 to not perform main test loop */
#define MAIN_TEST 1

/* Set this to 0 if you only want to test
 * using test_malloc_int()
 */
#define ALTERNATE_MALLOC_TYPE 1

/* Set this to 1 if you want to only free part (half) of the mallocs,
 * or 0 to free everything malloc'ed in the first for loop.
 */
#define PARTIAL_FREE 0

/* Set this to 1 to test slug_malloc with a 0 size allocation */
#define ZERO_ALLOC 0

/* Set this to 1 to test slug_malloc with a big size */
#define BIG_ALLOC 0

/* Set this to 1 to test slug_free with a double free call*/
#define DOUBLE_FREE 0

/* Set this to 1 to test slug_free's handling of a call to a free block
 * that's not the first byte of the malloc'ed memory */
#define WRONG_BYTE_FREE 1

#define TAG "[testing.c]: "

void test_malloc_int(int init);
void test_malloc_char(int init);

/* main()
 * Purpose: 
 *      Does malloc testing 
 *      through test_malloc_*()
 * Return Value: 
 *      Returns 0 at the end of the function,
 *      unless an assert failed.
 */
int main(int argc, char *argv[]) {
    int i; int *ptr;
    for (i = 0; i < 16; i++) {printf("=");}
    printf("\n");
    
    #if MAIN_TEST
    for (i = 0; i < NUM_MALLOCS; i++) {
        #if ALTERNATE_MALLOC_TYPE
        if (i % 2 == 0) {
        #endif
            test_malloc_int(i);
        #if ALTERNATE_MALLOC_TYPE    
        } else {
            test_malloc_char(i);
        }
        #endif
    }
    #endif

    #if ZERO_ALLOC
        ptr = (int *) malloc(0);
        /* should fail and terminate the program */
    #endif

    #if BIG_ALLOC
        ptr = (int *) malloc(128*1024*1024+1);/*ie: size > 128MiB*/
        /* should fail and terminate the program */
    #endif

    #if DOUBLE_FREE
        ptr = (int *) malloc(sizeof(int));
        *ptr = 13; /* test number, please ignore */
        printf(TAG "ptr:%d\n", *ptr);
        free(ptr);
        free(ptr);
        /* should fail and terminate the program */
    #endif

    #if WRONG_BYTE_FREE
        ptr = (int *) malloc(sizeof(int));
        *ptr = 13; /* test number, please ignore */
        printf(TAG "ptr:%d\n", *ptr);
        free(ptr+1);
        /* should fail and terminate the program */
    #endif

    for (i = 0; i < 16; i++) {printf("=");}
    printf("\n");
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
    printf(TAG "ptr:%d\n", *ptr);

    #if PARTIAL_FREE
    if (init % 4 == 0) {
    #endif
        free(ptr);
    #if PARTIAL_FREE
    }
    #endif
}

void test_malloc_char(int init) {
    char *ptr = (char *) malloc(sizeof(char));
    assert(ptr != NULL);

    *ptr = (char)init;
    printf(TAG "ptr:%s\n", ptr);

    #if PARTIAL_FREE
    if (init % 4 == 1) {
    #endif
        free(ptr);
    #if PARTIAL_FREE
    }
    #endif
}
