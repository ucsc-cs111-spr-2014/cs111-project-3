#include "slugmalloc.h"
#define NUM_MALLOCS 10

int main(int argc, char *argv[]) {
	int *ptr_one, i;
	int *ptrs[NUM_MALLOCS];
	for (i = 0; i < NUM_MALLOCS; i++) {
		ptrs[i] = (int *) malloc(sizeof(int));

		if (ptrs[i] == NULL) {
			printf("ERROR: Out of memory\n");
			return 1;
		}

		printf("%d\n", i);
		sleep(2);
		/*free(ptr_one);*/
	}
	
	
	
	print_slug_mem();
	sleep(2);

	free(ptrs[1]);	
	print_slug_mem();
	sleep(2);

	free(ptrs[3]);
	print_slug_mem();
	sleep(2);

	free(ptrs[5]);
	print_slug_mem();
	sleep(2);

	free(ptrs[7]);
	print_slug_mem();
	return 0;
}
