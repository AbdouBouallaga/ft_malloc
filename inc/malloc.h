#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>


#define PROT PROT_READ|PROT_WRITE
#define MAP MAP_SHARED|MAP_ANONYMOUS

void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);