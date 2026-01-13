
#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>
#include <time.h>
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


int main(int argc , char *argv[]) {   

    int serialCounter = 0;
    int sock;
	struct sockaddr_in server;
    char buf[1024];
	char exitMSG[1024] = "EXIT";
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1){
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 5095 );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
		perror("connect failed. Error");
		return 1;
	}
	
	puts("Connected\n");
	
   while (1) {

        int getInd = 0;

        printf("Vnesi sporočilo: ");
        bzero(buf, sizeof(buf));

        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            break;
        }

        if (strncmp(buf, "GET", 3) == 0 && (buf[3] == '\n' || buf[3] == '\0')) {
            getInd = 1;
            printf("\nZahtevali smo UUID\n");
        }

        printf("\nPošiljam sporočilo: %s\n", buf);

        if (send(sock, buf, strlen(buf), 0) < 0) {
            perror("send");
            break;
        }

        if (buf[0] == 'X') {
            printf("---------------- closing ----------------\n");
            break;
        }

        bzero(buf, sizeof(buf));

        int n = recv(sock, buf, sizeof(buf) - 1, 0);
        if (n <= 0) {
            perror("recv");
            break;
        }

        buf[n] = '\0';

        if (getInd == 1) {

            char *uuid_part = strchr(buf, '-') + 1;
            char *crc_part  = strrchr(buf, ' ') + 1;

            char uuid_to_check[37];
            strncpy(uuid_to_check, uuid_part, 36);
            uuid_to_check[36] = '\0';

            unsigned int calculated_crc = crc32(uuid_to_check, strlen(uuid_to_check));
            unsigned int received_crc   = strtoul(crc_part, NULL, 16);

            if (calculated_crc == received_crc) {
                printf("CRC koda SE ujema\n");
                serialCounter++;
                printf("Zaporedna številka zahteve: %d\n", serialCounter);
            } else {
                printf("CRC koda SE NE ujema: pričakovana %08X, prejeta %08X\n",
                    calculated_crc, received_crc);
            }

            bzero(buf, sizeof(buf));
            sprintf(buf, "PREJETO %08X", calculated_crc);

            printf("\nPošiljam sporočilo: %s\n\n", buf);

            if (send(sock, buf, strlen(buf), 0) < 0) {
                perror("send");
                break;
            }

            continue;
        }

        printf("Prejeto sporočilo: %s\n\n", buf);

    }
	
	close(sock);
	return 0;
}
