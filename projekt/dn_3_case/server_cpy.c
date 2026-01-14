#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <time.h>

/* ===================== UUID + CRC ===================== */

// Generate a random UUID (unique ID)
char *generate_uuid() {
    char *uuid = malloc(37);  // 36 chars + null terminator
    if (!uuid) return NULL;

    const char *hex = "0123456789abcdef";
    for (int i = 0; i < 36; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23)
            uuid[i] = '-';  // Dashes in UUID
        else if (i == 14)
            uuid[i] = '4';  // Version 4 UUID
        else if (i == 19)
            uuid[i] = '9';  // Variant
        else
            uuid[i] = hex[rand() % 16];  // Random hex
    }
    uuid[36] = '\0';
    return uuid;
}

// Simple CRC32 calculation for data integrity
unsigned int calculate_crc32(const char *data, size_t length) {
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

/* ===================== GLOBAL VARIABLES ===================== */

unsigned int global_counter = 0;          // Counts successful GET commands
int get_in_progress = 0;                  // Only one GET at a time
unsigned int thread_count = 0;           // Number of client threads

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

/* ===================== HANDLE CLIENT ===================== */

void *handle_client(void *socket_ptr) {
    int client_socket = *(int *)socket_ptr;
    free(socket_ptr);

    // Assign a number to this thread
    pthread_mutex_lock(&mutex);
    unsigned int thread_number = ++thread_count;
    pthread_mutex_unlock(&mutex);

    printf("[Thread #%u] Client connected\n", thread_number);

    char buffer[1024];

    while (1) {
        bzero(buffer, sizeof(buffer));

        int received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (received <= 0) {
            printf("[Thread #%u] Client disconnected\n", thread_number);
            break;
        }

        buffer[received] = '\0';
        printf("[Thread #%u] Received: %s\n", thread_number, buffer);

        // ===================== SHUTDOWN =====================
        if (strncmp(buffer, "X", 1) == 0) {
            printf("[Thread #%u] Shutdown command received\n", thread_number);
            close(client_socket);
            exit(0);  // Stop the whole server
        }

        // ===================== GET COMMAND =====================
        if (strncmp(buffer, "GET", 3) == 0) {

            // Only one GET can happen at a time
            pthread_mutex_lock(&mutex);
            while (get_in_progress)
                pthread_cond_wait(&cond, &mutex);

            get_in_progress = 1;
            unsigned int my_sequence = global_counter + 1;
            pthread_mutex_unlock(&mutex);

            printf("[Thread #%u] Processing GET (seq: %u)\n", thread_number, my_sequence);

            // Generate UUID and CRC
            char *uuid = generate_uuid();
            unsigned int crc = calculate_crc32(uuid, strlen(uuid));

            // Send UUID + CRC + sequence number to client
            char response[1024];
            sprintf(response, "%08u-%s %08X", my_sequence, uuid, crc);
            send(client_socket, response, strlen(response), 0);
            printf("[Thread #%u] Sent: %s\n", thread_number, response);
            free(uuid);

            sleep(5);  // Simulate processing time

            // ===================== WAIT FOR PREJETO =====================
            bzero(buffer, sizeof(buffer));
            received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (received <= 0) {
                pthread_mutex_lock(&mutex);
                get_in_progress = 0;
                pthread_cond_signal(&cond);
                pthread_mutex_unlock(&mutex);
                break;
            }

            buffer[received] = '\0';
            printf("[Thread #%u] Received: %s\n", thread_number, buffer);

            if (strncmp(buffer, "PREJETO", 7) == 0) {
                char *client_crc = strchr(buffer, ' ');
                if (client_crc) client_crc++;

                char expected_crc[9];
                sprintf(expected_crc, "%08X", crc);

                pthread_mutex_lock(&mutex);
                if (client_crc && strcmp(client_crc, expected_crc) == 0) {
                    global_counter++;
                    printf("[Thread #%u] CRC OK → global_counter = %u\n", thread_number, global_counter);
                } else {
                    char error_msg[128];
                    sprintf(error_msg, "NAPAKA %s %s",
                            client_crc ? client_crc : "NULL", expected_crc);
                    send(client_socket, error_msg, strlen(error_msg), 0);
                    printf("[Thread #%u] CRC ERROR (got=%s expected=%s)\n",
                           thread_number, client_crc ? client_crc : "NULL", expected_crc);
                }
                get_in_progress = 0;
                pthread_cond_signal(&cond);
                pthread_mutex_unlock(&mutex);

                continue;
            }

            pthread_mutex_lock(&mutex);
            get_in_progress = 0;
            pthread_cond_signal(&cond);
            pthread_mutex_unlock(&mutex);

            continue;
        }

        // ===================== UNKNOWN COMMAND =====================
        send(client_socket, "NEPREPOZNAVEN UKAZ", 18, 0);
    }

    close(client_socket);
    printf("[Thread #%u] Connection closed\n", thread_number);
    return NULL;
}

/* ===================== MAIN ===================== */

int main(void) {
    srand(time(NULL));

    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5095);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        return 1;
    }

    listen(server_socket, 5);
    printf("Server waiting for connections...\n");

    while (1) {
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("accept");
            continue;
        }

        int *socket_ptr = malloc(sizeof(int));
        *socket_ptr = client_socket;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, socket_ptr);
        pthread_detach(thread_id);

        printf("[Main] New client connected: %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }

    close(server_socket);
    return 0;
}
