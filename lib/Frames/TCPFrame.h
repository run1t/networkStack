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
#if !defined( UINT16 )
#define UINT16
typedef uint16_t u16;
#endif

#if !defined( TCPFRAME_H )
#define TCPFRAME_H
using namespace std;
typedef uint16_t u16;

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
	unsigned short checksum(unsigned short *ptr, unsigned int nbBytes);
	unsigned short get_tcp_checksum(struct iphdr * myip, struct tcphdr * mytcp);
	unsigned short get_ip_checksum(struct iphdr * myip);
	TCPFrame(unsigned char* buffer);
	TCPFrame();
	unsigned char* toFrame();
};

#endif // !defined( TCPFRAME_H )