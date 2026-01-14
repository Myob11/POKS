#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

unsigned int crc32(const char *data, size_t length) {
    unsigned int crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; i++) {
        crc ^= (unsigned char)data[i];
        for (int j = 0; j < 8; j++) {
            unsigned int mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
    }
    return ~crc;
}

int main() {

    int request_number = 0;
    int sock;
    struct sockaddr_in server;
    char buffer[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Cannot create socket\n");
        return 1;
    }
    printf("Socket created\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5095);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Cannot connect to server\n");
        return 1;
    }
    printf("Connected to server\n");

    while (1) {

        int is_get = 0;

        printf("Enter command: ");
        bzero(buffer, sizeof(buffer));

        if (!fgets(buffer, sizeof(buffer), stdin))
            break;

        if (strncmp(buffer, "GET", 3) == 0)
            is_get = 1;

        if (send(sock, buffer, strlen(buffer), 0) < 0) {
            printf("Cannot send data\n");
            break;
        }

        if (buffer[0] == 'X')
            break;

        bzero(buffer, sizeof(buffer));
        int n = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) {
            printf("Server disconnected\n");
            break;
        }
        buffer[n] = '\0';
        printf("Server says: %s\n", buffer);

        if (is_get) {

            if (strncmp(buffer, "NAPAKA", 6) == 0) {
                printf("Server error: %s\n", buffer);
                continue;
            }

            char *dash = strchr(buffer, '-');
            char *space = strrchr(buffer, ' ');

            if (!dash || !space || space < dash) {
                printf("Invalid server response\n");
                continue;
            }

            char uuid[37];
            strncpy(uuid, dash + 1, 36);
            uuid[36] = '\0';

            unsigned int received_crc = strtoul(space + 1, NULL, 16);
            unsigned int calc_crc = crc32(uuid, strlen(uuid));

            if (calc_crc == received_crc) {
                printf("CRC matches!\n");
                request_number++;
                printf("Request number: %d\n", request_number);
            } else {
                printf("CRC does NOT match (calc=%08X recv=%08X)\n", calc_crc, received_crc);
            }

            bzero(buffer, sizeof(buffer));
            sprintf(buffer, "PREJETO %08X", calc_crc);
            printf("Sending: %s\n", buffer);
            if (send(sock, buffer, strlen(buffer), 0) < 0) {
                printf("Cannot send data\n");
                break;
            }
        }
    }

    close(sock);
    return 0;
}
