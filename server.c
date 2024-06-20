#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 5000
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024


int client_sockets[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;


void broadcast_message(char *message, int sender_socket) {
    pthread_mutex_lock(&client_mutex);
    for (int i = 0; i < client_count; i++) {
        if (client_sockets[i] != sender_socket) {
            if (send(client_sockets[i], message, strlen(message), 0) < 0) {
                perror("Failed to send message to client");
            }
        }
    }
    pthread_mutex_unlock(&client_mutex);
}

void *handle_client(void *client_socket) {
    int sock = *(int *)client_socket;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while ((bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        printf("Client[%d]: %s\n", sock, buffer);
        broadcast_message(buffer, sock);
    }

    pthread_mutex_lock(&client_mutex);
    for (int i = 0; i < client_count; i++) {
        if (client_sockets[i] == sock) {
            client_sockets[i] = client_sockets[client_count - 1];
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&client_mutex);

    close(sock);
    printf("Client[%d] disconnected\n", sock);
    return NULL;
}

void *server_shell(void *arg) {
    char command[BUFFER_SIZE];
    while (1) {
        printf("server> ");
        if (fgets(command, sizeof(command), stdin) != NULL) {
            if (strncmp(command, "exit", 4) == 0) {
                printf("Server shutting down...\n");
                pthread_mutex_lock(&client_mutex);
                for (int i = 0; i < client_count; i++) {
                    close(client_sockets[i]);
                }
                pthread_mutex_unlock(&client_mutex);
                exit(0);
            }
            broadcast_message(command, -1);
        }
    }
    return NULL;
}

int main() {
    int server_socket, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Could not bind socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Could not listen on socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

pthread_t shell_thread;
    pthread_create(&shell_thread, NULL, server_shell, NULL);
    pthread_detach(shell_thread);

    while (1) {
        new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (new_socket == -1) {
            perror("Failed to accept client connection");
            continue;
        }

        pthread_mutex_lock(&client_mutex);
        client_sockets[client_count++] = new_socket;
        pthread_mutex_unlock(&client_mutex);

        printf("Client[%d] connected\n", new_socket);

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, handle_client, &new_socket);
        pthread_detach(client_thread);
    }

    close(server_socket);
    return 0;
}
