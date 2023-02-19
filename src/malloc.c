#include "../inc/malloc.h"
#include <errno.h>
/// GLOBAL VARIABLE HEAP

t_heap heap;
pthread_mutex_t mutex; // mutex for thread safety

void add_safe(t_metadata *ptr)
{
    ptr->safe_one = 0x2f;
    ptr->safe_two = 0x40;
    ptr->safe_three = 0x5c;
}

int check_safe(t_metadata *ptr)
{

    if (ptr->safe_one == '/' && ptr->safe_two == '@' && (ptr->safe_three == '\\' || ptr->safe_three == '|'))
    { // safe_three=='|' means its returned by mmap, freeable bu munmap
        return (1);
    }
    return (0);
}

void write_hex(int ch)
{
    if (ch >= 0 && ch < 10)
        ch = '0' + ch;
    else
        ch = 'a' + ch - 10;
    ft_putchar(ch);
}

void hex_dump(int ch)
{
    int a = ch / 16;
    int i = -1;
    while (++i < 2)
    {
        if (a < 10)
            a = '0' + a;
        else
            a = 'a' + a - 10;
        ft_putchar(a);
        a = ch % 16;
    }
}

void write_address(unsigned long long num)
{
    int tab[16];
    int i = 0;
    if ((void *)num == NULL)
        ft_putstr("NULL\t");
    else
    {
        ft_putstr("0x");
        while (num)
        {
            tab[i] = num % 16;
            num = num / 16;
            i++;
        }
        i--;
        while (i >= 0)
        {
            write_hex(tab[i]);
            i--;
        }
    }
}

void write_info(void *page, char *type)
{
    unsigned int c = 1;
    t_metadata *meta;
    while (page)
    {
        meta = page - heap.meta_size;
        if (!check_safe(meta))
        {
            ft_putstr("error \n");
            return;
            // show_alloc_mem_ex(page,1);
        }
        ft_putnbr(c);
        ft_putchar('\t');
        ft_putstr(type);
        ft_putchar(' ');
        ft_putchar(meta->safe_three);
        ft_putchar('\t');
        write_address((unsigned long long)page);
        ft_putchar('\t');
        ft_putnbr(meta->size - heap.meta_size);
        ft_putchar('\t');
        ft_putchar('\t');
        ft_putnbr(meta->size);
        ft_putchar('\t');
        ft_putchar('\t');
        ft_putchar(meta->isFree);
        ft_putchar('\t');
        write_address((unsigned long long)meta->prev);
        ft_putchar('\t');
        write_address((unsigned long long)meta->next);
        ft_putchar('\n');
        page = meta->next;
        c++;
    }
}

void show_alloc_mem_ex(void *ptr, int deb)
{
    pthread_mutex_lock(&mutex);
    ft_putstr("Memory dump of ");
    write_address((unsigned long)ptr);

    unsigned char *p = ptr;
    t_metadata *meta = ptr - heap.meta_size;
    int i = -1;
    int size;
    if (deb)
    {
        ft_putstr(" including metadata\n\n");
        size = meta->size;
        p = ptr - heap.meta_size;
    }
    else
    {
        ft_putstr(" content only\n\n");
        size = meta->size - heap.meta_size;
    }
    int c = 0;
    int d = 0;
    write_address((unsigned long)p);
    ft_putchar('\t');
    while (++i < size)
    {
        hex_dump((p[i]));
        ft_putchar(' ');
        c++;
        d++;
        if (c == 4)
        {
            ft_putchar(' ');
            c = 0;
        }
        if (d == 16)
        {
            ft_putchar('\n');
            write_address((unsigned long)p + i + 1);
            ft_putchar('\t');
            d = 0;
        }
    }
    ft_putchar('\n');
    ft_putchar('\n');
    pthread_mutex_unlock(&mutex);
}

void show_alloc_mem()
{
    pthread_mutex_lock(&mutex);
    ft_putchar('\n');
    ft_putstr("Sizeof Metadata: ");
    ft_putnbr(heap.meta_size);
    ft_putchar('\n');
    ft_putstr("System Pagesize: ");
    ft_putnbr(heap.pagesize);
    ft_putchar('\n');
    ft_putstr("Zones:\n");
    ft_putstr("Tiny: 1 to ");
    ft_putnbr(heap.TINY_LIMIT);
    ft_putchar('\n');
    ft_putstr("Small: ");
    ft_putnbr(heap.TINY_LIMIT + 1);
    ft_putstr(" to ");
    ft_putnbr(heap.SMALL_LIMIT);
    ft_putchar('\n');
    ft_putstr("Large: ");
    ft_putnbr(heap.SMALL_LIMIT + 1);
    ft_putchar('+');
    ft_putchar('\n');
    ft_putchar('\n');
    ft_putstr("N\tZONE\tPTR\t\tSIZE\t\tREAL_SIZE\tISFREE\tPREV\t\tNEXT\n");
    write_info(heap.tiny, "tiny");
    ft_putchar('\n');
    write_info(heap.small, "small");
    ft_putchar('\n');
    write_info(heap.large, "large");
    ft_putchar('\n');
    pthread_mutex_unlock(&mutex);
    return;
}

