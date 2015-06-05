#include "tcp.h"
#include "ip.h"
#include "icmp.h"
#include <fcntl.h>

void makeICMP_header(struct icmphdr *icmp,u_int8_t typeICMP,int seq_number,int id_number)
{
	//On remplit le paquet
	icmp->type = typeICMP;
	icmp->code = 0;
	//On met le checksum à 0 avant de le calculer
	icmp->checksum = 0;
	//Valeur au hasard permettant de distinguer différent paquet ICMP
	icmp->un.echo.id = htons(rand());

	if(typeICMP == ICMP_ECHOREPLY){
		printf("seq: %d\n",seq_number);
		icmp->un.echo.sequence = htons(seq_number + 1);
		icmp->un.echo.id = id_number;
	}else if(typeICMP == ICMP_ECHO){
		icmp->un.echo.sequence = htons(1);
	}
	//On calcule le checksum
	icmp->checksum = checksum_ICMP((unsigned short*)icmp, sizeof(struct icmphdr));
}

void sendICMP_request(struct icmphdr *ICMP_received,int type_ICMP,uint8_t buf[],int numbytes,struct iphdr *IP_header)
{
	//Le file descriptor de notre socket, ainsi que pour le setsockopt
	//struct pour le socket
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr("10.17.19.85");

	//On ouvre un socket
	int sd;
		
	//datagram a envoyer, et la data associe
	char datagram[4096],*data;

	//on nettoie l'emplacement memoire du datagram
	memset(&datagram,0,4096);

	//On dit a data d'inserer son message a la fin du paquet d
	data = datagram + sizeof(struct iphdr) + sizeof(struct icmphdr);
	strcpy(data,"");

	//On crée notre structure ip Header
	struct iphdr *ip_header = (struct iphdr *)datagram;
	makeIP_header(ip_header,data,datagram,IP_header->saddr,IPPROTO_ICMP);

	
	//Notre header ICMP
	struct icmphdr *ICMPheader = (struct icmphdr *)(data);
	makeICMP_header(ICMPheader,type_ICMP,ICMP_received->un.echo.sequence,ICMP_received->un.echo.id);
	
	//On ouvre le socket
	if((sd = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP)) == -1){
		perror("Could not load ICMP socket!");
	}
	
	int i = 4;
	int nbSent = 0;
	while(i > 0){
	if(nbSent = sendto(sd,data,ip_header->tot_len,0,(struct sockaddr *)&sin, sizeof(sin)) > 0){
		printf("Sending ICMP\n");
		sleep(2);
	}else{
		printf("Failed Sending ICMP\n");
		printf("%d\n",errno);
		sleep(2);
		}
	}
}

void icmpHandler(struct icmphdr *icmpHeader,uint8_t buf[],int numbytes,struct iphdr *IP_header)
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
			sendICMP_request(icmpHeader,ICMP_ECHOREPLY,buf,numbytes,IP_header);
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
