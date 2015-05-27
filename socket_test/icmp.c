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

void sendICMP_request()
{
	//Le file descriptor de notre socket, ainsi que pour le setsockopt
	int sockfd,value = 1;
	struct sockaddr_in socket_connection;

	//On set les paramètres de notre socket
	socket_connection.sin_family = AF_INET;
	//On set l'adresse source de notre socket avec la fonction getIp
	socket_connection.sin_addr.s_addr = inet_addr(getIp("eth0"));

	//On ouvre le socket
	if(sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_ICMP) == -1){
		perror("Could not load ICMP socket!");
}

	if(setsockopt(sockfd,IPPROTO_IP,IP_HDRINCL,&value,sizeof(int))){
		perror("Could not load Custom IPHEADER!);
	}
