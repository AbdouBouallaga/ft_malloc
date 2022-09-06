
#include "test.h"

int main(){
    char *str1 = (char *)malloc(sizeof(char)*170);
    char *str2 = (char *)malloc(sizeof(char)*200);
    char *str3 = (char *)malloc(sizeof(char)*250);
    char *str4 = (char *)malloc(sizeof(char)*168);
    ft_putstr("Show heap status\n");
    show_alloc_mem();
    ft_putstr("Free the 3th allocation and Show heap status\n");
    free(str3);
    show_alloc_mem();
    int i = -1;
    ft_putstr("Fill 1st allocation with '0'\n");
    while(++i < 169){
        str1[i] = '0';
    }
    str1[i] = '\0';
    show_alloc_mem_ex(str1, 0);
    i = -1;
    ft_putstr("Fill 2st allocation with 'a'\n");
    while(++i < 199){
        str2[i] = 'a';
    }
    str2[i] = '\0';
    show_alloc_mem_ex(str2, 0);
    i = -1;
    ft_putstr(">>>>TEST REALLOC<<<<\n ");
    ft_putstr("Show heap status\n");
    show_alloc_mem();
    show_alloc_mem_ex(str2, 0);
    ft_putstr("realloc 2nd allocation with size 500\n\n");
    str2 = realloc(str2, 500);
    show_alloc_mem_ex(str2, 0);
    show_alloc_mem();
    ft_putstr("free the 4th and 1st allocation to show memory defragmentation\n");
    free(str1);
    free(str4);
    show_alloc_mem();
    return(0);
}