#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define SLUGMALLOC_C
#include "slugmalloc.h"

#define TAG "CS-111-OS"

void print_slug_mem_t(slug_mem_t elem) {
	printf("%p|%s|%s|%d|%d\n", 
		elem->addr, elem->file, elem->line, elem->timestamp, elem->flags);
}

void print_slug_mem(void) {
	int i, num_mallocs, num_nulls;
	num_nulls = 0;
	num_mallocs = 0;

	printf("size:%d|max_size:%d\n", SLUG_MEM->size, SLUG_MEM->max_size);
	for (i = 0; i < SLUG_MEM->max_size; i++) {
		if (SLUG_MEM->allocs[i] != NULL) {
			if (SLUG_MEM->allocs[i]->flags == 1) {
				printf("c.");
				continue;
			}
			printf("s!");
			num_mallocs++;
			printf("%d:", i);
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
	char buf1[128];
	char buf2[32];

	printf("ptr:%p file_pos:%s\n", ptr, FILE_POS);
	printf("size:%d max_size:%d\n", 
		SLUG_MEM->size, SLUG_MEM->max_size);

	if((SLUG_MEM->size * 2) > SLUG_MEM->max_size) {
		printf("%s\n", "REALLOC-ATING");
		
		SLUG_MEM->max_size = SLUG_MEM->max_size * 4;
		SLUG_MEM->allocs =
			realloc(SLUG_MEM->allocs, SLUG_MEM->max_size * sizeof(slug_mem_t));
			/*anthony: needed to be ... * sizeof(...)*/
		print_slug_mem();
	}

	/*anthony: why do we allocate new mem for slugT when 
		we have a whole array we should be using?*/
	slugT = calloc(1, sizeof(slug_mem_t));
	/*anthony: why isn't flags init to 0 here? 
		I get it as big int (eg 1953719668) instead*/

	slugT->addr = ptr;

	/*anthony: not sure this is the best method
		try doing it without the extra buffers, 
		as I think the prob was with flags = 0 >.<*/
	strcpy(file_pos, FILE_POS);
	strcpy( buf1, strtok(file_pos, "|") ); 
	slugT->file = strdup( buf1 );
	strcpy( buf2, strtok(NULL, "|") );
	slugT->line = strdup( buf2 );

	print_slug_mem_t(slugT);

	slugT->timestamp = time(NULL);
	/*slugT->flags = 0; 
		anthony: somehow this fux up slugT->file, wat?*/

	SLUG_MEM->allocs[SLUG_MEM->size] = slugT;
	print_slug_mem_t(SLUG_MEM->allocs[SLUG_MEM->size]);
	SLUG_MEM->size++;
}

void *slug_malloc(size_t size, char *FILE_POS) 
{
	void *ptr;

	printf("%s:%s: %s\n", TAG, FILE_POS, "slug_malloc");
	if (SLUG_MEM == NULL) {
		printf("%s\n", "INIT-IALIZING");
		SLUG_MEM = calloc(1, sizeof(slug_mem));
		SLUG_MEM->size = 0;
		SLUG_MEM->max_size = 8;
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

	/*anthony: WRONG(?)*/
	/*for (i = 0; i < SLUG_MEM->max_size; i++) {
		if (SLUG_MEM->allocs[i] != NULL 
				&& SLUG_MEM->allocs[i]->flags == 0
				&& SLUG_MEM->allocs[i]->addr == ptr) {
			SLUG_MEM->allocs[i]->flags = 1;
		}
	}*/

	free(ptr);
}
