/*
 * server.c -- TCP Socket Server
 * 
 * adapted from: 
 *   https://www.educative.io/answers/how-to-implement-tcp-sockets-in-c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 2024
#define BUFFER_SIZE 4096
#define ROOT_DIR "server_storage"

void make_parent_dirs(const char *path) {
    char temp[1024];
    strcpy(temp, path);

    for (char *p = temp + strlen(ROOT_DIR) + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            mkdir(temp, 0755);
            *p = '/';
        }
    }
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_size = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_sock);
        return 1;
    }

    // Listen for connections
    if (listen(server_sock, 5) < 0) {
        perror("Listen failed");
        close(server_sock);
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_size);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

        // Receive command header
        memset(buffer, 0, sizeof(buffer));
        int header_len = recv(client_sock, buffer, sizeof(buffer), 0);
        if (header_len <= 0) {
            printf("Error reading command\n");
            close(client_sock);
            continue;
        }

        // Parse command: "WRITE path size"
        char command[16], filepath[1024];
        long filesize;
        if (sscanf(buffer, "WRITE %1023s %ld", filepath, &filesize) != 2) {
            printf("Invalid WRITE command\n");
            close(client_sock);
            continue;
        }

        // Clean leftover newline
        char *newline = strchr(filepath, '\n');
        if (newline) *newline = '\0';

        // Prepend ROOT_DIR to path
        char fullpath[2048];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", ROOT_DIR, filepath);

        // Make parent directories
        make_parent_dirs(fullpath);

        // Open file for writing
        FILE *fp = fopen(fullpath, "wb");
        if (!fp) {
            perror("Failed to open file for writing");
            close(client_sock);
            continue;
        }

        // Receive and write file content
        long bytes_received = 0;
        while (bytes_received < filesize) {
            ssize_t chunk = recv(client_sock, buffer, sizeof(buffer), 0);
            if (chunk <= 0) break;
            fwrite(buffer, 1, chunk, fp);
            bytes_received += chunk;
        }

        fclose(fp);
        printf("File saved: %s (%ld bytes)\n", fullpath, bytes_received);

        close(client_sock);
    }

    close(server_sock);
    return 0;
}
