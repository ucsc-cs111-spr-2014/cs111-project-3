#ifndef SLUGMALLOC_H
	#define SLUGMALLOC_H
	#include <time.h>
	
	typedef enum { used=0, freed=1 } bool;

	typedef struct {
		void *addr;
		char *file;
		char *line;
		time_t timestamp;
		int flags;
		size_t size;
	} slug_mem_t;

	typedef struct {
		int size;
		int max_size;
		slug_mem_t *allocs;
	} slug_mem;

	slug_mem SLUG_MEM;
	slug_mem *pntr;

	#ifndef SLUGMALLOC_C

		#include <stdlib.h>
		#include <stdio.h>
		#include <string.h>

		#define FUNCTIONIZE(a, b) a(b)
		#define STRINGIZE(a) #a
		#define INT2STRING(i) FUNCTIONIZE(STRINGIZE, i)
		#define FILE_POS __FILE__ "|" INT2STRING(__LINE__)

		#define malloc(s) slug_malloc((s), FILE_POS)

		#define free(s) slug_free((s), FILE_POS)

	#endif

#endif


