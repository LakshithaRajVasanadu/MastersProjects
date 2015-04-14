// Rdt 3.0 Client

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <string>

#include "packet.h"

using namespace std;

#define CHUNK_SIZE 10
#define SLEEP_TIME 10
#define TRUE 1

//States for client
#define WAIT_FOR_CALL_0_FROM_ABOVE 10
#define WAIT_FOR_ACK_0 20
#define WAIT_FOR_CALL_1_FROM_ABOVE 30
#define WAIT_FOR_ACK_1 40


class rdtClient {
	public:
		struct sockaddr_in server, from;
		unsigned int length;
		int sock, state, lastFinal, lastStart, lastSize, lastSeq;
		char lastSentMsg[BUFFER_SIZE];
		float prob_corruption, prob_loss;

		// Default Constructor
		rdtClient() { }		

		// Overloaded constructor
		rdtClient(char *argv[]) {
			state = WAIT_FOR_CALL_0_FROM_ABOVE;
			prob_corruption = atof(argv[5]);
			prob_loss = atof(argv[6]);
			initialize(argv);
		}

		// Destructor
		~rdtClient() { 
			cout << "Shutting down client" << endl;
			close(sock);
		}

		void initialize(char *argv[]);
		void udtSend(const char* msg, PACKET_TYPE type, int n, int seq, int start, int final); 
		void rdtRcv (PACKET *p);
		void rdtSend(const char* msg, int n, int start, int final);
		void udtRcv();
		void startTransmission(const char* inputFileName, const char* outputFilename);
		void startTimer();

};

// Initializes the client by setting up socket and address
void rdtClient::initialize(char *argv[]) {

	// Get IP address of the host and check if server host is valid
	struct in_addr ip;
	// convert IPv4 address from text to binary form
    inet_pton(AF_INET, argv[1], &ip);                 
    struct hostent *hp = gethostbyaddr(&ip, sizeof(ip), AF_INET);
    if(hp == NULL) {
        cerr << "ERROR!! No such host " << argv[1] << endl;
        exit(0);
    }

    // Creates a client socket
    cout << "Creating client Socket..." << endl;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)  {
        cerr << "ERROR!! Could not create socket" << endl;
        exit(1);
    }

    server.sin_family = AF_INET;
    bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
   	server.sin_port = htons(atoi(argv[2]));
   	length = sizeof(struct sockaddr_in);
}

// Rdt Send - Reliability check for Sending message over UDP
void rdtClient::rdtSend(const char* message, int n, int start, int final=0) {
	switch(state)	{
		case WAIT_FOR_CALL_0_FROM_ABOVE:
			cout << "State: " << "WAIT_FOR_CALL_0_FROM_ABOVE" << endl;
			cout << "Sending message: " << message << endl;
			state = WAIT_FOR_ACK_0;
			udtSend(message, DATA, n, 0, start, final);
			break;
		case WAIT_FOR_CALL_1_FROM_ABOVE:
			cout << "State: " << "WAIT_FOR_CALL_1_FROM_ABOVE" << endl;
			cout << "Sending message: " << message << endl;
			state = WAIT_FOR_ACK_1;
			udtSend(message, DATA, n, 1, start, final);
			break;
		default: ;
	}
}

// Udt Send - Sending actual UDP datagram over socket
void rdtClient::udtSend(const char* message, PACKET_TYPE type, int size, int seq, int start, int final) {
	PACKET *p = new PACKET(type, seq, message, size, start, final);
	p->checksum = computeChecksum(p->msg, p->len);

	//Generating data packet loss
	int lseed = (rand() % 100);
	if(lseed < (prob_loss * 100)) {
		cout << "Generating packet loss!!" << endl;
		cout << "Dropping packet!!" << endl;
		//p->print();
		delete p;
	}
	else 
	{
		//Generating corrupted checksum for data packet
		int seed = (rand() % 100);
		if(seed < (prob_corruption * 100)) {
		cout << "Generating corrupted checksum!!" << endl;
		p->checksum = 0;
		}

		cout << "Sending packet!!..." << endl;
		//p->print();
		int n = sendto(sock, p, sizeof(PACKET), 0, (const struct sockaddr *)&server, length);
   		if (n < 0) {
   			cerr << "Error in sending the packet!!" << endl;
   			exit(1);
   		}
   }
   	memset(lastSentMsg, 0, BUFFER_SIZE);
   	memcpy(lastSentMsg, message, size);
   	lastFinal = final;
   	lastStart = start;
   	lastSize = size;
   	lastSeq = seq;
   	
   	//Set timer
   	startTimer();
}

// Udt Receive - Receiving actual UDP datagram from socket
void rdtClient::udtRcv()	{
	char buffer[BUFFER_SIZE];
	PACKET *ackPacket = new PACKET();
	// Using MSG_DONTWAIT flag to make recv non-blocking
	int n = recvfrom(sock, (PACKET*)ackPacket, BUFFER_SIZE, MSG_DONTWAIT, (struct sockaddr *)&from, &length);
	if(n < 0)	{
		char prevMsg[lastSize];
		memset(prevMsg, 0, lastSize);
		memcpy(prevMsg, lastSentMsg, lastSize);
		cout << "Timeout!! Retransmission!! " << lastSentMsg << endl;
		udtSend(prevMsg, DATA, lastSize, lastSeq, lastStart, lastFinal);
   	}
   	else {
   		cout << "Received Packet:" << endl;
   		//ackPacket->print();
   		rdtRcv(ackPacket);
   	}
   	
}

