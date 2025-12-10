/*
medsebojno izključevanje - mutex --> zaklenemo del kode, ki je "nevaren"
damo kljucavnico na dolocen del kode in recemo dokler je ta kljucavnica aktivna noben 
ne more dostopati do te spremenljivke (lahko se zgodi dead/live lock)
*/

/*
2. VAJA - Medsebojno izključevanje - mutex
Uporaba mehanizma mutex izključuje pojavo nekonsistenc v primeru opravil, ki tekmujejo za skupni vir
(race conditions). Gre za primere, ko se zahteva neprekinjeno izvajanje določenega dela programa.
Napišite program kjer dve niti povečujeta isti števec.
Napišite program, ki inicializira dve novi niti, ki 2000 krat izvajata:
    • branje skupnega števca v lokalno spremenljivko,
    • zakasnitev (for zanka z matematiko cca. 10 ms),
    • povečanje lokalne spremenljivke in vpis v skupni števec.
Uporabite dva skupna števca :
    • prvi naj se uporablja brez zaščite,
    • povečevanje drugega naj bo zaščiteno z mutex.
Konec obeh niti sinhronizirajte s klicema funkcije join(), ki zadrži nadaljne izvajanje programa znotraj
funkcije main(). Izpišite vrednosti števcev.
Komentirajte, zakaj se števca razlikujeta.
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


void *fun1();
void *fun2();

int stevec = 0; 

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int main(){
    pthread_t nit1, nit2;
    int R = 0;

    pthread_create(&nit1, NULL, &fun1, NULL);
    pthread_create(&nit2, NULL, &fun2, NULL);   

    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);

    printf("vrednost stevca je: %d\n", stevec);
  
    return 0;
}

void *fun1(){
    int stevecint = 0;
    int i = 0;
    
    for(i = 0; i < 2000; i++){
        pthread_mutex_lock(&mutex1);
        // tu je dobro mesto za insertat mutex lock()
        // prva nit zaklene zanko, ga poveča, poveča stevec in ga na koncu odklene
        stevecint = stevec;
        stevecint++;
        printf("trenutni stevec je: %d\n", stevecint);
        int n = 0;
        for(n = 0; n < 100; n++){
            int a = 2*32/5;
            usleep(1);
        }
        stevec = stevecint;
        pthread_mutex_unlock(&mutex1);
        // mutex unlock()
    }
}   

void *fun2(){
    int stevecint = 0;
    int i = 0;
    
    for(i = 0; i < 2000; i++){
        pthread_mutex_lock(&mutex1);
        // tu je dobro mesto za insertat mutex lock()
        // prva nit zaklene zanko, ga poveča, poveča stevec in ga na koncu odklene
        stevecint = stevec;
        stevecint++;
        printf("trenutni stevec2    je: %d\n", stevecint);
        int n = 0;
        for(n = 0; n < 100; n++){
            int a = 2*32/5;
            usleep(1);
        }
        stevec = stevecint;
        pthread_mutex_unlock(&mutex1);
        // mutex unlock()
    }
}  
