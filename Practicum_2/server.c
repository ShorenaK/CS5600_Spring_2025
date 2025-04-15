#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

//the TCP port number the server will listen on
#define PORT 2024
//: size of the buffer used for receiving data
#define BUFFER_SIZE 4096
// the base directory on the server where files are stored
#define ROOT_DIR "server_storage"


// === int this section Creates any missing folders for incoming files with make_parent_dirs(), 
// ==== Reads the initial command sent by the client handle_client() , Receives the first message from the client recv(), 
// === Handles bad or dropped connections 

// Helper function to create parent directories for a given path
//Copies the full file path into a temporary buffer so we can modify it safely. 
void make_parent_dirs(const char *path) {
    char temp[1024];
    strcpy(temp, path);

    for (char *p = temp + strlen(ROOT_DIR) + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            mkdir(temp, 0755);  // Create the directory if it doesn't exist
            *p = '/';
        }
    }
}

// Function to handle a single client request
// Sets up a buffer to receive data from the client
void handle_client(int client_sock) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer)); // Clears it using memset to avoid leftover garbage data

    // Receive the initial command (e.g., WRITE, GET, RM)
    // Reads data from the client socket into the buffer
    ssize_t received = recv(client_sock, buffer, sizeof(buffer), 0);
    if (received <= 0) {
        // The client closed the connection or an error happened
        printf("Failed to receive command.\n");
        //So the server closes the socket and stops handling this client
        close(client_sock);
        return;
    }

    // ====== Handle WRITE command ======
    // Confirm the command is WRITE strncmp(), Extract file path and size sscanf(), 
    // Create necessary folders make_parent_dirs(), Open destination file fopen(), Find start of file data in the buffer strchr(), 
    // Save file content fwrite(), and lastly we ahve a loop Receive all file chunks from the client, and Finish writing and close file fclose(), 

    
    // Check if the message starts with "WRITE" (first 5 characters)
    if (strncmp(buffer, "WRITE", 5) == 0) {
        char filepath[1024];  // The file path the client wants to write to
        long filesize; // The size of the file (in bytes)

        // Parse command: "WRITE path size"
       // we are using sscanf() to Extract file path and size using 
        if (sscanf(buffer, "WRITE %1023s %ld", filepath, &filesize) != 2 || filesize <= 0) {
            printf("Invalid WRITE command.\n"); //If the format is wrong or size is invalid → show error and stop
            close(client_sock);
            return;
        }

        // Build the full file path
        char fullpath[2048];
        // Combine root directory + user’s requested path (e.g., "server_storage/folder/file.txt")
        snprintf(fullpath, sizeof(fullpath), "%s/%s", ROOT_DIR, filepath);
       // Create any needed directories using make_parent_dirs()
        make_parent_dirs(fullpath);

        // Open file for writing
        FILE *fp = fopen(fullpath, "wb"); // Try to create or open the destination file in binary write mode
        if (!fp) {
            perror("Failed to open file for writing"); // If it fails, print error and stop
            close(client_sock);
            return;
        }

        // Find where file data starts in the buffer
//Find the newline character that separates the command from the actual file content
        char *file_start = strchr(buffer, '\n');
        if (!file_start) {
            printf("Malformed WRITE command, no newline.\n");
            fclose(fp);
            close(client_sock);
            return;
        }

// Calculate how much of the received data is actual file content (after the header)
// This is  Useful if some file data already came in the first recv() call

        file_start++; // Skip the newline. Move past the newline
        ssize_t header_len = file_start - buffer;
        ssize_t initial_data_len = received - header_len;

// Tracking  If the first recv() already includes some file content, write it to the file and track how many bytes were written
        long bytes_received = 0;
        if (initial_data_len > 0) {
            fwrite(file_start, 1, initial_data_len, fp);
            bytes_received += initial_data_len;
        }

//Keep reading from the socket until the full file is received
        // Read the remaining bytes of the file
        while (bytes_received < filesize) {
            ssize_t chunk = recv(client_sock, buffer, sizeof(buffer), 0);
            if (chunk <= 0) break;
            fwrite(buffer, 1, chunk, fp);
            bytes_received += chunk; // Write each chunk to the file using fwrite()
        }
//Close the file after writing and print confirmtaion 
        fclose(fp);
        printf("File saved: %s (%ld bytes)\n", fullpath, bytes_received);
    }


        
        
    // ====== Handle GET command ======
    else if (strncmp(buffer, "GET", 3) == 0) {
        char filepath[1024];
        if (sscanf(buffer, "GET %1023s", filepath) != 1) {
            printf("Invalid GET command.\n");
            close(client_sock);
            return;
        }

        // Build full path
        char fullpath[2048];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", ROOT_DIR, filepath);

        // Open the file to read
        FILE *fp = fopen(fullpath, "rb");
        if (!fp) {
            char *msg = "SIZE 0\n";
            send(client_sock, msg, strlen(msg), 0);
            close(client_sock);
            return;
        }

        // Get file size
        fseek(fp, 0, SEEK_END);
        long filesize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        // Send file size to client
        char header[64];
        snprintf(header, sizeof(header), "SIZE %ld\n", filesize);
        send(client_sock, header, strlen(header), 0);

        // Wait for READY from client
        memset(buffer, 0, sizeof(buffer));
        recv(client_sock, buffer, sizeof(buffer), 0);

        if (strncmp(buffer, "READY", 5) != 0) {
            printf("Client did not confirm READY.\n");
            fclose(fp);
            close(client_sock);
            return;
        }

        // Send file contents
        while (!feof(fp)) {
            size_t bytes_read = fread(buffer, 1, sizeof(buffer), fp);
            if (bytes_read > 0) {
                send(client_sock, buffer, bytes_read, 0);
            }
        }

        fclose(fp);
        printf("File sent: %s (%ld bytes)\n", fullpath, filesize);
    }


        
    // ====== Handle RM (Delete) command ======
    else if (strncmp(buffer, "RM", 2) == 0) {
        char filepath[1024];
        if (sscanf(buffer, "RM %1023s", filepath) != 1) {
            printf("Invalid RM command.\n");
            close(client_sock);
            return;
        }

        char fullpath[2048];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", ROOT_DIR, filepath);

        // Attempt to delete the file
        if (remove(fullpath) == 0) {
            send(client_sock, "File deleted successfully.\n", 28, 0);
            printf("Deleted: %s\n", fullpath);
        } else {
            send(client_sock, "Failed to delete file.\n", 24, 0);
            perror("Delete failed");
        }
    }

    // ====== Unknown command ======
    else {
        printf("Unknown command: %s\n", buffer);
    }

    close(client_sock);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_size = sizeof(client_addr);

    // Create server socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to address
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_sock);
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_sock, 5) < 0) {
        perror("Listen failed");
        close(server_sock);
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    // Main server loop to accept and handle clients
    while (1) {
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_size);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));
        handle_client(client_sock);
    }

    close(server_sock);
    return 0;
}
