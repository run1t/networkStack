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

struct Servers createServer(char* ip,int port);
char* listenOn(struct Servers,int socket);
int initServer();
char* tcpHandler(uint8_t buf[],struct tcphdr *tcp_hdr,int sock,int numbytes,struct iphdr *IP_headerReceived);
int MacIsForMe(struct ethhdr *eh);
int IpIsForMe(struct iphdr *ih);
