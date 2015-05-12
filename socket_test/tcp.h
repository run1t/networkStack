#include <stdio.h>
#include <unistd.h>
#include <string.h>
//Pour avoir accès à malloc
#include <stdlib.h>

#include <sys/types.h>
//Pour avoir accès à ioctl qui donne accès aux informations du kernel
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
//Structure du header TCP
#include <netinet/tcp.h>
//Structure header IP
#include <netinet/ip.h>
//12 octets header nécessaire pour le calcul du checksum
struct header_tcp_checksum
{
	u_int32_t address_source;
	u_int32_t address_destination;
	u_int16_t tcp_length;
	u_int8_t placeholder;
	u_int8_t protocol;

};

struct in_addr  getIp(char* interface);
//*ptr pointeur vers la donnée à sortir le checksum, nbBytes nombre de bytes du header à checksumé
unsigned short checksum(unsigned short *ptr,int nbBytes);

void makeTCP_segment(struct tcphdr *tcp_segment,uint16_t dest,uint32_t seq,uint32_t ack_seq,uint16_t fin,uint16_t syn,uint16_t ack,char datagram[4096],char *data,uint16_t psh);

int tcp_sniffer();

void print_tcp_header(unsigned char* Buffer, int Size);
