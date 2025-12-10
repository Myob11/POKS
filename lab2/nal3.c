#include  <stdio.h>

int main(){

    int x = 5;
    int *p = &x;
    printf("%p\n:", &x);
    printf("%d\n:", *p);
    printf("%p\n:", p);

}