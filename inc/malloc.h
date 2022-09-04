#ifndef MALLOC_H
#define MALLOC_H

#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "../libft/libft.h"


#define PROT    PROT_READ|PROT_WRITE
#define MAP     MAP_SHARED|MAP_ANONYMOUS

#define TINY_FACTOR     5
// #define TINY_LIMIT      256
#define SMALL_FACTOR	5
// #define SMALL_LIMIT     1024


typedef struct		s_heap
{
	void			*head;
	void			*tiny;
	void			*small;
	void			*large;
	void			*largeEnd;
	int				pagesize;
	size_t			TINY_LIMIT;
	size_t			SMALL_LIMIT;

}					t_heap;


typedef struct		s_metadata
{
	size_t			size;
	unsigned int	isFree;
	void			*prev;
	void			*next;
}					t_metadata;


void show_alloc_mem();
void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);

#endif