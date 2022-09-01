#include "../inc/malloc.h"


/// GLOBAL VARIABLE HEAP

t_heap *heap = NULL;

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
    printf("PageSize%d\n\n", heap->pagesize);
    heap->TINY_LIMIT = heap->pagesize/2;
    heap->SMALL_LIMIT = heap->pagesize;

    heap->tiny = add_chunk(heap->pagesize * TINY_FACTOR);
    heap->small = add_chunk(heap->pagesize * SMALL_FACTOR);
    heap->large = NULL;
}

void            *allocate_in_zone(void **zone, size_t size,size_t zonefactor)
{
    void            *current = *zone;
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
        if (size > heap->SMALL_LIMIT)
            printf("ptr %p\nsize %d\nisFree %d\n",current, meta->size, meta->isFree);
        if(meta->isFree && size < meta->size - meta_size){
                meta->isFree = 0;
            if (size <= heap->SMALL_LIMIT){ 
                // in preallocated zones, the free space is devided,\
                take the size you need and define the rest as free space.
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
    void        *ptr;
    int         PageSize = getpagesize();
    int         sizeOfMeta = sizeof(t_metadata);
    t_metadata  *meta;
	/// initiate heap head
	if (!heap)
        init_heap();
    if (size){
        if (size <= heap->TINY_LIMIT)
            ret = allocate_in_zone(&heap->tiny, size, TINY_FACTOR);
        else if (size <= heap->SMALL_LIMIT)
            ret = allocate_in_zone(&heap->small, size, SMALL_FACTOR);
        else{
            ret = allocate_in_zone(&heap->largeEnd, size, (size/heap->pagesize)+1);
            heap->largeEnd = ret; // to save time, the serch for free space if not needed\
                                    large zone.
        }
    }
	return(ret);
}

void free(void *ptr)
{
    unsigned int    meta_size = sizeof(t_metadata);
    if (ptr == NULL){
        return;
    }
    int deb = 111;
    int     accum = 0;
    void    *current = ptr;
    void    *save_max;
    void    *save_min;
    int     zone_large;
	t_metadata *meta = ptr - sizeof(t_metadata);
	meta->isFree = 1;
    zone_large = (meta->size > heap->SMALL_LIMIT) ? 1 : 0;
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
        
        deb = munmap(meta_munmap, meta_munmap->size); // munmap remove pages by pagesize*
        // printf("munmap return %d\n",deb);
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
        printf("saved_max %p\n", save_max);
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
        printf("saved_min %p\n", save_min);

        printf("accum %d\n", accum);
    }
}

/////////////////////////////////////////test part//////////////////////////////////////////

void print_bytes(void *ptr, int size) 
{
    unsigned char *p = ptr;
    int i;
    for (i=0; i<size; i++) {
        printf("%02hhX ", p[i]);
    }
    printf("\n");
}

void    display()
{
    t_metadata *meta;
    int c = 1;
    void *page = heap->tiny;
    void *save;
    printf("====\t===========\t==========\n");
    printf("N\tZONE\tPTR\t\tSIZE\tISFREE\tPREV\t\tNEXT\n");
    while(page){
        meta = page - sizeof(t_metadata);
        printf("%d\ttiny\t%p\t%d\t%d\t", c, page, meta->size, meta->isFree);
        printf("%p\t%p\n",meta->prev, meta->next);
        page = meta->next;
        c++;
    }
    page = heap->small;
    printf("\n");\
    while(page){
        meta = page - sizeof(t_metadata);
        printf("%d\tsmall\t%p\t%d\t%d\t", c, page, meta->size, meta->isFree);
        printf("%p\t%p\n",meta->prev, meta->next);
        page = meta->next;
        c++;
    }
    page = heap->large;
    printf("\n");
    while(page){
        meta = page - sizeof(t_metadata);
        printf("%d\tLarge\t%p\t%d\t%d\t", c, page, meta->size, meta->isFree);
        printf("%p\t%p\n",meta->prev, meta->next);
        if (meta->next == NULL)
            save = page;
        page = meta->next;
        c++;
    }
    page = save;
    return;
}

int main(){
    int max = 6;
    int i = -1;
    // void *ptr;
    void *ptr0[max];
    void *ptr1[max];
    void *ptr2[max];
    int c = 1;
    // ptr0[0] = malloc(sizeof(char)*10);
    while (++i < max){
        ptr0[i] = malloc(163);
        ptr1[i] = malloc(16384);
        ptr2[i] = malloc(26384);
    }
    display();
    // free(ptr2[4]);
    free(ptr2[0]);
    free(ptr2[2]);
    free(ptr2[3]);
    // free(ptr2[]);
    // display();
    ptr2[4] = malloc(56384);
    display();
    return (0);
}

////////////////////////////


// typedef struct      s_list{
//     int             data;
//     long            link;
// }                   t_list;

// t_list    *add_blk(int data)
// {
//     void *m;
//     t_list *ret = (t_list *)malloc(sizeof(t_list));
//     m = ret - sizeof(t_metadata);
//     printf(">ret %p\n", ret);
//     printf(">meta %p\n\n", m);
//     ret->data = data;
//     return(ret);
// }

// long xor(t_list *l1, t_list *l2)
// {
//     return((long)l1 ^ (long)l2);
// }

// long *tr_for(t_list *cur, t_list *prev)
// {
//     return(xor(prev, cur->link));
// }

// long *tr_bac(t_list *cur, t_list *next)
// {
//     return(xor(next, cur->link));
// }


// int main()
// {
//     printf("\n");
//     int i = 2;
//     t_list *head = add_blk(1);
//     t_list *prev;
//     t_list *cur;
//     t_list *next = add_blk(2);
//     t_list *temp;
//     t_metadata *meta;
//     head->link = xor(NULL, next);
//     cur = next;
//     prev = head;
//     while (++i < 3){
//         temp = add_blk(i);
//         cur->link = xor(prev, temp);
//         temp->link = xor(NULL, cur);
//         prev = cur;
//         cur = temp;
//     }
//     i = 0;
//     cur = head;
//     prev = NULL;
//     printf("\n");
//     // while(++i < 11){
//     //     meta = cur - sizeof(t_metadata);
//     //     printf("addr: %p\nMetadata:\nsize = %d\nisfree = %d\n\n", cur, meta->size, meta->isFree);
//     //     next = tr_for(cur, prev);
//     //     prev = cur;
//     //     cur = next;
//     // }
//     return(0);
// }