#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>
#include <netpacket/packet.h>
#include <fcntl.h>
#include <netinet/ip.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

struct Servers{
	int port;
	char* ip;
};
struct responseTcp{
	char* Ip;
	int port;
	int id;
	char* message;
	int Type;
	int lastAck;
};
struct responseStack{
	int Type;
	struct responseTcp Tcp;
};



void sendClient(int port, char* ip, char* data,int lastAck);
struct Servers createServer(char* ip,int port);
struct responseStack listenOn(struct Servers,int socket);
int initServer();
struct responseTcp tcpHandler(uint8_t buf[],struct tcphdr *tcp_hdr,int sock,int numbytes,struct iphdr *IP_headerReceived);
int MacIsForMe(struct ethhdr *eh);
int IpIsForMe(struct iphdr *ih);
