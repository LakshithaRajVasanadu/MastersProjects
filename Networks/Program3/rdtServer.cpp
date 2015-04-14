// Rdt 3.0 Server

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <csignal>

#include "packet.h"

using namespace std;

#define TRUE 1

//States for server
#define WAIT_FOR_0_FROM_BELOW 11
#define WAIT_FOR_1_FROM_BELOW 21

class rdtServer{
	public:
		struct sockaddr_in server, from;
		socklen_t fromlen;
		int sock, state, final;
		unsigned int length;
		char lastCorrectMsg[BUFFER_SIZE], filename[BUFFER_SIZE];
		FILE* file_descriptor;
		float prob_corruption, prob_loss;

		// Default constructor
		rdtServer() { }

		// Overloaded constructor
		rdtServer(char *argv[]) {
			state = WAIT_FOR_0_FROM_BELOW;
			prob_corruption = atof(argv[2]);
			prob_loss = atof(argv[3]);
			initialize(argv);
		}

		// Destructor
		~rdtServer() { }
		void initialize(char *argv[]);
		void start();
		void udtRcv();
		void rdtRcv(PACKET *p);
		void udtSend(const char* msg, PACKET_TYPE type, int seq, int final);
		void deliverData(PACKET *p);
};

// Initializes the server by setting up socket and address
void rdtServer::initialize(char *argv[]) {

	// Creates a server socket
    cout << "Creating Server Socket..." << endl;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)  {
        cerr << "ERROR!! Could not create socket" << endl;
        exit(1);
    }

    length = sizeof(server);
   	bzero(&server,length);
   	server.sin_family = AF_INET;
   	server.sin_addr.s_addr = INADDR_ANY;
   	server.sin_port = htons(atoi(argv[1]));

   	// Binds the socket to the address
    cout << "Binding the socket to the address..." << endl; 
    if(bind(sock, (struct sockaddr *) &server, length) < 0)    {
        cerr << "ERROR!! Could not bind the socket" << endl;
        exit(1);
    }
    fromlen = sizeof(struct sockaddr_in);
}

// Udt Receive - Receiving actual UDP datagram from socket
void rdtServer::udtRcv()	{
	char buffer[BUFFER_SIZE];
	PACKET *dataPacket = new PACKET();
	int n = recvfrom(sock, dataPacket, sizeof(PACKET), 0, (struct sockaddr *)&from, &length);
	if (n < 0)	{
		cerr << "Error in receiving the packet!!" << endl;
   		exit(1);
   	}
   	final = dataPacket->final;
   	//dataPacket->print();
   	rdtRcv(dataPacket);
}

// Rdt Receive - Reliability check while receiving a packet
void rdtServer::rdtRcv(PACKET *p) {
	switch(state)	{
		case WAIT_FOR_0_FROM_BELOW:
			cout << "State: " << "WAIT_FOR_0_FROM_BELOW" << endl;

			//no corruption
			if(p->checksum == computeChecksum(p->msg, p->len)) {
				cout << "Received Packet is Not Corrupted :" << p->msg << endl;
				//seq no = 0
				if(p->seq == 0) {
					cout << "Has seq 0" << endl;
					state = WAIT_FOR_1_FROM_BELOW;
					cout << "Sending ACK O" << endl;
					udtSend("ACK0", ACK, 0, p->final);
					deliverData(p);
				}
				//seq no = 1
				else if(p->seq == 1) {
					cout << "Seq Number!= 0. Not ok" << endl;
					cout << "Sending ACK 1" << endl;
					udtSend("ACK1", ACK, 1, p->final);
				}
			}
			//corrupted
			else {
				cout << "Received Packet is Corrupted :" << p->msg << endl;
				cout << "Sending ACK 1" << endl;
				udtSend("ACK1", ACK, 1, p->final);
			}
			break;
		case WAIT_FOR_1_FROM_BELOW:
			cout << "State: " << "WAIT_FOR_1_FROM_BELOW" << endl;

			//no corruption
			if(p->checksum == computeChecksum(p->msg, p->len)) {
				cout << "Received Packet is Not Corrupted :" << p->msg << endl;
				//seq no = 1
				if(p->seq == 1) {
					cout << "Has seq 1" << endl;
					state = WAIT_FOR_0_FROM_BELOW;
					cout << "Sending ACK 1" << endl;
					udtSend("ACK1", ACK, 1, p->final);
					deliverData(p);
				}
				//seq no = 0
				else if(p->seq == 0) {
					cout << "Seq Number!= 1. Not ok" << endl;
					cout << "Sending ACK 0" << endl;
					udtSend("ACK0", ACK, 0, p->final);
				}
			}
			//corrupted
			else {
				cout << "Received Packet is Corrupted :" << p->msg << endl;
				cout << "Sending ACK 0" << endl;
				udtSend("ACK0", ACK, 0, p->final);
			}
			break;
		default: ;
		}
}

// Udt Send - Sending actual UDP datagram over socket
void rdtServer::udtSend(const char* message, PACKET_TYPE type, int seq, int final) {
	PACKET *p = new PACKET(type, seq, message, strlen(message), 0, final);
	p->checksum = computeChecksum(p->msg, p->len);
	
	//Generating corrupted checksum for ACK
	int seed = (rand() % 100);
	if(seed < (prob_corruption * 100)) {
		cout << "Generating corrupted checksum" << endl;
		p->checksum = 0; 
	}

	//Generating ACK packet loss
	int lseed = (rand() % 100);
	if(lseed < (prob_loss * 100)) {
		cout << "Generating packet loss!!" << endl;
		delete p;
		return;
	}

	cout << "Sending packet!!..." << endl;
	//p->print();
	int n = sendto(sock, p, sizeof(PACKET), 0, (const struct sockaddr *)&from, fromlen);
   	if (n < 0) {
   		cerr << "Error in sending the packet!!" << endl;
   		exit(1);
   	}
}

// Deliver data to the file
void rdtServer::deliverData(PACKET *p)	{
	// 1st message is output file name
	if(p->start == 1)	{
		// Open the output file
    	file_descriptor = fopen(p->msg, "wb");
    	if(NULL == file_descriptor) {
        	cerr << "ERROR!! Could not open file: " << p->msg << endl;
       		exit(0);
    	}
    	strcpy(filename, p->msg);
    	cout << "File " << p->msg << " opened successfully through fopen()" << endl;
		
		// Close file if its last message as well
    	if(p->final == 1)	{
			// Closing the output file
    		cout << "Closing the output file: " << filename << endl;
    		fclose(file_descriptor);
		}
	}

	//Write to output file if file descriptor is not null
	else if(file_descriptor)	{
		if(fwrite(p->msg, 1, p->len, file_descriptor) != p->len)    {
            cerr <<  "ERROR!! Could not write to file" << endl;
            exit(0);
        }

        // Close file if its last message
		if(p->final == 1)	{
			// Closing the output file
    		cout << "Closing the output file: " << filename << endl;
    		fclose(file_descriptor);
		}
	}
	else {
		cerr << "Could not write to file : File invalid!!" << endl;
		exit(0);
	}
}

// Start the server
void rdtServer::start()	{
	while(TRUE)	{
		udtRcv();
	}
 }

int main(int argc, char *argv[])
{	

	cout << "Press CTRL + C to quit the server!!" << endl;

	// Validates input arguments
    if(argc < 4)   {
        cerr << "Usage: " << argv[0] <<" [port_number] [probability_of_corruption] [probability_of_loss]" << endl;
        exit(0);
    }

    rdtServer *server = new rdtServer(argv);
    server->start();

	return 0;
}