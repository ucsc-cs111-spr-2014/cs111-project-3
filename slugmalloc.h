#ifndef SLUGMALLOC_H
#define SLUGMALLOC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FUNCTIONIZE(a, b) a(b)
#define STRINGIZE(a) #a
#define INT2STRING(i) FUNCTIONIZE(STRINGIZE, i)
#define FILE_POS __FILE__ ":" INT2STRING(__LINE__)

#define malloc(s) slug_malloc((s), FILE_POS)

#endif