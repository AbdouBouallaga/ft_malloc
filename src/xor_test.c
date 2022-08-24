#include <stdlib.h>
#include <stdio.h>


typedef struct      s_list{
    int             data;
    long            link;
}                   t_list;

t_list    *add_blk(int data){
    t_list *ret = (long)malloc(sizeof(t_list));
    printf("%p\n", ret);
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
    int i = 2;
    t_list *head = add_blk(1);
    t_list *prev;
    t_list *cur;
    t_list *next = add_blk(2);
    t_list *temp;
    head->link = xor(NULL, next);
    cur = next;
    prev = head;
    while (++i < 11){
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
    while(++i < 11){
        printf("%p\n", cur);
        printf("%d\n", cur->data);
        next = tr_for(cur, prev);
        prev = cur;
        cur = next;
    }
    return(0);
}