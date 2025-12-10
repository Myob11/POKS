#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(){

    srand(time(NULL)); //inicializiramo čas, brez tega bi vedno dobili isto "nakljucnost"
    printf("Ugani število med 1 in 30. \n");
    
    int stevilo = rand()%30+1;
    int u;
    int poizkus = 5;

    do {

        scanf("Vnesi število: ");
        scanf("%d", &u);

        if(u == stevilo){
            printf("Bravo!!\n");
            return 0;
        }
        if(u < stevilo){
            printf("Vpisou si premajhno vrednost! \n");
        }
        if(u > stevilo){
            printf("Vpisou si preveliko vrednost! \n");
        }

        poizkus --;
        printf("Na vlojo imaš še %d poizkusov!\n", poizkus);
        
       

    }while (poizkus>0);
    printf("Pravilno št. je %d\n", stevilo);

    return 0;
    
}