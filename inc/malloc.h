#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

#include <string.h>


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
	int				TINY_LIMIT;
	int				SMALL_LIMIT;

}					t_heap;


typedef struct		s_metadata
{
	unsigned int	size;
	unsigned int	isFree;
	void			*next;
	void			*prev;
}					t_metadata;


void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);