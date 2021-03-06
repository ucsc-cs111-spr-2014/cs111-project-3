#define __SLUGMALLOC_C__
#include "slugmalloc.h"

#define TAG "CS-111"

/* Set this to 1 to print extra debug information */
#define DEBUG_PRINT 0

/* buffy - used for printing 
 * gets incr/decr on function entry/exit
 */
char buffy[128];

/*
                  ##     ##    ###    ##       ##        #######   ######  
 ##   ##          ###   ###   ## ##   ##       ##       ##     ## ##    ## 
  ## ##           #### ####  ##   ##  ##       ##       ##     ## ##       
#########         ## ### ## ##     ## ##       ##       ##     ## ##       
  ## ##           ##     ## ######### ##       ##       ##     ## ##       
 ##   ##          ##     ## ##     ## ##       ##       ##     ## ##    ## 
          ####### ##     ## ##     ## ######## ########  #######   ######  
*/
/* slug_malloc():
 * Purpose: To malloc something of size size 
 *          and to keep track of extra information
 *          (see add_slug_mem)
 * Parameters:  size     - the size of memory to be malloc'd
 *              FILE_POS - Contains information about the malloc
 *                  ie: file and line (see slugmalloc.h @ "#define FILE_POS")
 */
void *slug_malloc(size_t size, char *FILE_POS) 
{
    void *ptr;
    int i;

    #if DEBUG_PRINT
        printf("%s", buffy);
        printf("%s%s%s%s:%s\n", ">>>", TAG, "-", FILE_POS, "slug_malloc");
        strcat(buffy, "\t"); 
    #endif
    
    /* if SLUG_MALLOC is not initialized */
    if (pntr == NULL) {
        SLUG_MEM.size = 0;
        SLUG_MEM.max_size = 8;
        SLUG_MEM.allocs = calloc(SLUG_MEM.max_size, sizeof(slug_mem_t));
        for (i = 0; i < SLUG_MEM.max_size; i++){
            SLUG_MEM.allocs[i].size = -1;
        }
        pntr = &SLUG_MEM;
        atexit(slug_atexit);
    }

    /*ERROR CHECKING*/
    if (size == 0) {
        fprintf(stderr, "%s::%s %lu\n", FILE_POS,
            "Error, allocation size was:", size);
        return;
    } else if (size > (128 * pow(2, 20)) ) {/*128Mi*/
        fprintf(stderr, "%s::%s, size was: %lu\n", FILE_POS,
            "Error, allocation size was too large", size);
        slug_atexit();
        abort();
    } /*END CHECKING*/

    ptr = malloc(size);
    add_slug_mem(ptr, FILE_POS, size);

    #if DEBUG_PRINT
        *strrchr(buffy, '\t') = (char)0;
        printf("%s", buffy);printf("%s:%s\n", "<<<" TAG, "slug_malloc");
    #endif
    return ptr;
}

/* within_bounds():
 * Purpose: takes a pointer an a slug_mem_t element
 *          and returns if the pointer is within the elements allocated memory
 *          given by elem.addr <-> elem.addr + elem.size*8
 * Assumptions: 
 *      elem is a valid slug_mem_t element.
 */
int within_bounds(void *ptr, slug_mem_t elem) {
    assert(ptr != NULL);

    return ( ptr > elem.addr 
            && ptr <= (void *)((int)elem.addr + elem.size*8) );
}

/*
                  ######## ########  ######## ######## 
 ##   ##          ##       ##     ## ##       ##       
  ## ##           ##       ##     ## ##       ##       
#########         ######   ########  ######   ######   
  ## ##           ##       ##   ##   ##       ##       
 ##   ##          ##       ##    ##  ##       ##       
          ####### ##       ##     ## ######## ######## 
*/
/* slug_free():
 * Purpose: To free memory at ptr and modify the flag corresponding to ptr.
 *          Also used to catch some possible bugs, such as:
 *              freeing memory already freed,
 *              and freeing address that have not been malloc'd.
 * Parameters: ptr      - points to address we want to free
 *             FILE_POS - contains information about the malloc
 */
