#include <stdio.h>

int main(){
    unsigned int i = -1;
    while(++i < 655550){
        printf("%ld", i);
    }
    return(0);
}