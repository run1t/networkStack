#include <string>
#include <iostream>
#include <sstream>
#include "ETHFrame.h"
#include "IPFrame.h"
#include <vector>
#include <cstring>
extern "C" { 
	#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <linux/tcp.h>
#include <netinet/in_systm.h>
#include <net/ethernet.h>

};
#if !defined( TCPFRAME_H )
#define TCPFRAME_H
using namespace std;
typedef uint16_t u16;
struct header_tcp_checksum
{
	u_int32_t address_source;
	u_int32_t address_destination;
	u_int16_t tcp_length;
	u_int8_t placeholder;
	u_int8_t protocol;

};
class TCPFrame
{
public:
	ETHFrame eth;
	IPFrame ip;
	int src;
	int dst;
	int seq_number;
	int ack_number;
	int HeaderLength;
	int Flags;
	int Windows;
	int Checksum;
	int urgentPointer;
	string data;
	int frameLength;
	vector<unsigned char> options;

	TCPFrame(unsigned char* buffer,int size);
	TCPFrame();
	unsigned char* toFrame();
};

#endif // !defined( TCPFRAME_H )