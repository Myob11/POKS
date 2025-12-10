
// R = (A + B) * (C - D) * (E + F)

// A + B = nit 1
// C - D = nit 2
// E + F = nit 3

// globalne spremenljivke struct(arg array)

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *fun1();
void *fun2();
void *fun3();

int A = 50;
int B = 20;
int C = 40;
int D = 23;
int E = 12;
int F = 22;

int AB = 0, CD = 0 , EF = 0;

int main(){

    pthread_t nit1, nit2, nit3;
    int R = 0;

    pthread_create(&nit1, NULL, &fun1, NULL);
    pthread_create(&nit2, NULL, &fun2, NULL);   
    pthread_create(&nit3, NULL, &fun3, NULL);

    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);
    pthread_join(nit3, NULL);

    R = AB * CD * EF;
    printf("rezultat je: %d\n", R);
  
    return 0;
}

void *fun1(){
    AB = A + B;
    sleep(20);   
}

void *fun2(){
    CD = C - D;  
    sleep(20);   
}

void *fun3(){
    EF = E + F;
    sleep(20);   
}