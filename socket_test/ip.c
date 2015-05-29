#include "tcp.h"
#include "ip.h"
#include "icmp.h"

//TODO Penser a faire un memset de datagram dans la couche superieur, On Top

//On renvoie un pointeur vers notre structure IP
void makeIP_header(struct iphdr *ip,char *data,char datagram[4096],char *destination_ip,int protocol_id){
	//(struct iphdr *) datagram;
	//On alloue la memoire necessaire a l'aide d'un malloc
	ip->ihl = 5;
	//Nous sommes en IPv4
	ip->version = 4;
	//La taille du packet ip
	if(protocol_id == IPPROTO_TCP){
		ip->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(data);
	}else if(protocol_id == IPPROTO_ICMP){
		ip->tot_len = sizeof(struct iphdr) + sizeof(struct icmphdr);
	}
	//Nous laissons le type of service a 0, comme indiaue dans la rfc 793
	ip->tos = 0;
	//On convertit l'id du paquet ip en network byte order avec htonl()
	//ip->id = htonl(123);
	//Nous ne souhaitons pas de fragmentation donc 0
	ip->frag_off = 0;
	//Nous le laissons un Time to Live par defaut a 0
	ip->ttl = 255;
	
	ip->protocol = protocol_id;

	//Le checksum est calcule plus tard
	ip->check = 0;
	//Nous convertissons les adresses IP source et destination en network byte order
	ip->saddr = inet_addr(inet_ntoa(getIp("eth0")));
	ip->daddr = inet_addr(destination_ip);

	printf("%s\n", inet_ntoa(getIp("eth0")));
	printf("%s\n", destination_ip);

	//Nous calculons le checksum IP
	ip->check = checksum((unsigned short *) datagram, ip->tot_len);
	 
}

/*int sendPacket(){
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
	char datagram[4096],*data,*destination_ip;

	destination_ip = "1.2.1.4";
	//on nettoie l'emplacement memoire du datagram
	memset(&datagram,0,4096);

	//On dit a data d'inserer son message a la fin du paquet d
	data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
	strcpy(data,"GET /repertoire/page.html HTTP/1.1");

	uint16_t dest = 80;
	uint32_t seq = 666;
	uint32_t ack_seq = 0;
	uint16_t fin = 0;
	uint16_t syn = 1;
	uint16_t ack = 0;
	

	//On crée notre structure ip Header
	struct iphdr *ip_header = (struct iphdr *)datagram;
	
	//On remplie notre structure 
	makeIP_header(ip_header,data,datagram,destination_ip);
	
	struct tcphdr *tcpHeader = (struct tcphdr *)(datagram + sizeof(struct ip));//makeTCP_segment(dest,seq,ack_seq,fin,syn,ack,datagram,data);
	printf("MAke tcp segment\n" );
	makeTCP_segment(tcpHeader,dest,seq,ack_seq,fin,syn,ack,datagram,data);
	
	printf("seq:%u\n",ip_header->ttl);
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
		int i;
		for(i=0;i<100000000;i++)
		{}
		//On envoie le datagram en passant le buffer datagram, ainsi que l'adresse de destination contenue dans la structure sockaddr
		if(sendto(sd,datagram,ip_header->tot_len,0,(struct sockaddr *) &sin,sizeof(sin))<0)
		{
			perror("Sending packet failed");
		}
		else
		{
			printf("Packet sent succesfully ");
		}
	}
	return 0;
}*/
