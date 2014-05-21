/* CREATED 5/21/2014
 *  Implement all the functions for slugmalloc.h
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define SLUGMALLOC_C
#include "slugmalloc.h"

#define TAG "CS-111"

char buffy[128];

/*
########  ########  #### ##    ## ########         ######## ##       ######## ##     ## 
##     ## ##     ##  ##  ###   ##    ##            ##       ##       ##       ###   ### 
##     ## ##     ##  ##  ####  ##    ##            ##       ##       ##       #### #### 
########  ########   ##  ## ## ##    ##            ######   ##       ######   ## ### ## 
##        ##   ##    ##  ##  ####    ##            ##       ##       ##       ##     ## 
##        ##    ##   ##  ##   ###    ##            ##       ##       ##       ##     ## 
##        ##     ## #### ##    ##    ##    ####### ######## ######## ######## ##     ## 
*/
/*Prints flag of the top of the allocs array/stack
    ie: SLUG_MEM.flags[SLUG_MEM.size]*/
void print_slug_mem_t(slug_mem_t elem, char *extra) {
    printf("%s", buffy);
    printf("%s:%p|%s|%s|%d|%d|%d\n", 
       extra, elem.addr, elem.file, 
       elem.line, elem.timestamp, 
       elem.flags, elem.size);
}

/*
########  ########  #### ##    ## ########            ###    ########  ########     ###    ##    ## 
##     ## ##     ##  ##  ###   ##    ##              ## ##   ##     ## ##     ##   ## ##    ##  ##  
##     ## ##     ##  ##  ####  ##    ##             ##   ##  ##     ## ##     ##  ##   ##    ####   
########  ########   ##  ## ## ##    ##            ##     ## ########  ########  ##     ##    ##    
##        ##   ##    ##  ##  ####    ##            ######### ##   ##   ##   ##   #########    ##    
##        ##    ##   ##  ##   ###    ##            ##     ## ##    ##  ##    ##  ##     ##    ##    
##        ##     ## #### ##    ##    ##    ####### ##     ## ##     ## ##     ## ##     ##    ##    
*/
void print_slug_mem(void) {
    int i, num_mallocs, num_nulls;
    slug_mem_t elem;

    num_nulls = 0;
    num_mallocs = 0;

    printf("%s", buffy);printf("%s:%s\n", ">>>" TAG, "print_slug_mem");
    strcat(buffy, "\t"); 

    printf("%s", buffy);
    printf("size:%d|max_size:%d\n", SLUG_MEM.size, SLUG_MEM.max_size);
    for (i = 0; i < SLUG_MEM.max_size; i++) {
       if (SLUG_MEM.allocs[i].size != -1) {
         if (SLUG_MEM.allocs[i].flags == freed) {
          printf("c.%d\n", i);
          continue;
         }
         printf("%s", buffy);printf("s!");
         num_mallocs++;
         printf("%d:", i);
         elem = SLUG_MEM.allocs[i];
         printf("%s:%p|%s|%s|%d|%d|%d\n", 
          "SLUG_MEM", elem.addr, elem.file, 
          elem.line, elem.timestamp, 
          elem.flags, elem.size);
       } else {
         num_nulls++;
       }
    }
    printf("%s", buffy);printf("mallocs:%d|nulls:%d\n", num_mallocs, num_nulls);

    *strrchr(buffy, '\t') = (char)0;
    printf("%s", buffy);printf("%s:%s\n", "<<<" TAG, "print_slug_mem");
}

/*
   ###    ########  ########          ######## ##       ######## ##     ## 
  ## ##   ##     ## ##     ##         ##       ##       ##       ###   ### 
 ##   ##  ##     ## ##     ##         ##       ##       ##       #### #### 
##     ## ##     ## ##     ##         ######   ##       ######   ## ### ## 
######### ##     ## ##     ##         ##       ##       ##       ##     ## 
##     ## ##     ## ##     ##         ##       ##       ##       ##     ## 
##     ## ########  ########  ####### ######## ######## ######## ##     ## 
*/
int add_slug_mem(void *ptr, char *FILE_POS, size_t size) 
{
    slug_mem_t slugT;
    char file_pos[64];
    int i;

    printf("%s", buffy);printf("%s:%s\n", ">>>" TAG, "add_slug_mem");
    strcat(buffy, "\t"); 

    printf("%s", buffy);printf("ptr:%p FILE_POS:%s\n", ptr, FILE_POS);
    printf("%s", buffy);printf("size:%d max_size:%d\n", 
       SLUG_MEM.size, SLUG_MEM.max_size);

    if((SLUG_MEM.size * 2) > SLUG_MEM.max_size) {
       printf("%s", buffy);printf("%s\n", "REALLOC-ATING");
       
       SLUG_MEM.max_size = SLUG_MEM.max_size * 4;
       SLUG_MEM.allocs =
         realloc(SLUG_MEM.allocs, SLUG_MEM.max_size * sizeof(slug_mem_t));
       for (i = SLUG_MEM.size+1; i < SLUG_MEM.max_size; i++){
         SLUG_MEM.allocs[i].size = -1;
       }
    }

    slugT.addr = ptr;

    /*anthony: not sure this is the best method
       try doing it without the extra buffers, 
       as I think the prob was with flags = 0 >.<*/
    strcpy(file_pos, FILE_POS);

    slugT.file = strdup( strtok(file_pos, "|") );
    slugT.line = strdup( strtok(NULL, "|") );

    print_slug_mem_t(slugT, "slugT");

    slugT.timestamp = time(NULL);
    slugT.flags = used; 
    slugT.size = size;
    SLUG_MEM.allocs[SLUG_MEM.size] = slugT;
    print_slug_mem_t(SLUG_MEM.allocs[SLUG_MEM.size], "allocs");
    print_slug_mem_t(slugT, "slugT");
    SLUG_MEM.size++;

    *strrchr(buffy, '\t') = (char)0;
    printf("%s", buffy);printf("%s:%s\n", "<<<" TAG, "add_slug_mem");
}

