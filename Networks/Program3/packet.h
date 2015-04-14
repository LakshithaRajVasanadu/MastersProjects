#include <iostream>
#include <string>
#include <sstream>

#define BUFFER_SIZE 256

using namespace std;

enum PACKET_TYPE { DATA, ACK };

struct PACKET {
    PACKET_TYPE    type;      			// DATA frame or ACK frame
    int            seq;         		// 0 or 1
    int            len;         		// length of the msg field only
    int            checksum;    		// checksum of the whole frame 
    char           msg[BUFFER_SIZE];
    int 		   final;
    int            start;

    PACKET();
    PACKET(PACKET_TYPE, int, char const*, int, int, int);
    ~PACKET();
    void print();
};

unsigned short computeChecksum(const char* msg, int len); // To compute checksum of a string