// SFTP(TCP) server

#include <stdio.h>          // For Standard I/O operations
#include <string.h>         // For string functions like memset(), bzero()
#include <stdlib.h>         // For General purpose functions like atoi()
#include <unistd.h>         // For system level APIs like write(), read(), close()
#include <sys/types.h>      // Has definitions of data types used in system call
#include <sys/socket.h>     // Has definitions of structures for sockets
#include <netinet/in.h>     // Has constants and structures needed for internet domain addresses

#define BUFFER_SIZE 256
#define CHUNK_SIZE 5
#define MAX_NUM_OF_CONNECTIONS 5

int main(int argc, char *argv[])
{
    int welcome_socket, client_socket, port_number, num_of_bytes=0;
    char buffer[BUFFER_SIZE], filename[BUFFER_SIZE];
    struct sockaddr_in serv_addr, cli_addr;
    // Structure contatining internet address
    /* struct sockaddr_in
        {
            short   sin_family;
            u_short sin_port;
            struct  in_addr sin_addr;
            char    sin_zero[8];  //Not used, must be zero 
        };
    */ 

    FILE* file_descriptor;
    socklen_t clilen;

    // Validates input arguments
    if(argc < 2)   {
        fprintf(stderr,"Usage: %s [port_number]\n", argv[0]);
        exit(0);
    }

    // Server creates a welcome socket
    fprintf(stdout, "Creating a welcome socket...\n");
    welcome_socket = socket(AF_INET, SOCK_STREAM, 0);
     if(welcome_socket < 0) { 
        fprintf(stderr, "ERROR!! Could not open a welcome socket\n");
        exit(1);
    }

    // Sets all the values in buffer to zero
    bzero((char *) &serv_addr, sizeof(serv_addr));

    port_number = atoi(argv[1]);

    // Sets the server's address family : IPv4
    serv_addr.sin_family = AF_INET;

    // Sets the 4-byte IP address
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Sets the server's port number
    serv_addr.sin_port = htons(port_number);

    // Binds the socket to the address
    fprintf(stdout, "Binding the socket to the address...\n");
    if(bind(welcome_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)    {
        fprintf(stderr, "ERROR!! Could not bind the socket\n");
        exit(1);
    }

    // Listening for connections on the socket
    fprintf(stdout, "Listening for connections on the welcome socket...\n");
    listen(welcome_socket, MAX_NUM_OF_CONNECTIONS);

    // Accepts the client connection and creates a new client socket 
    clilen = sizeof(cli_addr);
    client_socket = accept(welcome_socket, (struct sockaddr *) &cli_addr, &clilen);
    if(client_socket < 0) {
        fprintf(stderr, "ERROR!! Could not accept the connection\n");
        exit(1);
    }
    fprintf(stdout, "Accepted the client connection and created new client socket...\n");

    // Initialize the buffer
    bzero(buffer, BUFFER_SIZE);

    // Read the output file name from the buffer
    fprintf(stdout, "Reading the output file name...\n");
    num_of_bytes = read(client_socket, buffer, BUFFER_SIZE);
    if(num_of_bytes < 0)    { 
        fprintf(stderr, "ERROR!! Could not read the output file name from the socket\n");
        exit(1);
    }
    strcpy(filename, buffer);
    fprintf(stdout, "Output file name: %s\n",filename);
    
    // Set the buffer values to 0 
    memset(buffer, 0, sizeof(buffer));

    // Open the output file
    file_descriptor = fopen(filename, "wb");
    if(NULL == file_descriptor) {
        fprintf(stderr, "ERROR!! Could not open file %s\n", filename);
        exit(0);
    }
    fprintf(stdout, "File %s opened successfully through fopen()...\n", filename);

    // Reading from buffer in CHUNK_SIZE bytes
    fprintf(stdout, "Reading data from buffer in chunks of %d bytes\n", CHUNK_SIZE);
    while((num_of_bytes = read(client_socket, buffer, CHUNK_SIZE)) > 0) {
        //fprintf(stdout, "Bytes received : %d\n", num_of_bytes);
        //fprintf(stdout, "Data : [%s]\n", buffer);
        if(fwrite(buffer, 1, num_of_bytes, file_descriptor) != num_of_bytes)    {
            fprintf(stderr, "ERROR!! Could not write to %s file\n", filename);
            exit(0);
        }

    }

    if(num_of_bytes < 0)
        fprintf(stderr, "ERROR!! Could not read from the buffer\n");

    // Closing the output file
    fprintf(stdout, "Closing the output file %s...\n", filename);
    fclose(file_descriptor);

    // Closing the sockets and terminating the server
    fprintf(stdout, "Closing the sockets and terminating the server...\n");
    close(client_socket);
    close(welcome_socket);
     
    return 0; 
}
