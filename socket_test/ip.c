#include "tcp.h"
#include "ip.h"

struct iphdr makeIP_header(char *data,char *destination_ip){
	struct iphdr ip_header;

	char datagram[4096];

	//On nettoie notre tableau en le remplissant de zero
	memset(datagram,0,4096);

	ip_header.ihl = 5;
	//Nous sommes en IPv4
	ip_header.version = 4;
	//La taille du packet ip
	ip_header.tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(data);
	//Nous laissons le type of service a 0, comme indiaue dans la rfc 793
	ip_header.tos = 0;
	//On convertit l'id du paquet ip en network byte order avec htonl()
	ip_header.id = htonl(123);
	//Nous ne souhaitons pas de fragmentation donc 0
	ip_header.frag_off = 0;
	//Nous le laissons un Time to Live par defaut a 0
	ip_header.ttl = 255;
	//Le protocole de la couche superieur est TCP
	ip_header.protocol = IPPROTO_TCP;
	//Le checksum est calcule plus tard
	ip_header.check = 0;
	//Nous convertissons les adresses IP source et destination en network byte order
	ip_header.saddr = inet_addr(inet_ntoa(getIp("eth0")));
	ip_header.daddr = inet_addr(destination_ip);

	//Nous calculons le checksum IP
	ip_header.check = checksum((unsigned short *) datagram, ip_header.tot_len);

	return ip_header;
}