void *add_chunk(size_t size)
{
    void *ptr = NULL;
    t_metadata *meta;
    // ft_putstr("ADD CHUNK");
    meta = mmap(0, size, PROT, MAP, 0, 0);
    if (meta != (void *)-1)
    {
        // if (heap.mmaped->ptr == NULL){

        // }
        meta->size = size;
        meta->isFree = 'Y';
        meta->next = NULL;
        meta->prev = NULL;
        add_safe(meta);
        meta->safe_three = '|';
        void *metaAddr = meta;
        ptr = metaAddr + heap.meta_size;
        return (ptr);
    }
    else
    {
        return (NULL);
    }
}

int init_heap()
{
    heap.pagesize = getpagesize();
    heap.TINY_LIMIT = (heap.pagesize / 10) -32;
    heap.SMALL_LIMIT = heap.pagesize - 32;
    heap.meta_size = sizeof(t_metadata);

    heap.tiny = NULL;
    heap.small = NULL;
    heap.large = NULL;

    return (1);
}

void *allocate_in_zone(void **cur, size_t size, size_t zonefactor)
{
    void *new;
    void *temp;
    t_metadata *meta;
    t_metadata *nextMeta;
    size_t meta_size = heap.meta_size;

    if (*cur == NULL)
    {
        *cur = add_chunk(heap.pagesize * zonefactor);
    }
    void *current = *cur;
    // ft_putstr("lol");
    while (current) // search for free space and return a ptr.
    {
    alloc_add_next:
        meta = current - meta_size;

        if (!check_safe(meta))
        {
            // write_address((unsigned long)current + meta_size);
            // ft_putstr(" >> OUUUPS \n");
            if (meta->next)
            {
                current = meta->next;
                goto alloc_add_next;
            }
            else
                goto alloc_add_chunk;
        }
        if (meta->isFree == 'Y' && (meta->size - (size + meta_size)) >= 0 && size <= meta->size - meta_size)
        {
            meta->isFree = 'N';
            if ((meta->size - (size + meta_size)) >= (size_t)heap.meta_size)
            {
                // in preallocated zones, the free space is devided,
                // take the size you need and define the rest as free space.
                new = current + size + meta_size;
                nextMeta = new - meta_size;
                nextMeta->isFree = 'Y';
                nextMeta->size = meta->size - (size + meta_size);
                meta->size = size + meta_size;
                meta->next = new;
                nextMeta->prev = current;
                nextMeta->next = NULL;
                add_safe(nextMeta);
            }
            return (current);
        }
        if (meta->next != NULL)
            current = meta->next;
        else
        { // if there is no free space, add a chunk to the zone using mmap.
        alloc_add_chunk:
            temp = current;
            // ft_putnbr((int)heap.pagesize * zonefactor);
            // ft_putchar('\n');
            meta->next = add_chunk(heap.pagesize * zonefactor);
            current = meta->next;
            nextMeta = current - meta_size;
            nextMeta->prev = temp;
        }
    }
    return (NULL);
}

void *malloc(size_t size)
{
    pthread_mutex_lock(&mutex);
    void *ret = NULL;

    if (!heap.tiny && !heap.small && !heap.large)
        init_heap();
    if ((int)size < 0)
        size = 0;
    if (size <= heap.TINY_LIMIT)
        ret = allocate_in_zone(&heap.tiny, size, TINY_FACTOR);
    else if (size <= heap.SMALL_LIMIT)
        ret = allocate_in_zone(&heap.small, size, SMALL_FACTOR);
    else
        ret = allocate_in_zone(&heap.large, size, ((size + heap.meta_size) / heap.pagesize) + 1);
    // write_address((unsigned long)ret); //
    // ft_putchar('\n');//
    pthread_mutex_unlock(&mutex);
    return (ret);
}

int ptr_alloc_check(void *ptr)
{
    if (ptr == NULL)
        return (0);
    void *actu = heap.tiny;
    t_metadata *meta = NULL;
    while (actu)
    {

        if ((long)actu == (long)ptr)
            return (1);
        meta = actu - heap.meta_size;
        actu = meta->next;
    }
    actu = heap.small;
    while (actu)
    {

        if ((long)actu == (long)ptr)
            return (1);
        meta = actu - heap.meta_size;
        actu = meta->next;
    }
    actu = heap.large;
    while (actu)
    {
        if ((long)actu == (long)ptr)
            return (1);
        meta = actu - heap.meta_size;
        actu = meta->next;
    }
    return (0);
}

