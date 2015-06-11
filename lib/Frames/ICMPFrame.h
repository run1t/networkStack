/**
 * \file ICMPFrame.h
 * \brief fichier qui permet de lire les trames ICMP bruts
 * \author LE NOC Reunan, VIAUD Thomas, HIPEAU Kevin, TRICHARD Guillaume
 * \version 0.1
 *
 */
#include <string>
#include <iostream>
#include <sstream>
#include "ETHFrame.h"
#include "IPFrame.h"
#include <vector>
#include <cstring>
#include <cstdlib>
extern "C" { 
	#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <linux/tcp.h>
#include <netinet/in_systm.h>
#include <net/ethernet.h>


};

#if !defined( UINT16 )
#define UINT16
typedef uint16_t u16;
#endif

#if !defined( ICMPFRAME_H )
#define ICMPFRAME_H
using namespace std;

struct header_tcp_checksum
{
	u_int32_t address_source;
	u_int32_t address_destination;
	u_int16_t tcp_length;
	u_int8_t placeholder;
	u_int8_t protocol;

};

class ICMPFrame
{
public:
	ETHFrame eth;
	IPFrame ip;
	
	int Type;
	int Code;
	int Checksum;
	int Id;
	int sequence;
	string data;

	int frameLength;
	vector<unsigned char> options;
	unsigned short checksum(unsigned short *ptr, unsigned int nbBytes);
	unsigned short get_ip_checksum(struct iphdr * myip);
	unsigned short get_icmp_checksum(struct icmphdr * myicmp,int length);
	ICMPFrame(unsigned char* buffer);
	ICMPFrame();
	unsigned char* toFrame();
};

#endif // !defined( ICMPFRAME_H )