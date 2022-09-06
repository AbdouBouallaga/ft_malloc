
#include "test.h"

int main(){
    char *str1 = (char *)malloc(sizeof(char)*170);
    // free(str1);
    char *str2 = (char *)malloc(sizeof(char)*200);
    char *str4 = (char *)malloc(sizeof(char)*168);
    char *str3 = (char *)malloc(sizeof(char)*250);
    show_alloc_mem();
    free(str4);
    show_alloc_mem();
    int i = -1;
    while(++i < 169){
        str1[i] = '0';
    }
    str1[i] = '\0';
    i = -1;
    while(++i < 249){
        str3[i] = 'A';
    }
    str3[i] = '\0';
    i = -1;
    while(++i < 199){
        str2[i] = 'a';
    }
    str2[i] = '\0';
    str2 = realloc(str2, 500);
    show_alloc_mem();
    // show_alloc_mem_ex(str2, 0);
    return(0);
}