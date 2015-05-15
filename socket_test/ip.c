#include "tcp.h"
#include "ip.h"

struct iphdr *makeIP_header(char *data,char datagram[4096],char *destination_ip){
	struct iphdr *ip_header;
	//On alloue la memoire necessaire a l'aide d'un malloc
	ip_header = malloc(sizeof(struct iphdr));
	//On nettoie notre tableau en le remplissant de zero
	memset(datagram,0,4096);

	ip_header->ihl = 5;
	//Nous sommes en IPv4
	ip_header->version = 4;
	//La taille du packet ip
	ip_header->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(data);
	//Nous laissons le type of service a 0, comme indiaue dans la rfc 793
	ip_header->tos = 0;
	//On convertit l'id du paquet ip en network byte order avec htonl()
	ip_header->id = htonl(123);
	//Nous ne souhaitons pas de fragmentation donc 0
	ip_header->frag_off = 0;
	//Nous le laissons un Time to Live par defaut a 0
	ip_header->ttl = 255;
	//Le protocole de la couche superieur est TCP
	ip_header->protocol = IPPROTO_TCP;
	//Le checksum est calcule plus tard
	ip_header->check = 0;
	//Nous convertissons les adresses IP source et destination en network byte order
	ip_header->saddr = inet_addr(inet_ntoa(getIp("eth0")));
	ip_header->daddr = inet_addr(destination_ip);

	//Nous calculons le checksum IP
	ip_header->check = checksum((unsigned short *) datagram, ip_header->tot_len);

	return ip_header;
}

int sendPacket(struct iphdr *ip_header,struct tcphdr *tcp_header){
	//struct pour le socket
	struct sockaddr_in sin;

	sin.sin_family = AF_INET;
	sin.sin_port = htons(80);
	sin.sin_addr.s_addr = inet_addr("1.2.3.4");
	
	//On ouvre un socket
	int sd = socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
	if(sd == -1)
	{
		perror("Error in creating socket");
		exit(1);
	}

	//datagram a envoyer, et la data associe
	char datagram[4096],*data;
	//on nettoie l'emplacement memoire du datagram
	memset(datagram,0,4096);

	//On dit a data d'inserer son message a la fin du paquet
	data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
	strcpy(data,"hello World");

	int one = 1;
	const int *val = &one;
	
	//On met une option sur le socket pour signaler que l'on fournit les header IP et TCP
	//Premierement on passe le fileDescriptor,puis le niveau de l'option, et l'option a effectue
	if(setsockopt(sd,IPPROTO_IP,IP_HDRINCL,val,sizeof(one)) < 0)
	{
		perror("error in setting options to the socket");
		exit(0);
	}

	while(1)
	{
		//On envoie le datagram en passant le buffer datagram, ainsi que l'adresse de destination contenue dans la structure sockaddr
		if(sendto(sd,datagram,ip_header->tot_len,0,(struct sockaddr *) &sin,sizeof(sin))<0)
		{
			perror("Sending packet failed");
		}
		else
		{
			printf("Packet sent succesfully");
		}
	}
	return 0;
}
