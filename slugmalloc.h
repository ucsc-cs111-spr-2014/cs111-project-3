/* CREATED 5/21/2014
 * Header file to create two structs, slug_mem_t and slug_mem, and a type bool.
 * We also make macros for malloc(s) and free(s) in order to overwrite the
 * existing functionality of malloc(s) and free(s).
 */

#ifndef SLUGMALLOC_H
    #define SLUGMALLOC_H
    #include <time.h>
    
/* Creates a type bool that is used to indicate if a malloc is in use or if it
 * has been freed.
 */
    typedef enum { used=0, freed=1 } bool;

/* struct slug_mem_t:
 *   void   *addr       - stores the address where current malloc'd memory is stored
 *   char   *file       - name of file that called malloc
 *   char   *line       - line number where malloc was called
 *   time_t timestamp   - time that malloc was called
 *   int    flags       - indicates whether a malloc has been freed or not
 *   size_t size        - the size of the malloc
 */
    typedef struct {
        void *addr;
        char *file;
        char *line;
        time_t timestamp;
        int flags;
        size_t size;
    } slug_mem_t;

/* struct slug_mem:
 *   int size           - the total number of items that has been malloc'd
 *   int max_size       - maximum size of the array, is dynamically reallocated
 *   slug_mem_t *allocs - stores all current and freed allocations
 */
    typedef struct {
        int size;
        int max_size;
        slug_mem_t *allocs;
    } slug_mem;

    /* A slug_mem struct that contains an array of slug_mem_t 
     * that is accessed and modified by slugmalloc.c 
     * Used to keep track of all allocations occured by 
     * programs that include slugmalloc.h
     */
    slug_mem SLUG_MEM;

    /* Used for initialization purposes, 
     * ie: if null we init SLUG_MEM and point pntr to it.
     */
    slug_mem *pntr;

    #ifndef SLUGMALLOC_C /* ie: any file except slugmalloc.c */

        #include <stdlib.h>
        #include <stdio.h>
        #include <string.h>

        /* Everything below was given in the program prompt. */

        #define FUNCTIONIZE(a, b) a(b)
        #define STRINGIZE(a) #a
        #define INT2STRING(i) FUNCTIONIZE(STRINGIZE, i)
        #define FILE_POS __FILE__ "|" INT2STRING(__LINE__)

        #define malloc(s) slug_malloc((s), FILE_POS)
        #define free(s) slug_free((s), FILE_POS)

    #endif

#endif


