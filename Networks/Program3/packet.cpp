#include <iostream>
#include <string>
#include <sstream>

#include "packet.h"

using namespace std;

//Default constructor
PACKET::PACKET() {

}

//Overloaded Constructor
PACKET::PACKET(PACKET_TYPE type, int seq, char const* msg, int size, int start, int final) {
	this->type = type;
	this->seq  = seq;
	memset(this->msg, 0, BUFFER_SIZE);
	memcpy(this->msg, msg, size);
	this->len  = size;
	this->final = final;
	this->start = start;
}

//Default destructor
PACKET::~PACKET() {

}

//To print packet details
void PACKET::print() {
	cout << "Contents of the Packet are:" << endl;
	cout << "Type: " << (this->type == 0 ? "DATA" : "ACK") << endl;
	cout << "Seq Number: " << this->seq << endl;
	cout << "Start: " << this->start << endl;
	cout << "Final: " << this->final << endl;
	cout << "Data: " << this->msg << endl;
	cout << "Content Length: " << this->len << endl;
	cout << "Checksum: " << this->checksum << endl << endl;
}

unsigned short computeChecksum(const char* msg, int len)	{
	unsigned sum = 0;
	unsigned size = len;
	int i;
	//Accumulate checksum
	for (i = 0; i < size - 1; i += 2) {
		unsigned short word16 = *(unsigned short *) &msg[i];
		sum += word16;
	}
	// Handle odd-sized case
	if (size & 1) {
		unsigned short word16 = (unsigned char) msg[i];
		sum += word16;
	}
	// Fold to get the ones-complement result
	while (sum >> 16) 
		sum = (sum & 0xFFFF)+(sum >> 16);
	// Invert to get the negative in ones-complement arithmetic
	return (unsigned short)~sum;
}