void slug_free(void *ptr, char *FILE_POS) 
{
    int i; 
    int found_freed_match;
    found_freed_match = 0;
    i = 0;

    #if DEBUG_PRINT
        printf("%s", buffy);
        printf("%s%s%s%s:%s\n", ">>>", TAG, "-", FILE_POS, "slug_free");
        strcat(buffy, "\t"); 
    #endif

    for(i = 0; i < SLUG_MEM.size; i++) {
        if (SLUG_MEM.allocs[i].size != -1) {
            if(SLUG_MEM.allocs[i].addr == ptr) {
                if (SLUG_MEM.allocs[i].flags == used) {
                    SLUG_MEM.allocs[i].flags = freed;
                    free(ptr);
                    #if DEBUG_PRINT
                        print_slug_mem_t(SLUG_MEM.allocs[i], "free");
                    #endif
                    break;
                } else {
                    found_freed_match = 1;
                }
            } /* wasn't an exact match, 
               * so check if it's within the bounds of the curr elem */
            else if (within_bounds(ptr, SLUG_MEM.allocs[i])) {
                fprintf(stderr, 
                    "\nError @ %s: %s,\n%s%p-%p, %s%p\n%s\n\n", 
                    FILE_POS,
                    "tried to free a block of "
                    "memory with incorrect pointer", 
                    "block was: ", SLUG_MEM.allocs[i].addr, 
                    (int)SLUG_MEM.allocs[i].addr + SLUG_MEM.allocs[i].size*8,
                    "received: ", ptr,
                    "ABORTING!");
                slug_atexit();
                abort();
            }
        }
    }
    /*ERROR CHECKING*/
    if (i == SLUG_MEM.size) {
        if (found_freed_match) {
            fprintf(stderr, "\n%s::%s\n\n", FILE_POS,
                "Error: tried to free memory already freed");
            slug_atexit();
            abort();
        } else {
            fprintf(stderr, "\n%s::%s\n\n", FILE_POS,
                "Error: tried to free non-malloc'ed memory");
            slug_atexit();
            abort();
        }
    }/*END CHECKING*/

    #if DEBUG_PRINT
        *strrchr(buffy, '\t') = (char)0;
        printf("%s", buffy);printf("%s:%s\n", "<<<" TAG, "slug_free\n");
    #endif
}

/*
   ###    ########  ########                            ##     ## ######## ##     ## 
  ## ##   ##     ## ##     ##          ##   ##          ###   ### ##       ###   ### 
 ##   ##  ##     ## ##     ##           ## ##           #### #### ##       #### #### 
##     ## ##     ## ##     ##         #########         ## ### ## ######   ## ### ## 
######### ##     ## ##     ##           ## ##           ##     ## ##       ##     ## 
##     ## ##     ## ##     ##          ##   ##          ##     ## ##       ##     ## 
##     ## ########  ########  #######           ####### ##     ## ######## ##     ## 
*/
/* add_slug_mem():
 * Purpose: Adds an element to the SLUG_MEM.allocs array
 *          and keeps track of:
 *          file name, line number, time allocated, and size
 * Parameters: ptr      - the address of the malloc'ed memory
 *             FILE_POS - information about the malloc such as filename and line
 *                        number
 *             size - Size of the allocation
*/
void add_slug_mem(void *ptr, char *FILE_POS, size_t size)
{
    slug_mem_t slug_x;
    char file_pos[64];
    int i;

    #if DEBUG_PRINT
        printf("%s", buffy);printf("%s:%s\n", ">>>" TAG, "add_slug_mem");
        strcat(buffy, "\t"); 
        printf("%s", buffy);printf("ptr:%p FILE_POS:%s\n", ptr, FILE_POS);
        printf("%s", buffy);printf("size:%d max_size:%d\n", 
            SLUG_MEM.size, SLUG_MEM.max_size);
    #endif

    /* if SLUG_MEM's array is full */
    if( SLUG_MEM.size == (SLUG_MEM.max_size-1) ) {        
        SLUG_MEM.max_size = SLUG_MEM.max_size * 2;
        SLUG_MEM.allocs =
            realloc(SLUG_MEM.allocs, SLUG_MEM.max_size * sizeof(slug_mem_t));
        for (i = SLUG_MEM.size+1; i < SLUG_MEM.max_size; i++){
            SLUG_MEM.allocs[i].size = -1;
        }
    }

    strcpy(file_pos, FILE_POS);

    slug_x.addr = ptr;
    slug_x.file = strdup( strtok(file_pos, "|") );
    slug_x.line = strdup( strtok(NULL, "|") );
    slug_x.timestamp = time(NULL);
    slug_x.flags = used; 
    slug_x.size = size;

    SLUG_MEM.allocs[SLUG_MEM.size] = slug_x;
    SLUG_MEM.size++;

    #if DEBUG_PRINT
        print_slug_mem_t(slug_x, "slug_x");

        *strrchr(buffy, '\t') = (char)0;
        printf("%s", buffy);printf("%s:%s\n", "<<<" TAG, "add_slug_mem");
    #endif
}

