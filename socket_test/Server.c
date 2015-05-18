#include "Server.h"
#include "tcp.h"
#include "ip.h"
#include "icmp.h"
/**
* \file server.c
* \brief Fichier server
* \author Thomas Viaud
* Fichier qui permet d'initialiser le server, de le faire écouter le trafic, récuperer les packet qui lui sont destiné et gerer ceux-ci 
*/

#define DEST_MAC0	0x00
#define DEST_MAC1	0x00
#define DEST_MAC2	0x00
#define DEST_MAC3	0x00
#define DEST_MAC4	0x00
#define DEST_MAC5	0x00

#define TCP_ACK 16
#define TCP_PSH 8
#define TCP_RST 4
#define TCP_SYN 2
#define TCP_FIN 1

#define ETHER_TYPE	0x0800
#define DEFAULT_IF	"eth0"
#define BUF_SIZ		1024

//Definition des protocoles
#define ICMP_PROTO 0x01
#define TCP_PROTO 0x06
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
struct sockaddr_ll sa;
// Remplissage de la structure Seveur
struct Servers createServer(char* ip,int port){
	struct Servers s;
	s.ip = ip;
	s.port = port;
	return s;
}
u32 swap32(u32 in)
{
	u32 out = in<<24 | ((in&0xff00)<<8) | ((in&0xff0000)>>8) | ((in&0xff000000)>>24);
	return out;
}
//Initialisation de la connection
/**
* \fn int initServer()
* \brief Fonction d'initialisation du server
* \param La fonction ne recoit pas de parametres 
* \return La fonction retoure le socket 
*/

int initServer(){

	/* on mets en place le socket */
	int sockfd;
	int sockopt;


	/* */
	struct ifreq ifopts;	
	struct ifreq if_ip;
	
	
	/* On configure le nom de notre interface */
	char ifName[IFNAMSIZ];
	strcpy(ifName, DEFAULT_IF);
	memset(&if_ip, 0, sizeof(struct ifreq));
 
	/* On ouver l'ecoute des packet Ethernet */
	if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETHER_TYPE))) == -1) {
		perror("listener: socket");	
	}
 
	/* On met l'interface en promiscuis mode ce qui 
	permet d"couter tout les connections escequue 
	c'est vraiment necessaire ?*/

	strncpy(ifopts.ifr_name, ifName, IFNAMSIZ-1);
	ioctl(sockfd, SIOCGIFFLAGS, &ifopts);
	ifopts.ifr_flags |= IFF_PROMISC;
	ioctl(sockfd, SIOCSIFFLAGS, &ifopts);


	/* Gere un cas de fermeture prematurer*/
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) == -1) {
		perror("setsockopt");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	/* On "Bind" notre socket avec notre device */
	if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, ifName, IFNAMSIZ-1) == -1)	{
		perror("SO_BINDTODEVICE");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	memset(&sa, 0, sizeof (sa));
	sa.sll_family = AF_PACKET;
	sa.sll_ifindex = if_ip.ifr_ifindex;
	sa.sll_protocol = htons(ETH_P_ALL);

	return sockfd;

}

// Lancement de l'ecoute du serveur
/**
* \fn void listenOn(struct Server server,int sock)
* \brief Fonction d'écoute du server 
* \param La fonction recoit la structure de server(ip, port), et le socket
* \return La fonction ne retourne rien 
*/
char* listenOn(struct Servers server,int sock){
	//Permet d'avoir une taille optimale pour le buffer
	uint8_t buf[BUF_SIZ];
	int numbytes;
	struct ethhdr *etherHeader;
	struct iphdr *ip_hdr;
	struct tcphdr *tcp_hdr;
	struct icmphdr *icmp_hdr;
	
	numbytes = recvfrom(sock, buf, BUF_SIZ, 0, NULL, NULL);
		
	//On verifie que l'on a bien des données
	if(numbytes > 0){

	//On recupère le header
	etherHeader = (struct ethhdr *)buf;

	//Quelle est le type de ethernet ?
			
	if(etherHeader->h_proto == ETH_P_ARP){
		printf(" Arp\n");
	}else if(etherHeader->h_proto == htons(0x0800)){
		//On check si on la bonne address mac
		if(MacIsForMe(etherHeader)){
			//Recuperation du header ip
			ip_hdr = (struct iphdr *)(buf + sizeof(struct ethhdr));
			
			//On ne traite que l'ip V4

			if(ip_hdr->version == 4){
				//On check si c'est la bonne address IP
				if(IpIsForMe(ip_hdr)){
					switch(ip_hdr->protocol){
						//#ICMP
						case ICMP_PROTO:
							icmp_hdr = (struct icmphdr *)(buf +sizeof(struct ethhdr) + sizeof(struct iphdr));
							icmpHandler(icmp_hdr,buf,numbytes);
						break;

						//#TCP
						case TCP_PROTO:
							tcp_hdr = (struct tcphdr *)(buf + sizeof(struct ethhdr) + sizeof(struct iphdr));
							return tcpHandler(buf,tcp_hdr,sock,numbytes);
				
						break;

						}
					}
				}
		    }
	    }
    }	
    return "NULL";

}

