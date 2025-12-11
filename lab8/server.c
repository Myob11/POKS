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

int main(int argc, char *argv[]){
    
    int sock, length, n;
    struct sockaddr_in server, from;
    socklen_t fromlen;
    char buf[1024];

    if(argc < 2){
        fprintf(stderr,"ERROR, nisi dal porta\n");
        exit(1);
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        error("Napaka pri odpiranju soketa\n");
    }

    length = sizeof(server);
    bzero(&server, length);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    server.sin_port = htons(atoi(argv[1])); // podamo kot argument
    
    if(bind(sock, (struct sockaddr *)&server, length) < 0){ // ali IP ali port ni free
        error("Napaka pri bind\n"); // bindamo socket konstantno za naso uporabo
    }
    
    fromlen = sizeof(struct sockaddr_in);
    
    while(1) {
        
        printf("Čakam na sporočilo...\n");
        n = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&from, &fromlen);
        if (n < 0) {
            error("recvfrom\n");
        }

        printf("Prejeto sporočilo: %s\n", buf);
        if(buf[0] == 'X'){
            break;
        }   

        printf("Pošiljam sporočilo: \n");
        bzero(buf, sizeof(buf));
        if(fgets(buf, sizeof(buf), stdin) == NULL) {
            break;
        }

        printf("Pošiljam: %s\n", buf);
        n = sendto(sock, buf, strlen(buf) + 1, 0, (struct sockaddr *)&from, fromlen);
        if (n < 0) {
            error("sendto\n");
        }

        if(buf[0] == 'X'){
            break;
        }  
        
        close(sock);
        return 0;
}