/*
########  ########  #### ##    ## ########         ######## ##       ######## ##     ## 
##     ## ##     ##  ##  ###   ##    ##            ##       ##       ##       ###   ### 
##     ## ##     ##  ##  ####  ##    ##            ##       ##       ##       #### #### 
########  ########   ##  ## ## ##    ##            ######   ##       ######   ## ### ## 
##        ##   ##    ##  ##  ####    ##            ##       ##       ##       ##     ## 
##        ##    ##   ##  ##   ###    ##            ##       ##       ##       ##     ## 
##        ##     ## #### ##    ##    ##    ####### ######## ######## ######## ##     ## 
*/
/* print_slug_mem_t():
 * Purpose: To print out the information for elem
 * Parameters: elem - the element that we want to print the information for
 *             extra - an extra optional prefix to the main output
 * Assumptions: Assumes that elem and extra is not null.
 */
#if DEBUG_PRINT
void print_slug_mem_t(slug_mem_t elem, char *extra)
{
    printf("%s", buffy);
    printf("<%8s:%8p|%12s|%4s|%d|%2d|%2lu>\n", 
        extra, elem.addr, elem.file, 
        elem.line, elem.timestamp, 
        elem.flags, elem.size);
}
#endif

/*
########  ########  #### ##    ## ########            ###    ########  ########     ###    ##    ## 
##     ## ##     ##  ##  ###   ##    ##              ## ##   ##     ## ##     ##   ## ##    ##  ##  
##     ## ##     ##  ##  ####  ##    ##             ##   ##  ##     ## ##     ##  ##   ##    ####   
########  ########   ##  ## ## ##    ##            ##     ## ########  ########  ##     ##    ##    
##        ##   ##    ##  ##  ####    ##            ######### ##   ##   ##   ##   #########    ##    
##        ##    ##   ##  ##   ###    ##            ##     ## ##    ##  ##    ##  ##     ##    ##    
##        ##     ## #### ##    ##    ##    ####### ##     ## ##     ## ##     ## ##     ##    ##    
*/
/* print_slug_mem():
 * Purpose: To print out the information for the whole SLUG_MEM array,
 *          ie: prints out all non-freed blocks
*/
void print_slug_mem(void)
{
    int i, num_mallocs, num_nulls;
    slug_mem_t elem;

    num_nulls = 0;
    num_mallocs = 0;

    #if DEBUG_PRINT
        printf("%s", buffy);printf("%s:%s\n", ">>>" TAG, "print_slug_mem");
        strcat(buffy, "\t"); 
        printf("%s", buffy);
        printf("size:%d|max_size:%d\n", SLUG_MEM.size, SLUG_MEM.max_size);
    #endif

    printf("%s\n", "MEMORY STILL IN USE:");
    for (i = 0; i < SLUG_MEM.max_size; i++) {
        if (SLUG_MEM.allocs[i].size != -1) {
            if (SLUG_MEM.allocs[i].flags == freed) 
                continue;
            num_mallocs++;
            #if DEBUG_PRINT
                printf("%s", buffy);
            #endif
            printf("%6d: ", i);
            elem = SLUG_MEM.allocs[i];
            printf("%8p| %12s| %4s| %d| %6lu\n", 
                elem.addr, elem.file, 
                elem.line, elem.timestamp, 
                elem.size);
        } else {
            num_nulls++;
        }
    }
    printf("%6s: %8s| %12s| %4s|  %s| %6s\n\n", 
        "pos", "addr", "file", "line", 
                                "timestamp", "size");

    #if DEBUG_PRINT
        printf("%s", buffy);
        printf("mallocs:%d|nulls:%d\n", num_mallocs, num_nulls);
        *strrchr(buffy, '\t') = (char)0;
        printf("%s", buffy);printf("%s:%s\n", "<<<" TAG, "print_slug_mem");
    #endif
}


/*
                  ######## ########  ######## ########         ##     ## ######## ##     ## 
 ##   ##          ##       ##     ## ##       ##               ###   ### ##       ###   ### 
  ## ##           ##       ##     ## ##       ##               #### #### ##       #### #### 
#########         ######   ########  ######   ######           ## ### ## ######   ## ### ## 
  ## ##           ##       ##   ##   ##       ##               ##     ## ##       ##     ## 
 ##   ##          ##       ##    ##  ##       ##               ##     ## ##       ##     ## 
          ####### ##       ##     ## ######## ######## ####### ##     ## ######## ##     ## 
*/
/* slug_free_mem():
 * Purpose: Used to free anything we allocate
 *      NOTE/TODO: currently also cleans up users mallocs.
 */