int MacIsForMe(struct ethhdr *eh){

	/**
	* On recupère l'adresse Mac de la machine
	*/ 

	int sockfd;
	struct ifreq ifr;
	unsigned char *macDst;
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name,DEFAULT_IF,IFNAMSIZ-1);
	sockfd = socket(AF_INET,SOCK_STREAM,0);

	if(sockfd == -1){
		perror("Error socket opening MAC Address");
	}
	ioctl(sockfd,SIOCGIFHWADDR,&ifr);
	close(sockfd);
	
	/******/
	macDst = (unsigned char *)ifr.ifr_hwaddr.sa_data;	
	
	if( macDst[0] == eh->h_dest[0] &&
		macDst[1] == eh->h_dest[1] &&
		macDst[2] == eh->h_dest[2] &&
		macDst[3] == eh->h_dest[3] &&
		macDst[4] == eh->h_dest[4] &&
		macDst[5] == eh->h_dest[5]){
		return 1;
	}else{
		return 0;
	}

}


int IpIsForMe(struct iphdr *ih){

	/**
	* Recuperation de l'ip de la machine
	*/

	int fd;
 	struct ifreq ifr;
 	fd = socket(AF_INET, SOCK_DGRAM, 0);
 	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, DEFAULT_IF, IFNAMSIZ-1);
	ioctl(fd, SIOCGIFADDR, &ifr);
	close(fd);

	/**
	* Recuperation de l'ip destination
	*/ 
	
	char src_ip[20], dest_ip[20];
	strcpy(src_ip, inet_ntoa( *(struct in_addr *) &ih->daddr) );

	/**
	* On compare les deux
	*/

	if (strcmp(src_ip,inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr)) == 0)
      return 1;
   	else
      return 0;
	
}

