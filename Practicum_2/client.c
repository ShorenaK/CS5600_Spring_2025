/*
 * client.c -- TCP Socket Client
 * 
 * adapted from: 
 *   https://www.educative.io/answers/how-to-implement-tcp-sockets-in-c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 4 || strcmp(argv[1], "WRITE") != 0) {
        printf("Usage: %s WRITE local_file_path remote_file_path\n", argv[0]);
        return 1;
    }

    char *local_path = argv[2];
    char *remote_path = argv[3];

    // Open local file
    FILE *fp = fopen(local_path, "rb");
    if (fp == NULL) {
        perror("Failed to open local file");
        return 1;
    }

    // Read file size
    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (filesize <= 0) {
        printf("Empty or invalid file.\n");
        fclose(fp);
        return 1;
    }

    // Read file content
    char *filedata = malloc(filesize);
    if (!filedata) {
        perror("Memory allocation failed");
        fclose(fp);
        return 1;
    }

    fread(filedata, 1, filesize, fp);
    fclose(fp);

    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        free(filedata);
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2024);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        free(filedata);
        return 1;
    }

    // Send command header
    char header[1024];
    snprintf(header, sizeof(header), "WRITE %s %ld\n", remote_path, filesize);
    send(sock, header, strlen(header), 0);

    // Send file content
    send(sock, filedata, filesize, 0);
    printf("File '%s' sent to server as '%s'\n", local_path, remote_path);

    free(filedata);
    close(sock);
    return 0;
}