void slug_free_mem(void)
{
    #if DEBUG_PRINT
        printf("%s", buffy);printf("%s:%s\n", ">>>" TAG, "slug_free_mem");
        strcat(buffy, "\t"); 
    #endif

    int i;
    for(i = 0; i < SLUG_MEM.size; i++) {
        free(SLUG_MEM.allocs[i].file);
        free(SLUG_MEM.allocs[i].line);
        if (SLUG_MEM.allocs[i].flags != freed)
            free(SLUG_MEM.allocs[i].addr);
    }
    free(SLUG_MEM.allocs);
    pntr = NULL;

    #if DEBUG_PRINT
        *strrchr(buffy, '\t') = (char)0;
        printf("%s", buffy);printf("%s:%s\n", "<<<" TAG, "slug_free_mem");
    #endif
}

/*
                     ###    ######## ######## ##     ## #### ######## 
 ##   ##            ## ##      ##    ##        ##   ##   ##     ##    
  ## ##            ##   ##     ##    ##         ## ##    ##     ##    
#########         ##     ##    ##    ######      ###     ##     ##    
  ## ##           #########    ##    ##         ## ##    ##     ##    
 ##   ##          ##     ##    ##    ##        ##   ##   ##     ##    
          ####### ##     ##    ##    ######## ##     ## ####    ##    
*/
/* slug_atexit():
 * Used to call slug_memstats() and slug_free_mem() 
 *      when the the "parent" program is done. 
 */
void slug_atexit(void)
{
    #if DEBUG_PRINT
        printf("%s", buffy);printf("%s:%s\n", ">>>" TAG, "slug_atexit");
        strcat(buffy, "\t"); 
    #endif

    slug_memstats();
    slug_free_mem();

    #if DEBUG_PRINT
        *strrchr(buffy, '\t') = (char)0;
        printf("%s", buffy);printf("%s:%s\n", "<<<" TAG, "slug_atexit");
    #endif
}

/*
                  ##     ## ######## ##     ##  ######  ########    ###    ########  ######  
 ##   ##          ###   ### ##       ###   ### ##    ##    ##      ## ##      ##    ##    ## 
  ## ##           #### #### ##       #### #### ##          ##     ##   ##     ##    ##       
#########         ## ### ## ######   ## ### ##  ######     ##    ##     ##    ##     ######  
  ## ##           ##     ## ##       ##     ##       ##    ##    #########    ##          ## 
 ##   ##          ##     ## ##       ##     ## ##    ##    ##    ##     ##    ##    ##    ## 
          ####### ##     ## ######## ##     ##  ######     ##    ##     ##    ##     ######  
*/
/* slug_memstats():
 * Purpose: Prints out all still reachable (not freed) blocks of memory
 *          and prints out a summary as specified.
 *          (see: print statements or instructions.pdf)
 */
void slug_memstats(void)
{
    int i;
    int num_allocs;
    size_t tot_mem; size_t in_use_mem;
    double sum;
    double mean;
    double std_dev;

    num_allocs = 0; 
    tot_mem = 0; 
    in_use_mem = 0;
    sum = 0.; 
    mean = 0.; 
    std_dev = 0.;

    #if DEBUG_PRINT
        printf("%s", buffy);printf("%s:%s\n", ">>>" TAG, "slug_memstats");
        strcat(buffy, "\t"); 
    #endif

    print_slug_mem();

    for (i = 0; i < SLUG_MEM.size; i++) {
        tot_mem += SLUG_MEM.allocs[i].size;
        if (SLUG_MEM.allocs[i].flags == used) {
            num_allocs++;
            in_use_mem += SLUG_MEM.allocs[i].size;
        }
    }
    printf("%30s %6d\n", "Number of current allocations:", num_allocs);

    printf("%30s %6d\n", "Total memory still in use:", in_use_mem);
    printf("%30s %6d\n", "Total number of allocations:", SLUG_MEM.size);

    mean = tot_mem / (double)SLUG_MEM.size;

    for (i = 0; i < SLUG_MEM.size; i++) {
        sum += pow( (SLUG_MEM.allocs[i].size - mean), 2);
    }
    std_dev = sum / ((SLUG_MEM.size - 1)?(SLUG_MEM.size - 1):1);
    std_dev = sqrt(std_dev);

    printf("\nMean: %.2lfB, std_dev:%.2lfB\n\n", mean, std_dev);

    #if DEBUG_PRINT
        *strrchr(buffy, '\t') = (char)0;
        printf("%s", buffy);printf("%s:%s\n", "<<<" TAG, "slug_memstats");
    #endif
}
