
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

struct arg_struct{
 int arg1;
 int arg2;
};

int main(){

    pthread_t nit1, nit2, nit3;
    struct arg_struct args1, args2, args3;

    args1.arg1 = 50; // prejsnji A
    args1.arg2 = 20; // prejsnji B
    args2.arg1 = 40; // prejsnji C
    args2.arg2 = 23; // prejsnji D
    args3.arg1 = 12; // prejsnji E
    args3.arg2 = 22; // prejsnji F
    
    int AB = 0, CD = 0 , EF = 0;
    int R = 0;

    pthread_create(&nit1, NULL, &fun1, (void *)&args1);
    pthread_create(&nit2, NULL, &fun2, (void *)&args2);   
    pthread_create(&nit3, NULL, &fun3, (void *)&args3); 
    

    pthread_join(nit1, (void **)&AB);
    pthread_join(nit2, (void **)&CD);
    pthread_join(nit3, (void **)&EF);

    R = AB * CD * EF;
    printf("rezultat je: %d\n", R);
  
    return 0;
}

void *fun1(void *arguments){
    struct arg_struct *args = arguments;
    int AB = (args->arg1) + (args->arg2);
    sleep(20);
    pthread_exit((void *)AB);   
}

void *fun2(void *arguments){
    struct arg_struct *args = arguments;
    int CD = (args->arg1) - (args->arg2);
    sleep(20);
    pthread_exit((void *)CD);   
}

void *fun3(void *arguments){
    struct arg_struct *args = arguments;
    int EF = (args->arg1) + (args->arg2);
    sleep(20);
    pthread_exit((void *)EF);
       
}