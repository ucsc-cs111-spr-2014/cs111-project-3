#include <stdlib.h>
#include <stdio.h>

#define TAG "CS-111-OS"

void *slug_malloc(size_t size, char *FILE_POS) 
{
	printf(">%s:%s: %s\n", TAG, FILE_POS, "slug_malloc");
	return malloc(size);
}