/*
 ######  ##       ##     ##  ######           ##     ##    ###    ##       ##        #######   ######  
##    ## ##       ##     ## ##    ##          ###   ###   ## ##   ##       ##       ##     ## ##    ## 
##       ##       ##     ## ##                #### ####  ##   ##  ##       ##       ##     ## ##       
 ######  ##       ##     ## ##   ####         ## ### ## ##     ## ##       ##       ##     ## ##       
      ## ##       ##     ## ##    ##          ##     ## ######### ##       ##       ##     ## ##       
##    ## ##       ##     ## ##    ##          ##     ## ##     ## ##       ##       ##     ## ##    ## 
 ######  ########  #######   ######   ####### ##     ## ##     ## ######## ########  #######   ######  
*/
void *slug_malloc(size_t size, char *FILE_POS) 
{
    void *ptr;
    int i;
    printf("%s", buffy);
    printf("%s%s%s%s:%s\n", ">>>", TAG, "-", FILE_POS, "slug_malloc");
    strcat(buffy, "\t"); 

    if (pntr == NULL) {
       printf("%s", buffy);printf("%s\n", "INIT-IALIZING");
       SLUG_MEM.size = 0;
       SLUG_MEM.max_size = 8;
       SLUG_MEM.allocs = calloc(SLUG_MEM.max_size, sizeof(slug_mem_t));
       for (i = 0; i < SLUG_MEM.max_size; i++){
         SLUG_MEM.allocs[i].size = -1;
       }
       pntr = &SLUG_MEM;
       atexit(print_slug_mem);
    }
    ptr = malloc(size);
    add_slug_mem(ptr, FILE_POS, size);

    *strrchr(buffy, '\t') = (char)0;
    printf("%s", buffy);printf("%s:%s\n", "<<<" TAG, "slug_malloc");
    return ptr;
}

/*
 ######  ##       ##     ##  ######           ######## ########  ######## ######## 
##    ## ##       ##     ## ##    ##          ##       ##     ## ##       ##       
##       ##       ##     ## ##                ##       ##     ## ##       ##       
 ######  ##       ##     ## ##   ####         ######   ########  ######   ######   
      ## ##       ##     ## ##    ##          ##       ##   ##   ##       ##       
##    ## ##       ##     ## ##    ##          ##       ##    ##  ##       ##       
 ######  ########  #######   ######   ####### ##       ##     ## ######## ######## 
*/
void slug_free(void *ptr, char *FILE_POS) 
{
    int i;
    i = 0;
    printf("%s", buffy);
    printf("%s%s%s%s:%s\n", ">>>", TAG, "-", FILE_POS, "slug_free");
    strcat(buffy, "\t"); 

    for(i=0; i < SLUG_MEM.size; i++) {
       if (SLUG_MEM.allocs[i].size != -1) {
         if(SLUG_MEM.allocs[i].addr == ptr) {
            if (SLUG_MEM.allocs[i].flags == freed){
                perror("Trying to free already freed memory\n");
                return;
            }
            SLUG_MEM.allocs[i].flags = freed;
            free(ptr);
            break;
         }
       }
    }
    if (i == SLUG_MEM.size){
        perror("Trying to free memory that has not been allocated\n");
        return;
    }
    *strrchr(buffy, '\t') = (char)0;
    printf("%s", buffy);printf("%s:%s\n", "<<<" TAG, "slug_free");
}

/* All allocations:
    -Size of allocation
    -Timestamp for when the allocation took place
    -The actual address of the allocations
    -The file name
    -The line number in the test program where the allocation happened
   Overall:
    -Total number of allocations done (size)
    -The number of current active allocations (size-frees)?
    -The mean and standard deviation of sizes that have been allocated
*/

void slug_memstats(void)
{

}