char* tcpHandler(uint8_t buf[],struct tcphdr *tcp_hdr,int sock,int numbytes){
	
	/**
	* Dans cette partit nous allons gerer les syn ack etc..
	*/
	
	//On recupere le buffer
	unsigned char * tosend;
	tosend = (void*)malloc(BUF_SIZ);

	//Definition des headers
	struct ethhdr* eth_hdr = (struct ethhdr *)buf;
	struct iphdr* ip_hdr = (struct iphdr *)(buf + sizeof(struct ethhdr));
	

	/*Gestion du Syn*/
	if(tcp_hdr->syn == 1 &&  tcp_hdr->ack == 0){
		
		/**
		* Reception du [SYN] 
		* Lors de la recption du SYN nous devons renboyer un [SYN|ACK]
		* au client
		*
		* Ethernet response 
		*	destination<=source
		*	source=>destination
		*   ip -> ipv4
		* 
		* Ip response
		*	version = ipv4
		*/


		memcpy((void*)tosend, (void*)buf, BUF_SIZ); // make a copy of the original frame
		
		// On change le header ethernet pour intervertir les address mac
		memcpy(eth_hdr->h_dest, eth_hdr->h_source, 6);
		memcpy(eth_hdr->h_source, eth_hdr->h_dest, 6);
		
		//struct pour le socket
		struct sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_port = htons(80);
		sin.sin_addr.s_addr = inet_addr("1.2.3.4");

		//On ouvre un socket
		int sd = socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
		
		//datagram a envoyer, et la data associe
		char datagram[4096],*data,*destination_ip;

		//Ip destination
		destination_ip = "10.17.19.94";

		//on nettoie l'emplacement memoire du datagram
		memset(&datagram,0,4096);

		//On dit a data d'inserer son message a la fin du paquet d
		data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
		strcpy(data,"");

		//On crée notre structure ip Header
		struct iphdr *ip_header = (struct iphdr *)datagram;
		ip_header->id = ip_hdr->id;
		makeIP_header(ip_header,data,datagram,destination_ip,IPPROTO_TCP);
		

		struct tcphdr *tcpHeader = (struct tcphdr *)(datagram + sizeof(struct ip));//makeTCP_segment(dest,seq,ack_seq,fin,syn,ack,datagram,data);

		tcpHeader->window = htons(tcp_hdr->window);
		
		makeTCP_segment(tcpHeader,
		   /* port dest */   htons(tcp_hdr->source),
		   /* seq Num   */   0,
		   /* ack Num   */   swap32(swap32(tcp_hdr->seq)+1),
		   /* Fin       */   0,
		   /* Syn	    */   1,
		   /* Ack       */   1,
		   /* Data      */   data,
		   /* Psh       */   0);
		
		//On met une option sur le socket pour signaler que l'on fournit les header IP et TCP
		//Premierement on passe le fileDescriptor,puis le niveau de l'option, et l'option a effectue
		int one = 1;
		const int *val = &one;
		setsockopt(sd,IPPROTO_IP,IP_HDRINCL,val,sizeof(one));

		//On envoie le datagram en passant le buffer datagram, ainsi que l'adresse de destination contenue dans la structure sockaddr
		sendto(sd,datagram,ip_header->tot_len,0,(struct sockaddr *) &sin,sizeof(sin));
		
		//Fermeture du fileDescriptor
		close(sd);
		return "Connection";

	}else if(tcp_hdr->ack == 1 && tcp_hdr->psh == 1){
		

		
		memcpy((void*)tosend, (void*)buf, BUF_SIZ); // make a copy of the original frame
		
		// On change le header ethernet pour intervertir les address mac
		memcpy(eth_hdr->h_dest, eth_hdr->h_source, 6);
		memcpy(eth_hdr->h_source, eth_hdr->h_dest, 6);
		
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

		destination_ip = "10.17.19.94";
		//on nettoie l'emplacement memoire du datagram
		memset(&datagram,0,4096);

		//On dit a data d'inserer son message a la fin du paquet d
		data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
	
		strcpy(data,"");
		
		uint16_t dest = htons(tcp_hdr->source);
		uint32_t seq = htonl(ntohl(tcp_hdr->ack_seq));
		uint32_t ack_seq = htonl(ntohl(tcp_hdr->seq)+1+4+numbytes-60);
		uint16_t fin = 0;
		uint16_t syn = 0;
		uint16_t ack = 1;
		uint16_t psh = 0;

		//On crée notre structure ip Header
		struct iphdr *ip_header = (struct iphdr *)datagram;
		
		//On remplie notre structure 
		ip_header->id = ip_hdr->id;
		ip_header->ttl = ip_hdr->ttl;
		makeIP_header(ip_header,data,datagram,destination_ip,IPPROTO_TCP);
		
		struct tcphdr *tcpHeader = (struct tcphdr *)(datagram + sizeof(struct ip));//makeTCP_segment(dest,seq,ack_seq,fin,syn,ack,datagram,data);

		tcpHeader->window = htons(tcp_hdr->window);
		makeTCP_segment(tcpHeader,dest,seq,ack_seq,fin,syn,ack,data,psh);
		int one = 1;
		const int *val = &one;
		
		//On met une option sur le socket pour signaler que l'on fournit les header IP et TCP
		//Premierement on passe le fileDescriptor,puis le niveau de l'option, et l'option a effectue
		
		if(setsockopt(sd,IPPROTO_IP,IP_HDRINCL,val,sizeof(one)) < 0)
		{
			perror("error in setting options to the socket");
			exit(0);
		}

		
			
		//On envoie le datagram en passant le buffer datagram, ainsi que l'adresse de destination contenue dans la structure sockaddr
		if(sendto(sd,datagram,ip_header->tot_len,0,(struct sockaddr *) &sin,sizeof(sin))<0)
		{
			perror("Sending packet failed");
		}
		else
		{
			//printf("Packet sent succesfully ");
		}
		
		char webpage[] =
			"HTTP/1.1 404 Not Found\n";
		strcpy(data,webpage);

		struct iphdr *ip_header2 = (struct iphdr *)datagram;
		//On remplie notre structure 
		makeIP_header(ip_header2,data,datagram,destination_ip,IPPROTO_TCP);
		struct tcphdr *tcpHeader2 = (struct tcphdr *)(datagram + sizeof(struct ip));//makeTCP_segment(dest,seq,ack_seq,fin,syn,ack,datagram,data);

		
		
		
		psh = 0;
		ack = 1;

		makeTCP_segment(tcpHeader2,dest,seq,ack_seq,fin,syn,ack,data,psh);
		
		


		//On envoie le datagram en passant le buffer datagram, ainsi que l'adresse de destination contenue dans la structure sockaddr
		if(sendto(sd,datagram,ip_header->tot_len,0,(struct sockaddr *) &sin,sizeof(sin))<0)
		{
			perror("Sending packet failed");
		}
		else
		{
			//printf("Packet sent succesfully ");
		}
		close(sd);
	
	}else if(tcp_hdr->ack == 1 && tcp_hdr->fin == 1){
		

		
		memcpy((void*)tosend, (void*)buf, BUF_SIZ); // make a copy of the original frame
		
		// On change le header ethernet pour intervertir les address mac
		memcpy(eth_hdr->h_dest, eth_hdr->h_source, 6);
		memcpy(eth_hdr->h_source, eth_hdr->h_dest, 6);
		
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

		destination_ip = "10.17.19.94";
		//on nettoie l'emplacement memoire du datagram
		memset(&datagram,0,4096);

		//On dit a data d'inserer son message a la fin du paquet d
		data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
		
		strcpy(data,"");
	
		uint16_t dest = htons(tcp_hdr->source);
		uint32_t seq = htonl(ntohl(tcp_hdr->ack_seq));
		uint32_t ack_seq = htonl(ntohl(tcp_hdr->seq)+1+numbytes-60);
		uint16_t fin = 1;
		uint16_t syn = 0;
		uint16_t ack = 1;
		uint16_t psh = 0;

		//On crée notre structure ip Header
		struct iphdr *ip_header = (struct iphdr *)datagram;
		
		//On remplie notre structure 
		ip_header->id = ip_hdr->id;
		ip_header->ttl = ip_hdr->ttl;
		makeIP_header(ip_header,data,datagram,destination_ip,IPPROTO_TCP);
		
		struct tcphdr *tcpHeader = (struct tcphdr *)(datagram + sizeof(struct ip));//makeTCP_segment(dest,seq,ack_seq,fin,syn,ack,datagram,data);

		tcpHeader->window = htons(tcp_hdr->window);

		makeTCP_segment(tcpHeader,dest,seq,ack_seq,fin,syn,ack,data,psh);
		

		int one = 1;
		const int *val = &one;
		
		//On met une option sur le socket pour signaler que l'on fournit les header IP et TCP
		//Premierement on passe le fileDescriptor,puis le niveau de l'option, et l'option a effectue
		
		if(setsockopt(sd,IPPROTO_IP,IP_HDRINCL,val,sizeof(one)) < 0)
		{
			perror("error in setting options to the socket");
			exit(0);
		}

		
			
		//On envoie le datagram en passant le buffer datagram, ainsi que l'adresse de destination contenue dans la structure sockaddr
		if(sendto(sd,datagram,ip_header->tot_len,0,(struct sockaddr *) &sin,sizeof(sin))<0)
		{
			perror("Sending packet failed");
		}
		else
		{
			//printf("Packet sent succesfully ");
		}

		close(sd);
	}

	return "NULL";
}
