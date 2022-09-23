
#include <strings.h>
#include <stdlib.h>
// #include "./libft/libft.h"
#include "./inc/malloc.h"

int     check_safe(t_metadata *ptr)
{
    if (ptr->safe_one == '/' && ptr->safe_two == '@' && ptr->safe_three == '\\'){
        return(1);
    }
    return(0);
}

int main_2(void)
{
    int   i;
    char  *addr;

    i = 0;
    while (i < 1024) 
    {
        addr = (char*)malloc(1024);
        addr[0] = 42;
        free(addr); 
        i++; 
    }
    show_alloc_mem();
    return (0);
}

int main_m(){
    int len = 400;
    char *val[len+1000];
    int i = 0;
    char fill = 'A';
    int j;
    int s = 3;
    size_t size[3] = {376, 377, 378};
    t_metadata *meta;
    val[i] = malloc(size[s%3]);
    j = -1;
    while (++j < size[s%3]-1)
            val[i][j] = fill;
    val[i][j] = 0xff;
    while(++i < len)
    {
        j = -1;
        val[i] = malloc(size[s%3]);
        // ft_putnbr(i);
        // ft_putchar('\n');
        // show_alloc_mem();

        while (++j < size[s%3]-1)
            val[i][j] = fill;
        val[i][j] = 0xff;

        size[s%3]+=1;
        // meta = val[i] - sizeof(t_metadata);
        // if (!check_safe(meta)){
        //     ft_putstr("fucked\n");
        //     show_alloc_mem_ex(val[i-1], 1);
        //     show_alloc_mem_ex(val[i], 1);
        //     return(0);
        // }
        // ft_putchar(meta->safe_one);
        // ft_putchar(meta->safe_two);
        // ft_putchar(meta->safe_three);
        // meta->isFree<<=8;
        // show_alloc_mem_ex(val[i], 1);
        // show_alloc_mem();

        // if (i == 8){
        //     show_alloc_mem_ex(val[i-2], 1);
        //     show_alloc_mem_ex(val[i-1], 1);
        //     show_alloc_mem_ex(val[i], 1);
        // }
        s++;
    }
    show_alloc_mem();
    while (i >= 0)
    {
        free(val[i]);
        i--;
    }
    i-=1;
    show_alloc_mem();

    return(0);
}

int main_s(){
    int len = 5;
    char *val[len+1000];
    int i = 0;
    char fill = 'A';
    int j;
    int s = 3;
    size_t size = 2;
    t_metadata *meta;
    val[i] = malloc(size);
    j = -1;
    while (++j < size-1)
            val[i][j] = fill;
    val[i][j] = 0xff;
    while(++i < len)
    {
        j = -1;
        val[i] = malloc(size);
        // ft_putnbr(i);
        // ft_putchar('\n');
        // show_alloc_mem();

        // while (++j < size[s%3]-1)
        //     val[i][j] = fill;
        // val[i][j] = 0xff;

        // size+=1;
        // meta = val[i] - sizeof(t_metadata);
        // ft_putchar(meta->safe_one);
        // ft_putchar(meta->safe_two);
        // ft_putchar(meta->safe_three);
        // meta->isFree<<=8;
        // show_alloc_mem_ex(val[i], 1);
        // show_alloc_mem();

        // if (i == 8){
        //     show_alloc_mem_ex(val[i-2], 1);
        //     show_alloc_mem_ex(val[i-1], 1);
        //     show_alloc_mem_ex(val[i], 1);
        // }
        s++;
    }
    show_alloc_mem();
    i-=1;
    while (i >= 0)
    {
        free(val[i]);
        i--;
    }
    show_alloc_mem();
    // meta = val[i] - sizeof(t_metadata);
    return(0);
}
int main(){
    main_s();
    // main_m();
    // main_2();
    return(0);
}

// int main(){
// 	void *test = malloc(736);
// 	void *e = malloc(32);
// 	void *e1= malloc(32);
// 	void *e2 = malloc(32);
// 	void *e3 = malloc(32);
// 	void *e4 = malloc(32);
// 	void *e5 = malloc(32);
// 	void *e6 = malloc(32);
// 	show_alloc_mem();
    // char *str1 = (char *)malloc(size[s%3]of(char)*170);
    // char *str2 = (char *)malloc(size[s%3]of(char)*200);
    // char *str3 = (char *)malloc(size[s%3]of(char)*250);
    // char *str4 = (char *)malloc(size[s%3]of(char)*168);
    // ft_putstr("Show heap status\n");
    // show_alloc_mem();
    // ft_putstr("Free the 3th allocation and Show heap status\n");
    // free(str3);
    // show_alloc_mem();
    // int i = -1;
    // ft_putstr("Fill 1st allocation with '0'\n");
    // while(++i < 169){
    //     str1[i] = '0';
    // }
    // str1[i] = '\0';
    // show_alloc_mem_ex(str1, 0);
    // i = -1;
    // ft_putstr("Fill 2st allocation with 'a'\n");
    // while(++i < 199){
    //     str2[i] = 'a';
    // }
    // str2[i] = '\0';
    // show_alloc_mem_ex(str2, 0);
    // i = -1;
    // ft_putstr(">>>>TEST REALLOC<<<<\n ");
    // ft_putstr("Show heap status\n");
    // show_alloc_mem();
    // show_alloc_mem_ex(str2, 0);
    // ft_putstr("realloc 2nd allocation with size[s%3] 500\n\n");
    // str2 = realloc(str2, 500);
    // show_alloc_mem_ex(str2, 0);
    // show_alloc_mem();
    // ft_putstr("free the 4th and 1st allocation to show memory defragmentation\n");
    // free(str1);
    // free(str4);
    // show_alloc_mem();
//     return(0);
// }
