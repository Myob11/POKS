#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

/* ===================== UUID + CRC ===================== */

char *genUUID(void) {
    char *uuid = malloc(37);
    if (!uuid) return NULL;

    const char *hex = "0123456789abcdef";
    for (int i = 0; i < 36; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23)
            uuid[i] = '-';
        else if (i == 14)
            uuid[i] = '4';
        else if (i == 19)
            uuid[i] = '9';
        else
            uuid[i] = hex[rand() % 16];
    }
    uuid[36] = '\0';
    return uuid;
}

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

/* ===================== GLOBAL SYNC ===================== */

unsigned int global_seq = 0;        // global serial counter
int get_in_progress = 0;            // is a GET currently being processed
unsigned int thread_counter = 0;    // unique thread ID

pthread_mutex_t get_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  get_cond  = PTHREAD_COND_INITIALIZER;

/* ===================== THREAD HANDLER ===================== */

void *connection_handler(void *socket_desc) {

    int sock = *(int *)socket_desc;
    free(socket_desc);

    unsigned int thread_id;

    pthread_mutex_lock(&get_mutex);
    thread_id = ++thread_counter; // assign unique thread ID
    pthread_mutex_unlock(&get_mutex);

    printf("[Thread #%u] Handling client\n", thread_id);

    char buf[1024];

    while (1) {
        bzero(buf, sizeof(buf));
        int n = recv(sock, buf, sizeof(buf) - 1, 0);
        if (n <= 0) {
            printf("[Thread #%u] Client disconnected\n", thread_id);
            break;
        }

        buf[n] = '\0';
        printf("[Thread #%u] Received: %s\n", thread_id, buf);

        /* ===================== GET ===================== */
        if (strncmp(buf, "GET", 3) == 0 &&
           (buf[3] == '\n' || buf[3] == '\0')) {

            /* ---- WAIT FOR TURN ---- */
            pthread_mutex_lock(&get_mutex);
            while (get_in_progress)
                pthread_cond_wait(&get_cond, &get_mutex);

            get_in_progress = 1;
            unsigned int my_seq = global_seq + 1;
            pthread_mutex_unlock(&get_mutex);

            printf("[Thread #%u] Processing GET (seq: %u)\n", thread_id, my_seq);

            /* ---- GENERATE UUID ---- */
            char *uuid = genUUID();
            unsigned int crc = crc32(uuid, strlen(uuid));

            char response[1024];
            sprintf(response, "%08u-%s %08X", my_seq, uuid, crc);
            send(sock, response, strlen(response), 0);

            printf("[Thread #%u] Sent: %s\n", thread_id, response);
            free(uuid);

            sleep(5);

            /* ---- WAIT FOR PREJETO ---- */
            bzero(buf, sizeof(buf));
            n = recv(sock, buf, sizeof(buf) - 1, 0);
            if (n <= 0) break;

            buf[n] = '\0';
            printf("[Thread #%u] Received: %s\n", thread_id, buf);

            if (strncmp(buf, "PREJETO", 7) == 0) {

                char *crc_part = strchr(buf, ' ');
                if (crc_part) crc_part++;

                char expected_crc[9];
                sprintf(expected_crc, "%08X", crc);

                pthread_mutex_lock(&get_mutex);

                if (crc_part && strcmp(crc_part, expected_crc) == 0) {
                    global_seq++;
                    printf("[Thread #%u] CRC OK → counter = %u\n", thread_id, global_seq);
                } else {
                    send(sock, "NAPAKA 4900B4DB", 15, 0);
                    printf("[Thread #%u] CRC ERROR\n", thread_id);
                }

                get_in_progress = 0;
                pthread_cond_signal(&get_cond);
                pthread_mutex_unlock(&get_mutex);
            }

            continue;
        }

        /* ===================== OTHER COMMANDS ===================== */
        send(sock, "NEPREPOZNAVEN UKAZ", 18, 0);
    }

    close(sock);
    printf("[Thread #%u] Closed connection\n", thread_id);
    return NULL;
}

/* ===================== MAIN ===================== */

int main() {

    srand(time(NULL));

    int socket_desc, client_sock;
    struct sockaddr_in server, client;
    socklen_t c = sizeof(client);

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0) {
        perror("socket");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5095);

    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind");
        return 1;
    }

    listen(socket_desc, 5);
    puts("Waiting for connections...");

    while ((client_sock = accept(socket_desc,
            (struct sockaddr *)&client, &c))) {

        pthread_t tid;
        int *new_sock = malloc(sizeof(int));
        *new_sock = client_sock;

        pthread_create(&tid, NULL, connection_handler, new_sock);
        pthread_detach(tid);

        /* Print info about new connection */
        pthread_mutex_lock(&get_mutex);
        unsigned int current_thread_id = thread_counter + 1; // next thread id
        pthread_mutex_unlock(&get_mutex);

        printf("[Thread #%u] New client connected from %s:%d\n",
               current_thread_id,
               inet_ntoa(client.sin_addr),
               ntohs(client.sin_port));
    }

    close(socket_desc);
    return 0;
}
