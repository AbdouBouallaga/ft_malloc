#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "./libft/libft.h"


#define PROT    PROT_READ | PROT_WRITE
#define MAP     MAP_PRIVATE | MAP_ANONYMOUS

void    write_hex(int ch)
{
    if (ch >= 0 && ch < 10)
        ch = '0' + ch;
    else
        ch = 'a' + ch - 10;
    ft_putchar(ch);
}

void    write_address(unsigned long long num)
{
    int tab[16];
    int i = 0;
    if ((void*)num == NULL)
        ft_putstr("NULL\t");
    else{
        ft_putstr("0x");
        while (num){
            tab[i] = num%16;
            num = num/16;
            i++;
        }
        i--;
        while (i >= 0){
            write_hex(tab[i]);
            i--;
        }
    }
}

int main(){
    
    int page = getpagesize();
    int ret;
    int i = 0;
    char **addr = mmap(0, page, PROT, MAP,0, 0);
    while (i < 1000) 
    {
        addr[i] = mmap(0, page, PROT, MAP,0, 0);
        write_address((unsigned long long)addr[i]);
        ft_putchar('\n');
        addr[i][0] = 42;
        ft_putnbr(addr[i][0]);
        ft_putchar('\n');
        ret = munmap(addr[i], page);
        ft_putstr("munmap return\n");
        ft_putnbr(ret);
        ft_putchar('\n');
        write_address((unsigned long long)addr[i]);
        ft_putchar('\n');
        // ft_putnbr(addr[i][0]);
        i++;
    }
    // while (i+1) 
    // {   
    //      write_address((unsigned long long)addr[i]);
    //     ft_putchar('\n');
    //     int ret = munmap(addr[i], page);
    //     ft_putnbr(ret);
    //     i--;
    // }
    return(0);
}