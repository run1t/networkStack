#include "tcp.h"
#include "ip.h"
#include "icmp.h"

void makeICMP_header(struct icmphdr *icmp,char datagram[4096],u_int8_t typeICMP)
{

	char *fake_datagram;
	//On remplit le paquet
	icmp->type = typeICMP;
	icmp->code = 0;
	//icmp->checksum = checksum( 
	icmp->un.echo.id = 0;
	icmp->un.echo.sequence = 0;
	
	//int size_datagram_cs = sizeof(struct tcphdr) + sizeof(struct icmphdr) + strlen(data);
}
