#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

/* ===================== CRC ===================== */

unsigned int crc32(const char *data, size_t length) {
    unsigned int crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < length; i++) {
        crc ^= (unsigned char)data[i];
        for (int j = 0; j < 8; j++) {
            unsigned int mask = -(crc & 1u);
            crc = (crc >> 1) ^ (0xEDB88320u & mask);
        }
    }
    return ~crc;
}

/* ===================== MAIN ===================== */

int main(void) {

    int serialCounter = 0;
    int sock;
    struct sockaddr_in server;
    char buf[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(5095);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        return 1;
    }

    puts("Connected\n");

    while (1) {

        int isGET = 0;

        printf("Vnesi sporočilo: ");
        bzero(buf, sizeof(buf));

        if (!fgets(buf, sizeof(buf), stdin))
            break;

        if (strncmp(buf, "GET", 3) == 0 &&
            (buf[3] == '\n' || buf[3] == '\0')) {
            isGET = 1;
            printf("\nZahtevali smo UUID\n");
        }

        if (send(sock, buf, strlen(buf), 0) < 0) {
            perror("send");
            break;
        }

        if (buf[0] == 'X')
            break;

        bzero(buf, sizeof(buf));

        int n = recv(sock, buf, sizeof(buf) - 1, 0);
        if (n <= 0) {
            perror("recv");
            break;
        }

        buf[n] = '\0';
        printf("\nPrejeto: %s\n", buf);

        /* ===================== HANDLE GET RESPONSE ===================== */
        if (isGET) {

            /* ---- handle NAPAKA ---- */
            if (strncmp(buf, "NAPAKA", 6) == 0) {
                printf("Server reported error: %s\n\n", buf);
                continue;
            }

            /* ---- validate format ---- */
            char *dash = strchr(buf, '-');
            char *space = strrchr(buf, ' ');

            if (!dash || !space || space < dash) {
                printf("Neveljaven odgovor strežnika\n\n");
                continue;
            }

            char uuid[37];
            strncpy(uuid, dash + 1, 36);
            uuid[36] = '\0';

            unsigned int received_crc = strtoul(space + 1, NULL, 16);
            unsigned int calculated_crc = crc32(uuid, strlen(uuid));

            if (calculated_crc == received_crc) {
                printf("CRC koda SE ujema\n");
                serialCounter++;
                printf("Zaporedna številka zahteve: %d\n", serialCounter);
            } else {
                printf("CRC koda SE NE ujema (calc=%08X recv=%08X)\n",
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

        printf("\n");
    }

    close(sock);
    return 0;
}
