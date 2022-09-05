#include "../inc/malloc.h"

/// GLOBAL VARIABLE HEAP

t_heap *heap = NULL;
pthread_mutex_t mutex; // we only have 1 critical ressource, the heap, so we don't worry about locking hierarchie.

void    write_hex(int ch){
    if (ch >= 0 && ch < 10)
        ch = '0' + ch;
    else
        ch = 'a' + ch - 10;
    ft_putchar(ch);
}

void    hex_dump(int ch){
    int a = ch/16;
    int i = -1;
    while (++i < 2){
        if (a < 10)
            a = '0' + a;
        else
            a = 'a' + a - 10;
        ft_putchar(a);
        a = ch%16;
    }
}

void    write_address(unsigned long long num)
{
    int tab[16];
    int i = 0;
    if ((void*)num == NULL)
        ft_putstr("NULL\t");
    else{
        ft_putstr("0x");
        while (num){
            tab[i] = num%16;
            num = num/16;
            i++;
        }
        i--;
        while (i >= 0){
            write_hex(tab[i]);
            i--;
        }
    }
}

void    write_info(void *page, char *type)
{
    int c = 1;
    t_metadata *meta;
    while(page){
        meta = page - sizeof(t_metadata);
        ft_putnbr(c);
        ft_putchar('\t');
        ft_putstr(type);
        ft_putchar('\t');
        write_address((unsigned long)page);
        ft_putchar('\t');
        ft_putnbr(meta->size-sizeof(t_metadata));
        ft_putchar('\t');
        ft_putnbr(meta->isFree);
        ft_putchar('\t');
        write_address((unsigned long)meta->prev);
        ft_putchar('\t');
        write_address((unsigned long)meta->next);
        ft_putchar('\n');
        page = meta->next;
        c++;
    }
}

void show_alloc_mem_ex(void *ptr, int deb) 
{
    unsigned char *p = ptr;
    t_metadata *meta = ptr - sizeof(t_metadata);
    int i = -1;
    int size;
    if (deb){
        size = meta->size;
        p = ptr - sizeof(t_metadata);
    }
    else
        size = meta->size - sizeof(t_metadata);
    int c = 0;
    int d = 0;
    while (++i < size){
        hex_dump((p[i]));
        ft_putchar(' ');
        c++;
        d++;
        if (c == 4){
            ft_putchar(' ');
            c = 0;
        }
        if (d == 16){
            ft_putchar('\n');
            d = 0;
        }
    }
    printf("\n");
}

void    show_alloc_mem(){
    ft_putchar('\n');
    ft_putchar('\n');
    ft_putstr("N\tZONE\tPTR\t\tSIZE\tISFREE\tPREV\t\tNEXT\n");
    write_info(heap->tiny, "tiny");
    ft_putchar('\n');
    write_info(heap->small, "small");
    ft_putchar('\n');
    write_info(heap->large, "large");
    ft_putchar('\n');
    return;
}

void    *add_chunk(size_t size)
{
    void        *ptr = NULL;
    t_metadata  *meta;
    int         sizeofmeta = sizeof(t_metadata);

    meta = mmap(0, size+sizeofmeta, PROT, MAP,0, 0);
    if (meta != NULL){
	    meta->size = size;
	    meta->isFree = 1;
        meta->next = NULL;
        meta->prev = NULL;
        void *metaAddr = meta;
        ptr = metaAddr+sizeof(t_metadata);
        return(ptr);
    }
    else {
        return(NULL);
    }
}

void    init_heap()
{
    heap = mmap(0, sizeof(t_heap), PROT, MAP,0, 0);
    heap->pagesize = getpagesize();
    heap->TINY_LIMIT = heap->pagesize/2;
    heap->SMALL_LIMIT = heap->pagesize;

    heap->tiny = add_chunk(heap->pagesize * TINY_FACTOR);
    heap->small = add_chunk(heap->pagesize * SMALL_FACTOR);
    heap->large = NULL;
}

