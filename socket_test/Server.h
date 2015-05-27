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
struct Server{
	int port;
	char* ip;
};

struct Server createServer(char* ip,int port);
void listenOn(struct Server,int socket);
int initServer();
void tcpHandler(uint8_t buf[],struct tcphdr *tcp_hdr,int sock,int numbytes);