// Rdt Receive - Reliability check while receiving a packet
void rdtClient::rdtRcv(PACKET *p) {
	char prevMsg[lastSize];
	memset(prevMsg, 0, lastSize);
	memcpy(prevMsg, lastSentMsg, lastSize);

	switch(state)	{
		case WAIT_FOR_ACK_0:
			cout << "State: " << "WAIT_FOR_ACK_0" << endl;

			//ACK with seq no = 1 received
			if(p->type == ACK && p->seq == 1)	{
				cout << "ACK 1 received!! Retransmission!!" << endl;
				cout << "Sending message: " << lastSentMsg << endl;
				udtSend(prevMsg, DATA, lastSize, 0, lastStart, lastFinal);
			}

			//Received packet corrupted
			else if(p->checksum != computeChecksum(p->msg, p->len))	{
				cout << "Received Packet is Corrupted!! Retransmission!!" << endl;
				cout << "Sending message: " << lastSentMsg << endl;
				udtSend(prevMsg, DATA, lastSize, 0, lastStart, lastFinal);
			}

			//Received packet is not corrupted and its ACK has seq no = 0
			else if(p->checksum == computeChecksum(p->msg, p->len))	{
				if(p->type == ACK && p->seq == 0) {
					state = WAIT_FOR_CALL_1_FROM_ABOVE;
					cout << "ACK 0 received and Packet not corrupted!!" << endl;
				}
			}
			break;
		case WAIT_FOR_ACK_1:
		 	cout << "State: " << "WAIT_FOR_ACK_1" << endl;

		 	//ACK with seq no = 0 received
		 	if(p->type == ACK && p->seq == 0)	{
				cout << "ACK 0 received!! Retransmission!!" << endl;
				cout << "Sending message: " << lastSentMsg << endl;
				udtSend(prevMsg, DATA, lastSize, 1, lastStart, lastFinal);
			}
			//Received packet corrupted
			else if(p->checksum != computeChecksum(p->msg, p->len))	{
				cout << "Received Packet is Corrupted!! Retransmission!!" << endl;
				cout << "Sending message: " << lastSentMsg << endl;
				udtSend(prevMsg, DATA, lastSize, 1, lastStart, lastFinal);
			}
			//Received packet is not corrupted and its ACK has seq no = 1
			else if(p->checksum == computeChecksum(p->msg, p->len))	{
				if(p->type == ACK && p->seq == 1) {
					state = WAIT_FOR_CALL_0_FROM_ABOVE;
					cout << "ACK 1 received and Packet not corrupted!!" << endl;
				}
			}
			break;
		default: ;
	}
}

void rdtClient::startTimer() {
	cout << "Starting timer for " << SLEEP_TIME << " milli seconds..." << endl; 
	usleep(SLEEP_TIME * 1000);
	cout << "End of timer!!" << endl;
}

// Start data transmission from client
void rdtClient::startTransmission(const char* inputFileName, const char* outputFilename) {
	// Open the input file 
    FILE* file_descriptor = fopen(inputFileName, "rb");
    if(NULL == file_descriptor) {
        cerr << "ERROR!! Could not open file" << inputFileName << endl;
        exit(0);
    }
    cout << "File " << inputFileName << " opened successfully through fopen()...\n";

    // Get the size of input File
    struct stat st;
    stat(inputFileName, &st);

    //Send the output file name to the server
    cout << "Sending output file name " << outputFilename << " to the server...\n";
    rdtSend(outputFilename, strlen(outputFilename), 1, st.st_size==0?1:0);

    int final;

    while(TRUE) {
    	if(state == WAIT_FOR_CALL_0_FROM_ABOVE || state == WAIT_FOR_CALL_1_FROM_ABOVE) {
    		
    		if(final == 1)
    			break;

    		char message[BUFFER_SIZE];
    		memset(message, 0, sizeof(message));
    		// Read data from file in bytes of CHUNK_SIZE
    		int num_of_bytes = fread(message, 1, CHUNK_SIZE, file_descriptor);
    		if(num_of_bytes > 0) {
    			final = ((feof(file_descriptor) == true)?1:0);
    			rdtSend(message, num_of_bytes, 0, final);
    		}

        	if(num_of_bytes < CHUNK_SIZE)   {
        		if(num_of_bytes == 0)	{
        		 	cout << "No data to read!!" << endl;
        		  	break;
        		}
            	if(feof(file_descriptor))
                	cout << "End of file..." << endl;
            	if(ferror(file_descriptor))	{
            		cerr << "ERROR!! Could not read from the file" << endl;
            		break;
            	}
        	}
    	}
    	else if(state == WAIT_FOR_ACK_0 || state == WAIT_FOR_ACK_1) {
    		udtRcv();
    	}
    }
}

int main(int argc, char *argv[])
{	
	// Validates input arguments
	if(argc < 7) {
        cerr << "Usage: " << argv[0] << " [host_ip] [port_number] [input_file_name] [output_file_name] [probability_of_corruption] [probability_of_loss]" << endl;
        exit(0);
    }

    rdtClient *client = new rdtClient(argv);
    client->startTransmission(argv[3], argv[4]);
    delete client;

	return 0;
}