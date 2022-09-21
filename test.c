
#include <strings.h>
#include <stdlib.h>
#include "./libft/libft.h"
#include "./inc/malloc.h"


int main(){
    int len = 2;
    char *val[len];
    int i = -1;
    char fill = 'A';
    int j;
    size_t size = 49;
    t_metadata *meta;
    while(++i < len)
    {
        j = -1;
        val[i] = malloc(size);
        ft_putnbr(i);
        ft_putchar('\n');
        show_alloc_mem();
        while (++j < size+200)
            val[i][j] = fill;
        val[i][j] = '\0';
        meta = val[i] - sizeof(t_metadata);
        ft_putchar(meta->safe_one);
        ft_putchar(meta->safe_two);
        ft_putchar(meta->safe_three);
        // meta->isFree<<=8;
        show_alloc_mem_ex(val[i], 1);
        show_alloc_mem();

        // if (i == 8){
        //     show_alloc_mem_ex(val[i-2], 1);
        //     show_alloc_mem_ex(val[i-1], 1);
        //     show_alloc_mem_ex(val[i], 1);
        // }
    }
    show_alloc_mem_ex(val[1], 1);
    show_alloc_mem();
    free(val[1]);
    // i = -1;
    // while (++i < 1001){
    //     printf("%c")
    // }
    // show_alloc_mem();
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
    // char *str1 = (char *)malloc(sizeof(char)*170);
    // char *str2 = (char *)malloc(sizeof(char)*200);
    // char *str3 = (char *)malloc(sizeof(char)*250);
    // char *str4 = (char *)malloc(sizeof(char)*168);
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
    // ft_putstr("realloc 2nd allocation with size 500\n\n");
    // str2 = realloc(str2, 500);
    // show_alloc_mem_ex(str2, 0);
    // show_alloc_mem();
    // ft_putstr("free the 4th and 1st allocation to show memory defragmentation\n");
    // free(str1);
    // free(str4);
    // show_alloc_mem();
//     return(0);
// }
