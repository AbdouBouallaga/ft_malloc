#include "../inc/malloc.h"


/// GLOBAL VARIABLE HEAP

t_heap *heap = NULL;

void    *add_chunk(size_t size)
{
    void        *ptr = NULL;
    t_metadata  *meta;
    int         sizeofmeta = sizeof(t_metadata);
    // printf("sizeof t_meta %d\n", sizeofmeta);

    meta = mmap(0, size+sizeofmeta, PROT, MAP,0, 0);
    if (meta != NULL){
	    meta->size = size+sizeofmeta;
	    meta->isFree = 1;
        void *metaAddr = meta;
        // ptr = meta+(sizeofmeta/8); ///wtf is happening here
        ptr = metaAddr+sizeof(t_metadata); 
        // printf("__meta %p\n__ptr %p\n__size %d\n\n", meta, ptr,meta->size);
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
    heap->TINY_LIMIT = heap->pagesize;
    heap->SMALL_LIMIT = heap->pagesize*2;

    printf("PageSize%d\n\n", heap->pagesize);
    heap->tiny = add_chunk(heap->pagesize * TINY_FACTOR);
    heap->tiny->next = NULL;
    heap->tiny->prev = NULL;
    heap->small = add_chunk(heap->pagesize * SMALL_FACTOR);
    heap->small->next = NULL;
    heap->small->prev = NULL;
    heap->large = add_chunk(8);
    heap->large->next = NULL;
    heap->large->prev = NULL;
}

void            *allocate_in_zone(t_page **zone, size_t size,size_t zonefactor)
{
    t_page          *ptr;
    t_page          *page = *zone;
    void            *temp;
    t_metadata      *meta;
    t_metadata      *newMeta;
    unsigned int    meta_size = sizeof(t_metadata);
    // printf("factor %d\n", zonefactor);
    while (page)
    {
        temp = page;
        meta = temp-meta_size;
        // printf("meta %p\n",meta);
        // printf("\nINFO'\nmeta %p\nptr %p\nsize %d\nis free %d\nINFO END\n\n", meta, page,meta->size, meta->isFree);
        if(meta->isFree && size < meta->size - meta_size){
            meta->isFree = 0;
            newMeta = temp+size;
            ptr = temp+size+meta_size;
            newMeta->isFree = 1;
            newMeta->size = meta->size - size - meta_size;
            meta->size = size+meta_size;
            page->next = ptr;
            ptr->prev = page;
            ptr->next = NULL;
            return(page);
        }

        if (page->next)
            page = page->next;
        else{
            temp = page;
            page->next = add_chunk(heap->pagesize * zonefactor);
            page = page->next;
            page->prev = temp;
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
	if (!heap){
        printf("init heap zones\n");
        init_heap();
	}
    if (size){
        if (size <= heap->TINY_LIMIT)
            ret = allocate_in_zone(&heap->tiny, size, TINY_FACTOR);
        else if (size <= heap->SMALL_LIMIT)
            ret = allocate_in_zone(&heap->small, size, SMALL_FACTOR);
        else
            ret = allocate_in_zone(&heap->large, size, (size/heap->pagesize)+1);
    }
    // while(1)
    // {
    //     if (meta->size >= size+)
    //     {

    //     }
    // }
    //  printf("Msize%d\n", sizeof(t_metadata));
	return(ret);
}

void free(void *ptr)
{
    if (ptr == NULL){
        return;
    }
    printf("<>>>>>>>>>try to free %p\n",ptr);
    int     accum = 0;
    t_page  *current = ptr;
    void    *save_max;
    void    *save_min;
    void    *temp;
    int     zone_large;
	t_metadata *meta = ptr - sizeof(t_metadata);
	meta->isFree = 1;
    zone_large = (meta->size > heap->SMALL_LIMIT) ? 1 : 0;
    // if (zone_large){
    //     te
    // }
    save_max = current;
    while (current){
        temp = current;
        meta = temp - sizeof(t_metadata);
        if (meta->isFree){
            printf("free next\n");
            accum += meta->size;
        }
        else
            break;
        save_max = current;
        current = current->next;
    }
    printf("saved_max %p\n", save_max);
    current = ptr;
    save_min = current;
    while (current){
        current = current->prev;
        temp = current;
        meta = temp - sizeof(t_metadata);
        if (meta->isFree){
            printf("free prev\n");
            accum += meta->size;
            printf("accum %d\n", accum);
        }
        else
            break;
        save_min = current;
        if (current->prev == NULL)
            break;
        current = current->prev;
    }
    printf("saved_min %p\n", save_min);
    current = save_min;
    temp = current;
    meta = temp - sizeof(t_metadata);
    meta->isFree = 1;
    meta->size = accum;
    if (save_max != save_min)
        current->next = save_max;
    printf("saved_min %p\n", save_min);

    printf("accum %d\n", accum);
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


int main(){
    int i = -1;
    int max = 4;
    t_metadata *meta;
    void *ptr;
    void *ptr0[max];
    void *ptr1[max];
    void *ptr2[max];
    int c = 1;
    ptr0[0] = malloc(sizeof(char)*10);
    while (++i < max){
        // ptr0[i] = malloc(sizeof(char)*10);
        ptr1[i] = malloc(6000);
        ptr2[i] = malloc(12000);
        // if (ptr){
        //     meta = ptr-sizeof(t_metadata);
        //     printf("%p\n", ptr);
        //     // printf("addr %p\nsize %d\n\n",meta, meta->size);
        // }
    }
    /////////// data test

    i = -1;
    // char chr = 'A';
    // char *str;
    // t_page *p = ptr0[0];
    // while (p){
    //     str = p;
    //     while(++i < 101){
    //         str[i] = chr;
    //     }
    //     c += 2;
    //     p = p->next;
    // }

    //////////////

    ////// free test
    
    // free(ptr0[1]);
    // // free(ptr1[1]);
    // free(ptr0[2]);
    //////
    t_page *page = heap->tiny;
    t_page *save;
    printf("N\tZONE\tPTR\t\tSIZE\tISFREE\tPREV\t\tNEXT\n");
    while(page){
        ptr = page;
        meta = ptr - sizeof(t_metadata);
        printf("%d\ttiny\t%p\t%d\t%d\t", c, ptr, meta->size, meta->isFree);
        printf("%p\t%p\n",page->prev, page->next);
        page = page->next;
        c++;
    }
    page = heap->small;
    printf("\n");\
    while(page){
        ptr = page;
        meta = ptr - sizeof(t_metadata);
        printf("%d\tsmall\t%p\t%d\t%d\t", c, ptr, meta->size, meta->isFree);
        printf("%p\t%p\n",page->prev, page->next);
        page = page->next;
        c++;
    }
    page = heap->large;
    printf("\n");
    while(page){
        ptr = page;
        meta = ptr - sizeof(t_metadata);
        printf("%d\tLarge\t%p\t%d\t%d\t", c, ptr, meta->size, meta->isFree);
        printf("%p\t%p\n",page->prev, page->next);
        if (page->next == NULL)
            save = page;
        page = page->next;
        c++;
    }
    page = save;
    // printf("----\t----\t----\t\t----\t-\n");
    // while(page){
    //     ptr = page;
    //     meta = ptr - sizeof(t_metadata);
    //     printf("%d\tLargeR\t%p\t%d\t%d\n", c, ptr, meta->size, meta->isFree);
    //     page = page->prev;
    //     c++;
    // }
    // strcpy(ptr, "helo");
    // print_bytes(ptr, 1);
        // meta = ptr-sizeof(t_metadata);
        // printf("addr %p\nmera %p\nsize %d\n",ptr, meta, meta->size, meta->isFree);
    // ptr = malloc(90);
    // ptr = malloc(2048);
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