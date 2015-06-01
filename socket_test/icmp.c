#include "tcp.h"
#include "ip.h"
#include "icmp.h"
#include <fcntl.h>
/**
* \file icmp.c
* \brief Fichier d'envoie de de packet ICMP 
* \author Thomas Viaud
* Fichier qui permet l'envoie de packet ICMP pour réaliser un ping sur la machine 
*/
/**
* \fn void makeICMP_header (struct icmphdr *icmp,u_int8_t typeICMP)
* \brief Fonction de création de l'en-tete du packet ICMP (pour le ping)
* \param une structure qui contiend le header ICMP ainsi que le type de réponse (echo ou reply)
* \return Ne retourne rien car la fonction sert a forger l'en-tete ICMP
*/
void makeICMP_header(struct icmphdr *icmp,u_int8_t typeICMP)
{
	//On remplit le paquet
	icmp->type = typeICMP;
	icmp->code = 0;
	//On met le checksum à 0 avant de le calculer
	icmp->checksum = 0;
	//Valeur au hasard permettant de distinguer différent paquet ICMP
	icmp->un.echo.id = htons(rand());
	//On met la séquence à 0
	icmp->un.echo.sequence = htons(1);
	//On calcule le checksum
	icmp->checksum = checksum_ICMP((unsigned short*)icmp, sizeof(struct icmphdr));
}

//\fn void sendICMP_request()
void sendICMP_request()
{
	//Le file descriptor de notre socket, ainsi que pour le setsockopt
	//struct pour le socket
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr("192.168.1.26");

	//On ouvre un socket
	int sd = socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
		
	//datagram a envoyer, et la data associe
	char datagram[4096],*data,*destination_ip;

	//Ip destination
	destination_ip = "192.168.1.23";

	//on nettoie l'emplacement memoire du datagram
	memset(&datagram,0,4096);

	//On dit a data d'inserer son message a la fin du paquet d
	data = datagram + sizeof(struct iphdr) + sizeof(struct icmphdr);
	strcpy(data,"");

	//On crée notre structure ip Header
	struct iphdr *ip_header = (struct iphdr *)datagram;
	makeIP_header(ip_header,data,datagram,destination_ip,IPPROTO_ICMP);


	
	//Notre header ICMP
	struct icmphdr *ICMPheader = (struct icmphdr *)(data);
	makeICMP_header(ICMPheader,ICMP_ECHOREPLY);
	printf(" code ICMP%d",ICMPheader->code);


	
	//On ouvre le socket
	if((sd = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP)) == -1){
		perror("Could not load ICMP socket!");
	}
	
	int i = 4;
	while(i > 0){
	if(sendto(sd,data,ip_header->tot_len,0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) < 0){
		printf("Sending ICMP\n");
		sleep(2);
	}
	}
}

//\fn void icmpHandler(struct icmphdr *icmpHeader)
void icmpHandler(struct icmphdr *icmpHeader)
{
	//Si c'est un echo alors on désactive la réponse du kernel aux échos
	if(icmpHeader->type == ICMP_ECHO)
	{
		printf("ICMP ECHO Received \n");
		// le file descriptor du fichier /proc à modifier
		int fdProc = open("/proc/sys/net/ipv4/icmp_echo_ignore_all",O_WRONLY);
		if(fdProc == -1){
			perror("Error opening the file /proc/sys/net/ipv4/icmp_echo_ignore_all\n");
		}
		int nbByteWrite = write(fdProc,"1",1);
		if(nbByteWrite == 0){
			printf("No write\n");
		}
		else if(nbByteWrite == -1){
			perror("Error writing to file\n");
		}
		else if(nbByteWrite > 0){
			sendICMP_request();
			printf("Succes writing to file\n");
			int closeSuccess = close(fdProc);
			if(closeSuccess == 0){
				printf("File closed successfully\n");
			}else if(closeSuccess == -1){
				printf("Error closing the file\n");
			}
		}
	}
}
