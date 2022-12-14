#ifndef MALLOC_H
#define MALLOC_H


#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include "../libft/libft.h"


#include <string.h>


#define PROT    PROT_READ | PROT_WRITE
#define MAP     MAP_PRIVATE | MAP_ANONYMOUS


#define TINY_FACTOR     1
#define SMALL_FACTOR	1

// typedef struct		s_mmaped
// {
// 	void 			*ptr;
// 	size_t			size;
// 	void			*prev;
// 	void			*next;
// }					t_mmaped;

typedef struct		s_heap
{
	void			*tiny;
	void			*small;
	void			*large;

	void			*largeEnd;

	int				pagesize;
	int				meta_size;
	size_t			TINY_LIMIT;
	size_t			SMALL_LIMIT;
	// void			*safe; //a backup of meta_datas
	// int				safe_count;
}					t_heap;


typedef struct		s_metadata
{
	char			safe_one;
	char			safe_two;
	char			safe_three;
	char			isFree;
	size_t			size;
	void			*prev;
	void			*next;
}					t_metadata;


void show_alloc_mem();
void show_alloc_mem_ex(void *ptr, int deb); // deb 1 will dump all chunk content "include allocation meta data". 0 will only dump the content.
void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);

#endif