void            *allocate_in_zone(void *current, size_t size,size_t zonefactor)
{
    void            *new;
    void            *temp;
    t_metadata      *meta;
    t_metadata      *newMeta;
    unsigned int    meta_size = sizeof(t_metadata);
    if (current == NULL){
        heap->large = add_chunk(heap->pagesize * zonefactor);
        current = heap->large;
    }
    while (current) // search for free space and return a ptr.
    {
        meta = current-meta_size;
        if(meta->isFree && size < meta->size - meta_size){
                meta->isFree = 0;
            if (size <= heap->SMALL_LIMIT){ 
                // in preallocated zones, the free space is devided,
                // take the size you need and define the rest as free space.
                new = current+size+meta_size;
                newMeta = new-meta_size;
                newMeta->isFree = 1;
                newMeta->size = meta->size - size - meta_size;
                meta->size = size+meta_size;
                meta->next = new;
                newMeta->prev = current;
                newMeta->next = NULL;
            }
            return(current);
        }
        if (meta->next)
            current = meta->next;
        else{ //if there is no free space, add a chunk to the zone using mmap.
            temp = current;
            meta->next = add_chunk(heap->pagesize * zonefactor);
            current = meta->next;
            newMeta = current-meta_size;
            newMeta->prev = temp;
        }
    }
    return(NULL);
}

void            *malloc(size_t size)
{
	void        *ret = NULL;
	/// initiate heap head
    pthread_mutex_lock(&mutex);
	if (!heap)
        init_heap();
    if (size){
        if (size <= heap->TINY_LIMIT)
            ret = allocate_in_zone(heap->tiny, size, TINY_FACTOR);
        else if (size <= heap->SMALL_LIMIT)
            ret = allocate_in_zone(heap->small, size, SMALL_FACTOR);
        else{
            ret = allocate_in_zone(heap->largeEnd, size, (size/heap->pagesize)+1);
            heap->largeEnd = ret; // to save time, the serch for free space if not needed large zone.
        }
    }
    pthread_mutex_unlock(&mutex);
	return(ret);
}

void free(void *ptr)
{
    pthread_mutex_lock(&mutex);
    if (ptr == NULL){
        return;
    }
    int     accum = 0;
    void    *current = ptr;
    void    *save_max;
    void    *save_min;
    int     zone_large;
	t_metadata *meta = ptr - sizeof(t_metadata);
	meta->isFree = 1;
    zone_large = ((size_t)meta->size > heap->SMALL_LIMIT) ? 1 : 0;
    if (zone_large){ //large zone
	    t_metadata *meta_munmap = meta;
        save_min = meta->prev;
        save_max = meta->next;
        if  (save_min){
            meta = save_min - sizeof(t_metadata);
            meta->next = save_max;
        }
        if (save_max){
            meta = save_max - sizeof(t_metadata);
            meta->prev = save_min;
        }

        if (ptr == heap->large) //in case of free chunk that was the heap head
            heap->large = meta_munmap->next;
        if (meta_munmap->next == NULL && meta_munmap->prev == NULL) //..free all chunks of large
            heap->largeEnd = NULL;
        
        munmap(meta_munmap, meta_munmap->size); // munmap remove pages by pagesize*
    }
    else {
        save_max = current;
        while (current){ // search for free chunks after ptr to defragment the heap
            meta = current - sizeof(t_metadata);
            if (meta->isFree)
                accum += meta->size;
            else
                break;
            current = meta->next;
            save_max = current;
        }
        current = ptr;
        meta = current - sizeof(t_metadata);
        save_min = current;
        while (current ){ // search for free chunks before ptr to defrag
            current = meta->prev;
            meta = current - sizeof(t_metadata);
            if (meta->isFree){
                accum += meta->size;
            }
            else
                break;
            save_min = current;
            if (meta->prev == NULL)
                break;
        }
        current = save_min;
        meta = current - sizeof(t_metadata);
        meta->isFree = 1;
        meta->size = accum;
        if (save_max != save_min)
            meta->next = save_max;
    }
    pthread_mutex_unlock(&mutex);
}

// void    *realloc(void *ptr, size_t size){
//     char *ret;
//     char *old = ptr;
//     t_metadata *meta = ptr - sizeof(t_metadata);
//     t_metadata *newmeta = ptr - sizeof(t_metadata);
//     meta->isFree = 1;
//     newmeta = meta->next - sizeof(t_metadata);
//     if ((meta->size - sizeof(t_metadata)) + (newmeta->size - sizeof(t_metadata)) >= size){

//     }
// }

