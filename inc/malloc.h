#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>


#define PROT    PROT_READ|PROT_WRITE
#define MAP     MAP_SHARED|MAP_ANONYMOUS

#define TINY_FACTOR     1
#define TINY_LIMIT      1024
#define SMALL_FACTOR    2
#define SMALL_LIMIT     2048

typedef struct		s_metadata
{
	unsigned int	size;
	unsigned int	isFree;
}					t_metadata;


void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);