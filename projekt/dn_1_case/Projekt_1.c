
/*
    V svoji besedilni predstavitvi je 16 oktetov UUID predstavljenih kot 32 šestnajstiških števk, 
    prikazanih v petih skupinah, ločenih z vezaji, v obliki 8-4-4-4-12 (skupno 36 znakov). (32 
    šestnajstiških znakov in 4 vezaji). Na primer:

    123e4567-e89b-12d3-a456-426614174000 
    xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx 

    Za potrebe projekta bomo UUID razširili, tako da bo vseboval informacijo o zaporedni številki 
    UUID. Podatek o zaporedni številki serijske številke bo dolg 8 znakov, tako da zaporedni številki dodamo  
    Na primer: 

    00000001-xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx 
    00000002-xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx

    Spišite  program  za  generiranje  serijskih  številk.  Vsako  serijsko  številko  opremite  s  CRC32 
    kontrolno vsoto. Program naj izpiše prvih 100 serijskih številk in kontrolnih vsot ločenih s 
    presledki, na primer:  
    00000001-xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx 7d3a2b4e 
    00000002-xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx 233a6abe
*/

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>



char *genUUID(void) {
    char *uuid = malloc(37); // 36 znakov + \0
    if (!uuid) return NULL;

    const char *hex = "0123456789abcdef";
    for (int i = 0; i < 36; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            uuid[i] = '-';
        } else if (i == 14) {
            uuid[i] = '4'; 
        } else if (i == 19) {
            uuid[i] = '9';
        } else {
            uuid[i] = hex[rand() % 16];
        }
    }
    uuid[36] = '\0';
    return uuid;
}

unsigned int crc32(const char *data, size_t length) {
    unsigned int crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < length; i++) {
        unsigned char byte = (unsigned char)data[i];
        crc ^= (unsigned int)byte;
        for (int j = 0; j < 8; j++) {
            unsigned int mask = -(crc & 1u);
            crc = (crc >> 1) ^ (0xEDB88320u & mask);
        }
    }
    return ~crc;
}



int main(void) {

    srand(time(NULL));

    int koliko = 100;
    int zapSt[koliko];

    for(int i = 0; i < koliko; i++){
        zapSt[i] = i + 1;
        char *uuid = genUUID();
        printf("%08d-%s %08x\n", zapSt[i], uuid, crc32(uuid, strlen(uuid)));
        free(uuid);
    }

    return 0;
}




