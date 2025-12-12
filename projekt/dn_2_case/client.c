#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
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

int main(int argc, char *argv[]) {

    int serialCounter = 0;
    struct hostent *hp;
    int sock, length, n;
    struct sockaddr_in server, from;
    socklen_t fromlen;
    char buf[1024];

    if (argc != 2) {
        printf("Nekaj je narobe\n");
        exit(1);
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        error("Napaka pri odpiranju soketa\n");
    }
    bzero((char *)&server, sizeof(server));
    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if(hp == NULL) {
        error("Nepoznan host\n");
    }
    bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
    server.sin_port = htons(5069); // port
    length = sizeof(struct sockaddr_in);

    while(1) {

        int getInd = 0;

        // pošlji sporočilo
        printf("Vnesi sporočilo: ");
        bzero(buf, sizeof(buf));
        if(fgets(buf, sizeof(buf), stdin) == NULL) {
            break;
        }
        if(strncmp(buf, "GET", 3) == 0 && (buf[3] == '\0' || buf[3] == '\n')) {
            getInd = 1;
            printf("zahtevali smo UUID\n");
        }

        printf("Pošiljam sporočilo: %s\n", buf);
        n = sendto(sock, buf, strlen(buf) + 1, 0, (const struct sockaddr *)&server, length);
        if(n < 0){
            error("sendto\n");  
        }
        if(buf[0] == 'X'){
            break;
        }

        // prejem sporočila
        fromlen = sizeof(struct sockaddr_in);
        n = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&from, &fromlen);
        if(n < 0){
            error("recvfrom\n");
        }
        
        if(getInd == 1){

            char *uuid_part = strchr(buf, '-') + 1; // pointer premaknemo na mesto kjer je najden "-" + 1 
                                                    // in s tem smo na prvem mestu UUID-ja
            char *crc_part = strrchr(buf, ' ') + 1; // pointer premaknemo na mesto kjer je najden zadnji presledek + 1
                                                    // in s tem smo na prvem mestu CRC kode

            // printf("uuid del: %s\n", uuid_part);
            // printf("crc del: %s\n", crc_part);
            
            char uuid_to_check[37];
            strncpy(uuid_to_check, uuid_part, 36);
            
            unsigned int calculated_crc = crc32(uuid_to_check, strlen(uuid_to_check));
            unsigned int received_crc = strtoul(crc_part, NULL, 16);
            
            if(calculated_crc == received_crc) {
            printf("CRC koda SE ujema\n");
            serialCounter++;
            printf("Zaporedna številka zahteve: %d\n", serialCounter);
            } 
            else {
            printf("CRC koda SE NE ujema: pričakovana %08X, prejeta %08X\n", calculated_crc, received_crc);
            }

            // pošlji potrditev PREJETO + crc del
            bzero(buf, sizeof(buf));
            sprintf(buf, "PREJETO %08X", calculated_crc);

            printf("Pošiljam sporočilo: %s\n", buf);
            n = sendto(sock, buf, strlen(buf) + 1, 0, (const struct sockaddr *)&server, length);
            if(n < 0){
                error("sendto\n");  
            }
        }

        printf("Prejeto sporočilo: %s\n", buf);

        if(buf[0] == 'X'){
            break;
        }
    }
    close(sock);
    return 0;          
}

