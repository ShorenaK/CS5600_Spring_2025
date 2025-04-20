er#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

// To limit how much data we send or receive at once.
#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
// It checks if the user gave enough command-line arguments. If not, it shows the correct way to run the program, so users aren’t confused.
// It prevents the program from continuing with missing inputs, which could cause errors.

    if (argc < 2) {
        printf("Usage:\n");
        printf("  %s WRITE local_file_path remote_file_path\n", argv[0]);
        printf("  %s GET remote_file_path local_file_path\n", argv[0]);
        printf("  %s RM remote_file_path\n", argv[0]);
        return 1;
    }

    // connection handle    
    int sock;
    // structure that will store the server’s address info like IP address and port.
    struct sockaddr_in server_addr;
    // temporary storage for messages — a string or binary data holder.
    char buffer[BUFFER_SIZE];

    // Create socket, server address setup AF_INET -- IPv4 address family , SOCK_STREAM -- telling the system to use TCP
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

     // Set server address
    server_addr.sin_family = AF_INET;
    // sets the port number (2024) but in network byte order 
    server_addr.sin_port = htons(2024);
    // IP address, converting "127.0.0.1" string to the binary form.
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Try to Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return 1;
    }
// === In PART 1: we made Socket connection to server connect(), File opened (client-side) fopen(), we calculate File size fseek() + ftell(), 
//=== we allocated memory wiht malloc(filesize), File read into memory with fread(), 
// ==  File closed	fclose(fp) after reading, Header prepared snprintf() → command line: "WRITE path size"
    
// Check if the first argument is "WRITE" and that exactly 4 arguments were passed
if (strcmp(argv[1], "WRITE") == 0 && argc == 4) {
    // argv[2] - path to the local file on the client's machine
    char *local_path = argv[2];
    // argv[3] -  path to where the file should be saved on the server
    char *remote_path = argv[3];

    // Try to open the local file in binary read mode ("rb")
    FILE *fp = fopen(local_path, "rb");
    // If file cannot be opened, print error, close socket, and exit
    if (!fp) {
        perror("Failed to open local file");
        close(sock);
        return 1;
    }
    // We need to find out how big the file is (in bytes) so that:
    // We know how much memory to allocate for it
    // We can tell the server how many bytes we’re sending
    // Move file pointer to the end to measure file size
    fseek(fp, 0, SEEK_END);
    // Get the current position of the file pointer = total file size in bytes
    long filesize = ftell(fp);
    // Move file pointer back to the beginning so we can read the file later
    fseek(fp, 0, SEEK_SET);

    // If the file is empty or has an invalid size, exit with a message
    if (filesize <= 0) {
        printf("Empty or invalid file.\n");
        fclose(fp);
        close(sock);
        return 1;
    }

    // Allocate memory for file content based on file size we measured earlier
    char *filedata = malloc(filesize); // requests that many bytes from the heap.
    if (!filedata) { //filedata is a pointer to the start of the allocated memory.
        perror("Memory allocation failed");
        fclose(fp); // Close the file before exiting
        close(sock); // Close the network connection
        return 1; // Exit with error
    }

    // Read the file content into the allocated memory buffer
    size_t bytes_read = fread(filedata, 1, filesize, fp);
    fclose(fp); // We're done reading from the file, so close it

    // Confirm that the number of bytes read matches the expected file size
    if (bytes_read != filesize) {
        printf("Client: fread only read %zu of %ld bytes\n", bytes_read, filesize);
        free(filedata); // Free the allocated memory since we're not using it
        close(sock); // Also close the socket
        return 1;
    }

    // A confirmation message showing how many bytes were successfully read into memory
    printf("Client: read %zu bytes from file\n", bytes_read);

//  Prepare the command header  and  Send command header
// This header tells the server what to do, where to save the file, and how big it is.
// Format: "WRITE remote_file_path filesize\n"
// Example: "WRITE folder/file.txt 128\n"
  
    char header[1024];
    snprintf(header, sizeof(header), "WRITE %s %ld\n", remote_path, filesize);
    // Sends the header string through the open socket.
    // The server will parse this header to prepare for the incoming file content.
    send(sock, header, strlen(header), 0);

    // Send actual file content
    ssize_t sent = send(sock, filedata, filesize, 0);
    // Print confirmation of how much was sent
    printf("Client: sent %zd bytes to server\n", sent);

    // Confirm what file was sent and where
    printf("File '%s' sent to server as '%s'\n", local_path, remote_path);
    free(filedata); // Free the allocated memory now that the file has been sent
  }

    
