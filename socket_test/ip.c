#include "tcp.h"
#include "ip.h"
#include "icmp.h"

//TODO Penser a faire un memset de datagram dans la couche superieur, On Top

//On renvoie un pointeur vers notre structure IP
void makeIP_header(struct iphdr *ip,char *data,char datagram[4096],uint32_t destination_ip,int protocol_id){
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
	ip->daddr = destination_ip;


	//Nous calculons le checksum IP
	ip->check = checksum((unsigned short *) datagram, ip->tot_len);
	 
}
