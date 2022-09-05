
#include "test.h"

int main(){
    char *str1 = (char *)malloc(sizeof(char)*17);
    // free(str1);
    char *str2 = (char *)malloc(sizeof(char)*25);
    char *str3 = (char *)malloc(sizeof(char)*25);
    show_alloc_mem();
    int i = -1;
    while(++i < 16){
        str1[i] = '0';
    }
    str1[i] = '\0';
    i = -1;
    while(++i < 24){
        str3[i] = 'A';
    }
    str3[i] = '\0';
    i = -1;
    while(++i < 24){
        str2[i] = 'a';
    }
    str2[i] = '\0';
    show_alloc_mem_ex(str2, 0);
    // str1[i] = '\0';
    // i = -1;
    // while(++i < 16){
    //     str2[i] = 'B';
    // }
    // show_alloc_mem();
    // printf("%d\n", sizeof(str1));
    // print_bytes(str1-sizeof(t_metadata), 17+sizeof(t_metadata));
    // printf("\n");
    // print_bytes(str2-sizeof(t_metadata), 25+sizeof(t_metadata));
    // printf("\n");
    // print_bytes(str3-sizeof(t_metadata), 25+sizeof(t_metadata));
    // str3 = realloc(str3, 45);
    // printf("\n");
    // print_bytes(str3-sizeof(t_metadata), 45+sizeof(t_metadata));
    // show_alloc_mem();
    return(0);
}