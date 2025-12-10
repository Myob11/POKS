#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

// SEMAFOR

void *fun1();

int semafor = 0;

int main(){

    pthread_t nit1, nit2;
    char *msg1 = "11111111111111111111111";
    char *msg2 = "22222222222";

    pthread_create(&nit1, NULL, &fun1, (void*)msg1);
    pthread_create(&nit2, NULL, &fun1, (void*)msg2);   

    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);

    printf("KONEC\n");

    return 0;
}

void *fun1(void *arg){
    int i,j,b;

    while(semafor); // nit ki tukaj caka ubistvu busy waita
    //kriticen del kode
    semafor = 1; //zaklenemo semafor

    for(int i = 0; i < 40; i++){
            printf("msg niti je: %s\n", (char *)arg);
            for(int n = 0; n < 10000; n++){
                int a = 2*32/5;
            }    
    }

    semafor = 0; //odklenemo semafor 
    for(int c = 0; c < 1000; c++){
        int a = 2*32/5;
        sleep(30); //da lahko vidimo riti v in njihove prioritete
    }
    return NULL;
}


