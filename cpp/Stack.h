#define TCP_ACK 16
#define TCP_PSH 8
#define TCP_RST 4
#define TCP_SYN 2
#define TCP_FIN 1

#define ETHER_TYPE	0x0800
#define DEFAULT_IF	"eth0"
#define BUF_SIZ		1024
#define ETHERTYPE_ARP 0x0806
#define ETHERTYPE_IPv4 0x0800

//Definition des protocoles
#define ICMP_PROTO 0x01
#define TCP_PROTO 0x06
#include "Frames/ETHFrame.h"
#include "Frames/IPFrame.h"
#include "Frames/TCPFrame.h"
#include "Frames/ICMPFrame.h"
#include "Frames/ARPFrame.h"

#include "PC.h"
#include <string>
#include <iostream>
#include <errno.h>
#include <unistd.h>

using namespace std;

extern "C" {    // another way
	#include <sys/socket.h>
    #include <netpacket/packet.h>
    #include <net/ethernet.h>

};


class Stack
{	
public:
	/*Attributs de la stack */
	string IP;
	int  port;
	int sock;
	/*Methode de la stack*/ 
	Stack(string ip,int port);
	void receiver();
	void Sender(TCPFrame tcp);
	void Sender(ICMPFrame icmp);
	void Sender(ARPFrame arp);

};