// ==== PART 2: GET elsse if check command 	Make sure it's "GET" with 2 file paths,then Build and send GET request	"GET remote_path\n"
//=== Receive file size from server	e.g., "SIZE 2048\n", Confirm READY to receive	Send "READY\n", Create local file	Open for writing
// == Receive file chunks	Using recv() and fwrite(), Close and confirm	Close file, print success message

    //Check if the user typed "GET" and passed the correct number of arguments
    // (./client GET remote_path local_path) strcmp() --> string compare

    else if (strcmp(argv[1], "GET") == 0 && argc == 4) {
        // Store the remote file path (on the server) and the local file path (where to save the file)
        char *remote_path = argv[2];
        char *local_path = argv[3];

        char header[1024];
        // Format a command message like "GET folder/file.txt\n"
        snprintf(header, sizeof(header), "GET %s\n", remote_path);
        send(sock, header, strlen(header), 0); // Send the GET command to the server to request the file

        long filesize; //Declare a variable to hold the incoming file size
        memset(buffer, 0, sizeof(buffer)); // Clear the buffer before receiving data
        recv(sock, buffer, sizeof(buffer), 0); // Receive the server’s response, which should be something like: "SIZE 1024\n"

         //Parse the filesize from the message
        if (sscanf(buffer, "SIZE %ld", &filesize) != 1 || filesize <= 0) {
            printf("Invalid file or file not found on server.\n");
            close(sock);
            return 1;
        }

        // Tell the server you're ready to receive the file
        send(sock, "READY\n", 6, 0);

         // Open the local file for writing in binary mode
        FILE *fp = fopen(local_path, "wb");
        if (!fp) {
           //  If local file can’t be created → print error, clean up, and exit
            perror("Failed to create local file");
            close(sock);
            return 1;
        }

        long bytes_received = 0; // Track how many bytes we’ve received so far
        // Receive the file in chunks . Write each chunk into the local file . Stop when we’ve received the full file
        while (bytes_received < filesize) {
            ssize_t chunk = recv(sock, buffer, sizeof(buffer), 0);
            if (chunk <= 0) break;
            fwrite(buffer, 1, chunk, fp);
            bytes_received += chunk;
        }
       // Done writing → close the local file
        fclose(fp);
        // Print a success message showing where the file came from and where it was saved
        printf("File '%s' received from server as '%s' (%ld bytes)\n", remote_path, local_path, bytes_received);
    }

// ==== PART 3: RM Delete Delete commend , Check for "RM" commandto Validates, Format "RM path\n" Build command string
// Send to server to Ask server to delete the file, Wait for reply from Server responds with success/failure
// === Print result Show the server's message adn finally Close socket	Clean shutdown


    // Check if the user typed "RM" with one path: ./client RM remote_path
    else if (strcmp(argv[1], "RM") == 0 && argc == 3) {
        char *remote_path = argv[2];  // Store the remote file path to delete (on the server)

        // Create the command string, like: "RM folder/myfile.txt\n" This tells the server to delete that file
        char header[1024];
        snprintf(header, sizeof(header), "RM %s\n", remote_path);
        send(sock, header, strlen(header), 0); // Send the command to the server over the socket

        memset(buffer, 0, sizeof(buffer)); // Clear the buffer before receiving any server response
        recv(sock, buffer, sizeof(buffer), 0); // Receive the server's response — success or failure message
        printf("Server response: %s\n", buffer); // Print whatever the server replied (e.g. "File deleted successfully.")
    }

<<<<<<< HEAD
    // ====== Multi-client ======
=======
    // ====== PART 4A: Multi-client working on it  ======
>>>>>>> 615308030dbf90e6c516a4b3e034eff373db5b02
    /*
   
    */

    // Catch-All for Invalid Commands, If user typed an unknown command or gave the wrong number of arguments, show usage error
    else {
        printf("Invalid command or argument count.\n");
    }

    close(sock); // Close the connection to the server
    return 0; // End the program successfully
}

