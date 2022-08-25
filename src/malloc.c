#include "../inc/malloc.h"

void	*HeapHead = NULL;

#define PROT PROT_READ|PROT_WRITE
#define MAP MAP_SHARED|MAP_ANONYMOUS

typedef struct		s_metadata{
	unsigned int	size;
	unsigned char	isFree;
}					t_metadata;

void *malloc(size_t size)
{
	void *ptr;
	/// initiate heap head
	if (HeapHead == NULL){
		HeapHead = mmap(0, 0, PROT, MAP,0, 0);
	}
    ////
    //  printf("size%d\n", size);
    //  printf("Msize%d\n", sizeof(t_metadata));
	/// allocate metadata
	t_metadata *meta = mmap(0, sizeof(t_metadata), PROT, MAP,0, 0);
	meta->size = size;
	meta->isFree = 0;
	ptr = mmap(0, size, PROT, MAP,0, 0);

	/// allocate memory by size


    printf("malloc\nptr: %p\nmeta: %p\n s: %p\nF: %p\n", ptr, meta, meta->size, meta->isFree);
	return(ptr);
}

void free(void *ptr){
	t_metadata *meta = ptr - sizeof(t_metadata);

	meta->isFree = 1;

}

////////////////////////////////////////////////test part/////////////////////////////////////////////

typedef struct      s_list{
    int             data;
    long            link;
}                   t_list;

t_list    *add_blk(int data){
    void *m;
    t_list *ret = (t_list *)malloc(sizeof(t_list));
    m = ret - sizeof(t_metadata);
    printf(">ret %p\n", ret);
    printf(">meta %p\n\n", m);
    ret->data = data;
    return(ret);
}

long xor(t_list *l1, t_list *l2){
    return((long)l1 ^ (long)l2);
}

t_list *tr_for(t_list *cur, t_list *prev){
    return(xor(prev, cur->link));
}

t_list *tr_bac(t_list *cur, t_list *next){
    return(xor(next, cur->link));
}

int main()
{
    printf("\n");
    int i = 2;
    t_list *head = add_blk(1);
    t_list *prev;
    t_list *cur;
    t_list *next = add_blk(2);
    t_list *temp;
    t_metadata *meta;
    head->link = xor(NULL, next);
    cur = next;
    prev = head;
    while (++i < 3){
        temp = add_blk(i);
        cur->link = xor(prev, temp);
        temp->link = xor(NULL, cur);
        prev = cur;
        cur = temp;
    }
    i = 0;
    cur = head;
    prev = NULL;
    printf("\n");
    // while(++i < 11){
    //     meta = cur - sizeof(t_metadata);
    //     printf("addr: %p\nMetadata:\nsize = %d\nisfree = %d\n\n", cur, meta->size, meta->isFree);
    //     next = tr_for(cur, prev);
    //     prev = cur;
    //     cur = next;
    // }
    return(0);
}