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
	icmp->un.echo.id = htons(rand());
	//On met la séquence à 0
	icmp->un.echo.sequence = htons(1);

	//On calcule le checksum
	icmp->checksum = checksum_ICMP((unsigned short*)icmp, sizeof(struct icmphdr));
}

void sendICMP_request()
{
	//Le file descriptor de notre socket, ainsi que pour le setsockopt
	//struct pour le socket
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr("10.17.18.62");

	//On ouvre un socket
	int sd = socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
		
	//datagram a envoyer, et la data associe
	char datagram[4096],*data,*destination_ip;

	//Ip destination
	destination_ip = "10.17.18.62";

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
	makeICMP_header(ICMPheader,ICMP_ECHO);
	printf(" code de ouf%d",ICMPheader->code);


	
	//On ouvre le socket
	if((sd = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP)) == -1){
		perror("Could not load ICMP socket!");
	}
	

	int i = 4;
	while(i > 0){
	if(sendto(sd,data,ip_header->tot_len,0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) < 0){
		printf("fail\n");
	}else{
		printf(" ok\n");
	}
	printf("Sending ICMP\n");
	sleep(2);
	}
}
