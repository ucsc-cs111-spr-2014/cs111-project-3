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
	ie: SLUG_MEM->flags[SLUG_MEM->size]*/
void print_slug_mem_t(slug_mem_t elem, char *extra) {
	printf("%s", buffy);
	printf("%s:%p|%s|%s|%d|%d\n", 
		extra, elem->addr, elem->file, 
		elem->line, elem->timestamp, 
		SLUG_MEM->flags[SLUG_MEM->size]);
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
	printf("size:%d|max_size:%d\n", SLUG_MEM->size, SLUG_MEM->max_size);
	for (i = 0; i < SLUG_MEM->max_size; i++) {
		if (SLUG_MEM->allocs[i] != NULL) {
			if (SLUG_MEM->flags[i] == freed) {
				printf("c.%d\n", i);
				continue;
			}
			printf("%s", buffy);printf("s!");
			num_mallocs++;
			printf("%d:", i);
			elem = SLUG_MEM->allocs[i];
			printf("%s:%p|%s|%s|%d|%d\n", 
				"SLUG_MEM", elem->addr, elem->file, 
				elem->line, elem->timestamp, 
				SLUG_MEM->flags[i]);
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
int add_slug_mem(void *ptr, char *FILE_POS) 
{
	slug_mem_t slugT;
	char file_pos[64];
	char buf1[128];
	char buf2[32];

	printf("%s", buffy);printf("%s:%s\n", ">>>" TAG, "add_slug_mem");
	strcat(buffy, "\t"); 

	printf("%s", buffy);printf("ptr:%p FILE_POS:%s\n", ptr, FILE_POS);
	printf("%s", buffy);printf("size:%d max_size:%d\n", 
		SLUG_MEM->size, SLUG_MEM->max_size);

	if((SLUG_MEM->size * 2) > SLUG_MEM->max_size) {
		printf("%s", buffy);printf("%s\n", "REALLOC-ATING");
		
		SLUG_MEM->max_size = SLUG_MEM->max_size * 4;
		SLUG_MEM->allocs =
			realloc(SLUG_MEM->allocs, SLUG_MEM->max_size * sizeof(slug_mem_t));
		SLUG_MEM->flags =
			realloc(SLUG_MEM->flags, SLUG_MEM->max_size * sizeof(int));
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

	slugT->file = strdup( strtok(file_pos, "|") );
	slugT->line = strdup( strtok(NULL, "|") );

	print_slug_mem_t(slugT, "slugT");

	slugT->timestamp = time(NULL);
	SLUG_MEM->flags[SLUG_MEM->size] = used; 

	SLUG_MEM->allocs[SLUG_MEM->size] = slugT;
	print_slug_mem_t(SLUG_MEM->allocs[SLUG_MEM->size], "allocs");
	print_slug_mem_t(slugT, "slugT");
	SLUG_MEM->size++;

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

	printf("%s", buffy);
	printf("%s%s%s%s:%s\n", ">>>", TAG, "-", FILE_POS, "slug_malloc");
	strcat(buffy, "\t"); 

	if (SLUG_MEM == NULL) {
		printf("%s", buffy);printf("%s\n", "INIT-IALIZING");
		SLUG_MEM = calloc(1, sizeof(slug_mem));
		SLUG_MEM->size = 0;
		SLUG_MEM->max_size = 8;
		SLUG_MEM->allocs = calloc(SLUG_MEM->max_size, sizeof(slug_mem_t));
		SLUG_MEM->flags = calloc(SLUG_MEM->max_size, sizeof(int));

		atexit(print_slug_mem);
	}
	ptr = malloc(size);
	add_slug_mem(ptr, FILE_POS);

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

	for(i=0; i < SLUG_MEM->size; i++) {
		if (SLUG_MEM->allocs[i] != NULL) {
			if(SLUG_MEM->allocs[i]->addr == ptr) {
				SLUG_MEM->flags[i] = freed;
				free(ptr);
				break;
			}
		}
	}
	*strrchr(buffy, '\t') = (char)0;
	printf("%s", buffy);printf("%s:%s\n", "<<<" TAG, "slug_free");
}
