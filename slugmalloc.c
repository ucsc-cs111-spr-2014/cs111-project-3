#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SLUGMALLOC_C
#include "slugmalloc.h"

#define TAG "CS-111-OS"

/*
make sure we:
-Dont free non-malloc'd memory
-Print memstats
-error on freeing non-1st-byte of malloc
-Dont free already freed memory
*/

/*
typedef struct {
	void *addr;
	char *file;
	char *line;
	long timestamp;
	unsigned flags;
} *slug_mem_t;

typedef struct {
	slug_mem_t *allocs;
	int size;
	int maxSize;
} slug_mem;
*/

int add_slug_mem(void *ptr, char *FILE_POS) 
{
	/*
	if size * 2 > maxSize -> realloc()
	increment size

	make new slug_mem_t
		init address
		split FILE_POS into file and line
		init timestamp and flags (1/0, free/not_free)
	add to slug_mem
	*/
	/*if(SLUG_MEM.size * 2 > SLUG_MEM.maxSize) {
		realloc();
	}*/
	slug_mem_t slugT = malloc(sizeof(slug_mem_t));
	slugT->addr = ptr;
	slugT->file = strtok(FILE_POS, "|");
	slugT->line = strtok(NULL, "|");
	slugT->timestamp = time(NULL);
	slugT->flags = 0;
	SLUG_MEM->allocs[SLUG_MEM->size++] = slugT;
}

void *slug_malloc(size_t size, char *FILE_POS) 
{
	void *ptr;
	
	printf(">%s:%s: %s\n", TAG, FILE_POS, "slug_malloc");
	ptr = malloc(size);
	add_slug_mem(ptr, FILE_POS);
	return ptr;
}

void slug_free(void *ptr, char *FILE_POS) 
{
	printf(">%s:%s: %s\n", TAG, FILE_POS, "slug_free");
	free(ptr);
}
