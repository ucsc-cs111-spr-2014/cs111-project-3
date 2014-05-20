#ifndef SLUGMALLOC_H
	#define SLUGMALLOC_H
	#include <time.h>

	typedef struct {
		void *addr;
		char *file;
		char *line;
		time_t timestamp;
		int flags;
	} *slug_mem_t;

	typedef struct {
		slug_mem_t *allocs;
		int size;
		int max_size;
	} *slug_mem;

	slug_mem SLUG_MEM;

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


