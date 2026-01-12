/* Creates a datagram UDP server.  The port number is passed as an argument. */
/* This server runs forever */

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <time.h>

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


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {

    srand(time(NULL));
    int sock, length, n;
    struct sockaddr_in server, from;
    socklen_t fromlen;
    char buf[1024];
    int serialCounter = 0;
   
   sock = socket(AF_INET, SOCK_DGRAM, 0);  /* create new socket */

   if(sock < 0) error("Opening socket");

   length = sizeof(server);
   bzero(&server, length);
   server.sin_family = AF_INET;  /* Internet */
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port = htons(5069); /* port */

   if (bind(sock, (struct sockaddr *)&server, length) < 0) /* bind socket with port */
       error("binding");

   fromlen = sizeof(struct sockaddr_in);

   printf("-----------------waiting for data on port UDP 5069 -----------------\n");

   while (1) {

       n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&from, &fromlen);
       if(n < 0) error("recvfrom");

        printf("Received a datagram: %s\n", buf);

        if(buf[0] == 'X'){
            printf("Connection closed by client.\n");
            printf("------------------------------ closing -----------------------------\n");
            break;
        }
                
        
        // dodani del za CRC32 preverjanje in odziv na GET
        if(strncmp(buf, "GET", 3) == 0 && (buf[3] == '\0' || buf[3] == '\n')) {
            static unsigned int seq_num = 0;
            char *uuid = genUUID();
            char response[1024];
            
            seq_num++;
            
            unsigned int crc = crc32(uuid, strlen(uuid));
            sprintf(response, "%08u-%s %08X", seq_num, uuid, crc);
            
            bzero(buf, 1024);
            n = sendto(sock, response, strlen(response) + 1, 0,
              (struct sockaddr *)&from, fromlen);
            if(n < 0) error("sendto uuid");
            printf("Sent %s\n", response);
            printf("\n");
            free(uuid);

            char crc_char[9];
            sprintf(crc_char, "%08X", crc);
            

            while(1){
                
                n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&from, &fromlen);
                if(n < 0) error("recvfrom");
                printf("Received a datagram: %s\n", buf);
                printf("\n");

                if(strncmp(buf, "PREJETO", 7) == 0){
                    
                    char *crc_part = strchr(buf, ' ') + 1; // postavimo se na mesto kjer je uuid v replyu
                    
                    if(strcmp(crc_part, crc_char) == 0){
                        serialCounter++;
                        printf("CRC32 preverjanje uspešno. Števec: %d\n", serialCounter);
                        printf("\n");
                        break;
                    }
                    else{
                        printf("CRC32 preverjanje neuspešno\n");
                        bzero(buf, 1024);
                        char odgovor[1024];
                        sprintf(odgovor, "Napacna CRC koda: %s", crc_part);
                        n = sendto(sock, odgovor, strlen(odgovor) + 1, 0,
                        (struct sockaddr *)&from, fromlen);
                        if(n < 0) error("sendto uuid");
                        printf("Sent %s\n", odgovor);
                        printf("\n");
                    }

                }         
            }
        }
        else {
            char response[1024] = "NEPREPOZNAVEN UKAZ";
            n = sendto(sock, response, strlen(response) + 1, 0,
              (struct sockaddr *)&from, fromlen);
            if(n < 0) error("sendto neprepoznan ukaz");
            printf("Sent %s\n", response);
        }
        
   }
   
   close(sock);
   return 0;
}

