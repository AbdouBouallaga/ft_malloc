
#include "test.h"

/////////////////////////////////////////test part//////////////////////////////////////////

void    write_hex(int ch){
    int a = ch/16;
    int i = -1;
    while (++i < 2){
        if (a < 10)
            a = '0' + a;
        else
            a = 'A' + a - 10;
        ft_putchar(a);
        a = ch%16;
    }
}

// void print_bytes(void *ptr, int size) 
// {
//     unsigned char *p = ptr;
//     int i;
//     int c = 0;
//     int d = 0;
//     for (i=0; i<size; i++) {
//         printf("%02hhX ", p[i]);
//         c++;
//         d++;
//         if (c == 4){
//             printf(" ");
//             c = 0;
//         }
//         if (d == 16){
//             printf("\n");
//             d = 0;
//         }

//     }
//     printf("\n");
// }
void print_bytes(void *ptr, int size) 
{
    unsigned char *p = ptr;
    int i;
    int c = 0;
    int d = 0;
    for (i=0; i<size; i++) {
        write_hex((p[i]));
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

// int main(){
//     int max = 6;
//     int i = -1;
//     // void *ptr;
//     void *ptr0[max];
//     void *ptr1[max];
//     void *ptr2[max];
//     int c = 1;
//     // ptr0[0] = malloc(sizeof(char)*10);
//     while (++i < max){
//         ptr0[i] = malloc(163);
//         ptr1[i] = malloc(16384);
//         ptr2[i] = malloc(26384);
//     }
//     display();
//     // free(ptr2[4]);
//     free(ptr2[0]);
//     free(ptr2[2]);
//     free(ptr2[3]);
//     // free(ptr2[]);
//     // display();
//     ptr2[4] = malloc(56384);
//     display();
//     return (0);
// }

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