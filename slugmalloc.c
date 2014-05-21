/* CREATED 5/21/2014
 * Implement all the functions for slugmalloc.h
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <math.h>

#define SLUGMALLOC_C
#include "slugmalloc.h"

#define TAG "CS-111"

char buffy[128];

int slug_atexit(void);

void print_slug_mem_t(slug_mem_t elem, char *extra) 
{
    printf("%s", buffy);
    printf("%s:%p|%s|%s|%d|%d|%d\n", 
        extra, elem.addr, elem.file, 
        elem.line, elem.timestamp, 
        elem.flags, elem.size);
}

void print_slug_mem(void) 
{
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
            num_mallocs++;
            printf("%s", buffy);
            printf("%d:", i);
            elem = SLUG_MEM.allocs[i];
            printf("%p|%s|%s|%d|%d|%d\n", 
                elem.addr, elem.file, 
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

    strcpy(file_pos, FILE_POS);

    slugT.file = strdup( strtok(file_pos, "|") );
    slugT.line = strdup( strtok(NULL, "|") );

    slugT.timestamp = time(NULL);
    slugT.flags = used; 
    slugT.size = size;
    SLUG_MEM.allocs[SLUG_MEM.size] = slugT;
    print_slug_mem_t(slugT, "slugT");
    SLUG_MEM.size++;

    *strrchr(buffy, '\t') = (char)0;
    printf("%s", buffy);printf("%s:%s\n", "<<<" TAG, "add_slug_mem");
}

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
        atexit(slug_atexit);
    }
    ptr = malloc(size);
    add_slug_mem(ptr, FILE_POS, size);

    *strrchr(buffy, '\t') = (char)0;
    printf("%s", buffy);printf("%s:%s\n", "<<<" TAG, "slug_malloc");
    return ptr;
}

void slug_free(void *ptr, char *FILE_POS) 
{
    int i;
    i = 0;
    printf("%s", buffy);
    printf("%s%s%s%s:%s\n", ">>>", TAG, "-", FILE_POS, "slug_free");
    strcat(buffy, "\t"); 

    for(i=0; i < SLUG_MEM.size; i++) {
        if (SLUG_MEM.allocs[i].size != -1) {
            if(SLUG_MEM.allocs[i].addr == ptr
                && SLUG_MEM.allocs[i].flags == used) {
                SLUG_MEM.allocs[i].flags = freed;
                free(ptr);
                print_slug_mem_t(SLUG_MEM.allocs[i], "free");
                break;
            }
        }
    }
    *strrchr(buffy, '\t') = (char)0;
    printf("%s", buffy);printf("%s:%s\n", "<<<" TAG, "slug_free\n");
}

void slug_free_mem(void)
{
    free(SLUG_MEM.allocs);
    pntr = NULL;
}

void slug_memstats(void);

int slug_atexit(void)
{
    printf("%s\n", "slug_atexit");
    slug_memstats();
    slug_free_mem();
    return 0;
}

void slug_memstats(void)
{
    int i;
    int num_allocs;
    size_t tot_mem;
    double sum;
    double mean;
    double std_dev;

    num_allocs = 0;
    tot_mem = 0;
    sum = 0.;
    mean = 0.;
    std_dev = 0.;

    print_slug_mem();
    printf("Number of allocations: %d\n", SLUG_MEM.size);

    for (i = 0; i < SLUG_MEM.size; i++) {
        if (SLUG_MEM.allocs[i].flags == used) {
            num_allocs++;
            tot_mem += SLUG_MEM.allocs[i].size;
        }
    }
    printf("Total memory in use: %dB\n", tot_mem);
    printf("Number current allocations: %d\n", num_allocs);

    mean = tot_mem / (double)SLUG_MEM.size;

    for (i = 0; i < SLUG_MEM.size; i++) {
        sum += pow( (SLUG_MEM.allocs[i].size - mean), 2);
    }
    std_dev = sum / ((SLUG_MEM.size - 1)?(SLUG_MEM.size - 1):1);
    std_dev = sqrt(std_dev);

    printf("Mean: %.2lfB, std_dev:%.2lfB\n", mean, std_dev);
}