#include "tcp.h"
#include "ip.h"
#include "icmp.h"

void makeICMP_header(struct icmphdr *icmp,u_int8_t typeICMP)
{

	char *fake_datagram;
	//On remplit le paquet
	icmp->type = typeICMP;
	icmp->code = 0;
	//On met le checksum à 0 avant de le calculer
	icmp->checksum = 0;
	//Valeur au hasard permettant de distinguer différent paquet ICMP
	icmp->un.echo.id = 0;
	//On met la séquence à 0
	icmp->un.echo.sequence = 0;

	//On calcule le checksum
	icmp->checksum = checksum((unsigned short*)icmp, sizeof(struct icmphdr));
}
