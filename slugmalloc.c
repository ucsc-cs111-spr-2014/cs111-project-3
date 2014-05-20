#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define SLUGMALLOC_C
#include "slugmalloc.h"

#define TAG "CS-111-OS"

void print_slug_mem_t(slug_mem_t elem) {
	printf("%p|%s|%s\n", 
		elem->addr, elem->file, elem->line);
}

void print_slug_mem(void) {
	int i, num_mallocs, num_nulls;
	num_nulls = 0;
	num_mallocs = 0;

	printf("size:%d|max_size:%d\n", SLUG_MEM->size, SLUG_MEM->max_size);
	for (i = 0; i < SLUG_MEM->max_size; i++) {
		if (SLUG_MEM->allocs[i] != NULL) {
			if (SLUG_MEM->allocs[i]->flags == 1) {
				printf("continue.");
				continue;
			}
			printf("start!");
			num_mallocs++;
			print_slug_mem_t(SLUG_MEM->allocs[i]);
		} else {
			num_nulls++;
		}
	}

	printf("mallocs:%d|nulls:%d\n", num_mallocs, num_nulls);
}

int add_slug_mem(void *ptr, char *FILE_POS) 
{
	slug_mem_t slugT;
	char file_pos[64];

	printf("size:%d max_size:%d\n", 
		SLUG_MEM->size, SLUG_MEM->max_size);

	if((SLUG_MEM->size * 2) > SLUG_MEM->max_size) {
		printf("%s\n", "REALLOC-ATING");
		
		SLUG_MEM->max_size = SLUG_MEM->max_size * 4;
		SLUG_MEM->allocs =
			realloc(SLUG_MEM->allocs, SLUG_MEM->max_size);
		print_slug_mem();
	}

	slugT = malloc(sizeof(slug_mem_t));
	slugT->addr = ptr;

	strcpy(file_pos, FILE_POS);
	strcpy( slugT->file, strtok(file_pos, "|") );
	strcpy( slugT->line, strtok(NULL, "|") );

	slugT->timestamp = time(NULL);
	slugT->flags = 0;
	SLUG_MEM->allocs[SLUG_MEM->size] = slugT;
	print_slug_mem_t(slugT);
	print_slug_mem_t(SLUG_MEM->allocs[SLUG_MEM->size]);
	SLUG_MEM->size++;
}

void *slug_malloc(size_t size, char *FILE_POS) 
{
	void *ptr;

	printf("%s:%s: %s\n", TAG, FILE_POS, "slug_malloc");
	if (SLUG_MEM == NULL) {
		printf("%s\n", "INIT-IALIZING");
		SLUG_MEM = malloc(sizeof(slug_mem));
		SLUG_MEM->size = 0;
		SLUG_MEM->max_size = 128;
		SLUG_MEM->allocs = calloc(SLUG_MEM->max_size, sizeof(slug_mem_t));
	}
	ptr = malloc(size);
	add_slug_mem(ptr, FILE_POS);
	return ptr;
}

void slug_free(void *ptr, char *FILE_POS) 
{
	int i;
	printf("%s:%s: %s\n", TAG, FILE_POS, "slug_free\n");

	/*for (i = 0; i < SLUG_MEM->max_size; i++) {
		if (SLUG_MEM->allocs[i] != NULL 
				&& SLUG_MEM->allocs[i]->flags == 0
				&& SLUG_MEM->allocs[i]->addr == ptr) {
			SLUG_MEM->allocs[i]->flags = 1;
		}
	}*/

	free(ptr);
}
