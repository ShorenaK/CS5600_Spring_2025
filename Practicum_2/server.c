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


// === here we Create any missing folders for incoming files with make_parent_dirs(), 
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

    // ====== Handle WRITE from the clinet but would be recoved from the server end  ======
    // Confirm the command is WRITE using strncmp(), and then Extract file path and size with sscanf(), 
    // Create necessary folders make_parent_dirs(), Open destination file fopen(), Find start of file data in the buffer with strchr(), 
    // Save file content with fwrite(), and lastly we haveve a loop Receive all file chunks from the client, and Finish writing and close file fclose(), 

    
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
// This is  Useful if some file data already came in the first with recv() call

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
     
    // ====== Handle GET cleint get the file from server and copyes it locally command ======
    // == Check for "GET" command	Validate input, Extract file path From buffer using sscanf(), Build full server path Add ROOT_DIR prefix,
    // == Try to open file	Exit if not found, Get file size With fseek() and with ftell() function, Send "SIZE [number]"	Let client know how many bytes to expect
    // == Wait for "READY" Client confirms it's ready to receive, Send file content	Chunk by chunk using fread() and send(), Close file	Finish and clean up

    else if (strncmp(buffer, "GET", 3) == 0) {  // Check if the incoming command starts with "GET", Only the first 3 characters
        char filepath[1024];
        if (sscanf(buffer, "GET %1023s", filepath) != 1) { // Extract the requested file path from the string (e.g., GET folder/file.txt)
            printf("Invalid GET command.\n");
            close(client_sock);
            return;
        }

        // Build full path
        char fullpath[2048]; // Construct the absolute path to the file in the server's file system
        snprintf(fullpath, sizeof(fullpath), "%s/%s", ROOT_DIR, filepath); // Example: server_storage/folder/file.txt

        // Open the file to read, Try to open the file in read-binary mode
        FILE *fp = fopen(fullpath, "rb"); 
        if (!fp) {
            char *msg = "SIZE 0\n";
            send(client_sock, msg, strlen(msg), 0); // Send SIZE 0\n to tell client the file is missing
            close(client_sock); //Then close the connection
            return;
        }

        // Get file size Move to end of file to get its size, then return to beginning so we can start reading
        fseek(fp, 0, SEEK_END);
        long filesize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        // Send file size to client, SIZE 2048\n
        char header[64];
        snprintf(header, sizeof(header), "SIZE %ld\n", filesize);
        send(client_sock, header, strlen(header), 0);

        // Wait for READY from client  respond with "READY\n" before starting the file transfer
        memset(buffer, 0, sizeof(buffer));
        recv(client_sock, buffer, sizeof(buffer), 0);

        // If the client doesn't say it's ready → cancel the transfer
        if (strncmp(buffer, "READY", 5) != 0) {
            printf("Client did not confirm READY.\n");
            fclose(fp);
            close(client_sock);
            return;
        }

// the process of Sending file contents  Read file data in chunks using fread() and send each chunk over the socket, Loop continues until end of file
        while (!feof(fp)) {
            size_t bytes_read = fread(buffer, 1, sizeof(buffer), fp);
            if (bytes_read > 0) {
                send(client_sock, buffer, bytes_read, 0);
            }
        }

        fclose(fp); //Close the file 
        printf("File sent: %s (%ld bytes)\n", fullpath, filesize); //Print a confirmation that the file was successfully sent
    }

    // ====== Handle RM (Delete) command ======
     // Check for "RM" command	Detect delete request, Extract file path Use sscanf() to read filename, Build full path	Prefix with ROOT_DIR, 
    // Try to delete file	Use remove(), Send result back to client Success or failure message, Close socket End client session cleanly

    // Check if the command starts with "RM" — used to signal a request to delete a file 
    else if (strncmp(buffer, "RM", 2) == 0) {
        char filepath[1024];
        if (sscanf(buffer, "RM %1023s", filepath) != 1) { //Extract the file path from the command using sscanf()
            printf("Invalid RM command.\n"); // If format is wrong (e.g. no file path), print an error and exit 
            close(client_sock);
            return;
        }

        // Build the full path to the file within the server’s storage directory
        char fullpath[2048];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", ROOT_DIR, filepath);

        // Attempt to delete the file
        if (remove(fullpath) == 0) {
            send(client_sock, "File deleted successfully.\n", 28, 0);  //If successful: Send a confirmation message back to the client
            printf("Deleted: %s\n", fullpath); // Print a log message on the server
        } else {
            send(client_sock, "Failed to delete file.\n", 24, 0); // if deletaion fails we use perror() to print out the system error message .
            perror("Delete failed"); 
        }
    }

    // ====== Unknown command ====== here we catche anything that isn’t WRITE, GET, or RM, goiod for debugging 
    else {
        printf("Unknown command: %s\n", buffer);
    }

    close(client_sock); //Close the socket connection with the client once you're done processing the command
}

// server entry point Create socket using socket() , Set address IP + port info Bind to port using bind(), Listen for clients using listen(), 
// Accept client connections in an infinite loop  one by one, Call handle_client(client_sock) for each connection
// Close socket	Cleanup (not used here due to infinite loop)
int main() {
    int server_sock, client_sock; // socket used to listen for new connections, socket used to communicate with a specific client
    struct sockaddr_in server_addr, client_addr; //store IP and port info 
    socklen_t client_size = sizeof(client_addr); // this is used used by accept() to know the size of the client address structure

    // Create server socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0); // AF_INET → IPv4
    if (server_sock < 0) { // SOCK_STREAM → TCP protocol ❌ If socket creation fails, print error and exit
        perror("Socket creation failed");
        return 1;
    }

    // Set server address
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_port = htons(PORT); //convert port to network byte order  
    server_addr.sin_addr.s_addr = INADDR_ANY;  // accept connections on any network interface (localhost, LAN, etc.)

    // Bind socket to address
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed"); // Link the socket to the IP + port
        close(server_sock); //If another process is already using the port, bind will fail 
        return 1;
    }

    // Listen for incoming connections Now the socket is in passive mode, waiting for clients
    if (listen(server_sock, 5) < 0) {
        perror("Listen failed");
        close(server_sock);
        return 1;
    }

    printf("Server listening on port %d...\n", PORT); // print a message to show the server is running

// Main server loop to accept and handle clients
// Wait for a client to connect: accept() blocks until a new client arrives
// When a client connects, it returns a new client_sock to talk to that specific client
// If accept() fails, print error and continue waiting


    while (1) {
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_size);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr)); // Print client’s IP
        handle_client(client_sock); // Call your custom function handle_client(client_sock) to process their request (WRITE, GET, RM)
    } // Repeat the whole loop forever — server always waits for the next client

    close(server_sock); // Closes the listening socket if server ever exits (good practice)
    return 0;
} 
