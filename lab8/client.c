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

int main(int argc, char *argv[]) {

    struct hostent *hp;
    int sock, length, n;
    struct sockaddr_in server, from;
    socklen_t fromlen;
    char buf[1024];

    if (argc != 3) {
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
    server.sin_port = htons(atoi(argv[2]));
}