void free(void *ptr)
{
    size_t accum = 0;
    void *current = ptr;
    void *save_max;
    void *save_min;
    int zone_large;
    pthread_mutex_lock(&mutex);
    if (ptr == NULL || !ptr_alloc_check(ptr))
        return;

    t_metadata *meta = ptr - heap.meta_size;

    if (meta->isFree == 'Y')
    {
        ft_putstr("\"malloc: *** error for object ");
        write_address((unsigned long)ptr);
        ft_putstr(": pointer being freed was not allocated\"\n");
        return;
    }
    meta->isFree = 'Y';
    zone_large = (((size_t)meta->size - heap.meta_size) > heap.SMALL_LIMIT) ? 1 : 0;

        save_max = current;
        meta = current - heap.meta_size;
        accum += meta->size;
        current = meta->next;
        while (current)
        { // search for free chunks after ptr to defragment the heap
            meta = current - heap.meta_size;
            if (meta->isFree == 'Y' && meta->safe_three != '|')
            {
                accum += meta->size;
            }
            else
                break;
            current = meta->next;
        }
        save_max = current;
        current = ptr;
        meta = current - heap.meta_size;
        save_min = current;
        if (meta->safe_three == '|')
        { // if ptr is returned by mmap, no need to go back.

            goto jump;
        }
        current = meta->prev;
        while (current)
        { // search for free chunks before ptr to defrag
            meta = current - heap.meta_size;
            if (meta->isFree == 'Y')
            {
                accum += meta->size;
            }
            else
                break;
            save_min = current;
            if (meta->prev == NULL || meta->safe_three == '|')
                break;
            current = meta->prev;
        }
        current = save_min;
    jump:
        meta = current - heap.meta_size;
        meta->isFree = 'Y';
        meta->size = accum;

        if (save_max != save_min)
            meta->next = save_max;
    // }
    if (meta->safe_three == '|' && (!meta->next || ((t_metadata *)(meta->next))->safe_three != '|'))
    {
        save_min = meta->prev; // pop the chunk from heap.
        save_max = meta->next;
        if (save_min)
        {
            meta = save_min - heap.meta_size;
            meta->next = save_max;
        }
        if (save_max)
        {
            meta = save_max - heap.meta_size;
            meta->prev = save_min;
        }
        if (current == heap.tiny)
            heap.tiny = save_max;
        else if (current == heap.small)
            heap.small = save_max;
        else if (current == heap.large)
            heap.large = save_max;
        meta = current - heap.meta_size;
        munmap(meta, meta->size); // deallocate the memory.
    }
    pthread_mutex_unlock(&mutex);
}

void *realloc(void *ptr, size_t size)
{

    pthread_mutex_lock(&mutex);
    if (size < 1)
    {
        free(ptr);
        return (NULL);
    }
    if (ptr == NULL)
    {
        ptr = malloc(size);
        return (ptr);
    }
    if (!ptr_alloc_check(ptr))
    {
        return (NULL);
    }
    void *ret = NULL;
    t_metadata *meta = ptr - heap.meta_size;
    if (meta->next == NULL)
    {
        goto just_allocate;
    }
    t_metadata *nextmeta = meta->next - heap.meta_size;
    if (nextmeta->isFree == 'Y' && (meta->size - heap.meta_size) + (nextmeta->size) == size)
    { // if realloc will use exacly all the next free chunk
        meta->size += nextmeta->size;
        meta->next = nextmeta->next;
        nextmeta = meta->next - heap.meta_size;
        nextmeta->prev = ptr;
        ret = ptr;
    }
    else if (nextmeta->isFree == 'Y' && (meta->size - heap.meta_size) + (nextmeta->size) > size)
    { // if realloc will use less than the avalable free space
        void *save_next = nextmeta->next;
        size_t save_size = nextmeta->size;
        meta->next = ptr + size + heap.meta_size;
        t_metadata *midmeta = meta->next - heap.meta_size;
        midmeta->size = save_size - (size - (meta->size - heap.meta_size)); // new chunk, what's left
        midmeta->next = save_next;
        midmeta->prev = ptr;
        midmeta->isFree = 'Y';
        add_safe(midmeta);
        if (save_next != NULL)
        {
            nextmeta = save_next - heap.meta_size;
            nextmeta->prev = midmeta + heap.meta_size;
            meta->size = size + heap.meta_size;
        }
        ret = midmeta + heap.meta_size;
    }
    else
    {
    just_allocate:
        ret = malloc(size);
        size_t i = -1;
        size_t len = meta->size - heap.meta_size;
        char *dst = ret;
        char *src = ptr;
        while (++i < len)
        {
            dst[i] = src[i];
        }
        free(ptr);
    }
    pthread_mutex_unlock(&mutex);
    return (ret);
}