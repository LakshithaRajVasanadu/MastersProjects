// SFTP(TCP) Client

#include <stdio.h>          // For Standard I/O operations      
#include <stdlib.h>         // For General purpose functions like atoi()
#include <unistd.h>         // For system level APIs like write(), read(), close() 
#include <string.h>         // For string functions like memset(), bzero()
#include <sys/types.h>      // Has definitions of data types used in system call
#include <sys/socket.h>     // Has definitions of structures for sockets
#include <netinet/in.h>     // Has constants and structures needed for internet domain addresses
#include <netdb.h>          // Has definitions for network database operations like gethostbyname()
#include <arpa/inet.h>      // Has definitions for internet operations like in_addr

#define BUFFER_SIZE 256
#define CHUNK_SIZE 10
#define TRUE 1

int main(int argc, char *argv[])
{
    int client_socket, port_number, num_of_bytes;
    struct sockaddr_in server_address;
    // Structure contatining internet address
    /* struct sockaddr_in
        {
            short   sin_family;
            u_short sin_port;
            struct  in_addr sin_addr;
            char    sin_zero[8];  //Not used, must be zero 
        };
    */ 
    
    struct hostent *server;
    // Defines host computer on the internet
    /* struct  hostent
        {
            char    *h_name;        // official name of host 
            char    **h_aliases;    // alias list 
            int     h_addrtype;     // host address type 
            int     h_length;       // length of address 
            char    **h_addr_list;  // list of addresses from name server 
            #define h_addr  h_addr_list[0]  // address, for backward compatiblity 
        };
    */
    FILE* file_descriptor;
    char buffer[BUFFER_SIZE];
    struct in_addr ip;
    // Internet Address
    /* struct in_addr 
       {
            uint32_t    s_addr;     // address in network byte order 
       };
    */

    // Validation of input arguments
    if(argc < 5) {
        fprintf(stderr,"Usage: %s [host_ip] [port_number] [input_file_name] [output_file_name]\n", argv[0]);
        exit(0);
    }

    // Get IP address of the host and check if server host is valid
    inet_pton(AF_INET, argv[1], &ip);                 // convert IPv4 address from text to binary form
    server = gethostbyaddr(&ip, sizeof(ip), AF_INET);
    if(server == NULL) {
        fprintf(stderr, "ERROR!! No such host %s\n", argv[1]);
        exit(0);
    }

    port_number = atoi(argv[2]);

    // Creates a client socket
    fprintf(stdout, "Creating client Socket...\n");
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket < 0)  {
        fprintf(stderr, "ERROR!! Could not create socket\n");
        exit(1);
    }
    
    // Sets all the values in buffer to zero
    bzero((char *) &server_address, sizeof(server_address));

    // Sets the server's address family : IPv4
    server_address.sin_family = AF_INET;

    // Sets the server's address
    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);

    // Sets the server's port number, converts a port number in host byte order to a port number in network byte order
    server_address.sin_port = htons(port_number);

    // Establish connection to the server from client
    fprintf(stdout, "Establishing connection to the server...\n");
    if(connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)    { 
        fprintf(stderr, "ERROR!! Could not connect to the server\n");
        exit(0);
    }

    // Open the input file 
    file_descriptor = fopen(argv[3],"rb");
    if(NULL == file_descriptor) {
        fprintf(stderr, "ERROR!! Could not open file %s\n", argv[3]);
        exit(0);
    }
    fprintf(stdout, "File %s opened successfully through fopen()...\n", argv[3]);

   
    // Send output file name to server
    fprintf(stdout, "Sending output file name %s to the server...\n", argv[4]);
    num_of_bytes = write(client_socket, argv[4], strlen(argv[4]));
        if(num_of_bytes < 0)   { 
            fprintf(stderr, "ERROR!! Could not write output file name to socket\n");
            exit(1);
        }

    // Read and send input file data to the server
    fprintf(stdout, "Reading and sending Input file data in chunks of %d bytes...\n", CHUNK_SIZE);
    while(TRUE) {

        // Sets all the values in buffer to zero
        memset(buffer, 0, sizeof(buffer));

        // Read data from file in chunks
        num_of_bytes = fread(buffer, 1, CHUNK_SIZE, file_descriptor);
        //fprintf(stdout, "Bytes read : %d \n", num_of_bytes);
                
        if(num_of_bytes > 0) {
            //fprintf(stdout, "Sending Bytes: %s \n", buffer);
            if (write(client_socket, buffer, num_of_bytes) < 0) {
                fprintf(stderr, "ERROR!! Could not write output file name to socket\n");
                exit(1);
            }
        }

        if(num_of_bytes < CHUNK_SIZE)   {
            if(feof(file_descriptor))
                fprintf(stdout, "End of file...\n");
            if(ferror(file_descriptor))
                fprintf(stderr, "ERROR!! Could not read from the file\n");
            break;
        }
    }

    // Close the client connection
    fprintf(stdout, "Closing Client Connection...\n");
    close(client_socket);

    return 0;
}