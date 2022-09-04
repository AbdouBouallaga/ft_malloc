#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <string.h>
#include "../libft/libft.h"

typedef struct		s_metadata
{
	int	size;
	unsigned int	isFree;
	void			*prev;
	void			*next;
}					t_metadata;

void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void show_alloc_mem();

#endif