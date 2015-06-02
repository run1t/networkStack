#include "tcp.h"
#include "ip.h"
#include "icmp.h"

//TODO Penser a faire un memset de datagram dans la couche superieur, On Top
/**
* \file ip.c
* \brief Fichier de création de l'en tete IP 
* \author Thomas VIAUD, Reunan LE NOC, Kevin HIPEAU, Guillaume TRICHARD
* Fichier qui permet de créer l'en tete IP pour l'envoie de packet 
*/
/**
* \fn void makeIP_header(struct iphdr *ip,char *data,char datagram[4096],char *destination_ip,int protocol_id)
* \brief Fonction de créartion de l'en tete IP 
* \param La fonction recoit en parametre la structure de l'en tete IP, le packet a transformer, l'adresse IP de destination, l'identifiant du protocole 
* \return La fonction ne retourne rien car elle sert a structurer l'en tete IP 
*/
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
