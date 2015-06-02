#include "tcp.h"
/**
* \file tcp.c
* \brief Fichier de création de de l'en tete TCP ainsi que le checksum 
* \author Thomas VIAUD, Reunan LE NOC, Kevin HIPEAU, Guillaume TRICHARD
* Fichier qui permet de créer l'en tete IP pour l'envoie de packet
*/
/**
* \fn struct in_addr  getIp(char* interface)
* \brief Fonction de récupération de l'adresse IP d'une interface 
* \param La fonction recoit en parametre l'interface sur laquelle nous souhaitons connaitre l'adresse IP 
* \return La fonction retourne une structure "in_addr" qui contiend l'ip de l'interface 
*/
struct in_addr  getIp(char* interface){
	int sockfd;
	//Interface request pour avoir l'adresse IP
	struct ifreq ifr;
	//Structure contenant les informations du socket server
	struct sockaddr_in server_si;
	
	//On remplit de zéro la structure, on la nettoie
	memset(&server_si,0,sizeof(server_si));
	server_si.sin_family = AF_INET;
	//On met l'adresse ip du serveur, INADDR_ANY est une constante qui permet d'avoir l'adresse IP par défaut
	server_si.sin_addr.s_addr = INADDR_ANY;
	//On choisit le port de l'application, htons() permet de mettre l'écriture de int en byte pour le réseau
	server_si.sin_port = htons(4000);

	//On cherche à avoir une IP ipv4
	ifr.ifr_addr.sa_family = AF_INET;

	//On copie dans ifr.ifr_name le nom de l'interface désirée ici eth0, on spécifie la taille a copier avec IFNAMSIZ
	strncpy(ifr.ifr_name,interface,IFNAMSIZ-1);

	
	//On créée un socket de la famille INET en gros IP
	//De type SOCK_STREAM c'est à dire TCP, et de protocol 0 en général TCP
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1){
		//Si il y a une erreur on la renvoie
		perror("Error Socket");
	}
//	printf("%d\n",sockfd);
	
	//On demande au kernel l'adresse IP, et il nous remplit la structure ifr
	ioctl(sockfd,SIOCGIFADDR,&ifr);

	close(sockfd);
	//inet_ntoa sert à transformer une addresse en byte en texte readable, on lui passe l'addresse de notre adresse ipV4, mais on cast par sockaddr_in pour avoir accès à sin_addr
	return ((struct sockaddr_in * )&ifr.ifr_addr)->sin_addr;
}
/**
* \fn unsigned short checksum(unsigned short *ptr,int nbBytes)
* \brief Fonction de création du checksum 
* \param La fonction recoit en parametre le packet sur lequel on souhaite réaliser le check sum et la taille de celui-ci 
* \return La fonction retourne le checksum 
*/
unsigned short checksum(unsigned short *ptr,int nbBytes){
	//register permet de dire au compilateur que l'on souhaite que la variable soit mise dans le registre processeur
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum = 0;
	while(nbBytes>1){
	//on avance dans le pointeur et on incrémente la valeur de sum avec la valeur du pointeur à sa position
		sum+=*ptr++;
	//On décrémente le nb de Bytes de 2 étant donné que le pointeur est un short(donc tient sur 2 bytes, 16 bits)
		nbBytes-=2;
	}

	sum += 6<<8 | ((6&0xff00)>>8);
	//Si on tombe sur un header impair alors on lui rajoute la donnée nécessaire pour qu'il soit pair
	if(nbBytes==1){
		oddbyte = 0;
		*((u_char *)&oddbyte) = *(u_char*)ptr;
		sum+=oddbyte;
	}

	//On bouge la somme de 16 bits vers la droite en gros divisé par 65536 (16 bits) et on lui ajoute et on ajoute le reste avec le low_order de 16 bits
	sum = (sum>>16)+(sum & 0xffff);
	//On redécalle le sum de 16bits 
	sum = sum + (sum>>16);
	//On fait le complément de la somme avec l'opérateur ~(bitwise NOT), autrement dit on donne l'inverse au niveau des bits 1=0 et 0=1;
	answer=(short)~sum;
	//On retourne le checksum
	return(answer);
}
/**
* \fn unsigned short checksum_ICMP(unsigned short *ptr,int nbBytes)
* \brief Fonction de création du checksum ICMP
* \param La fonction recoit en parametre le packet sur lequel on souhaite réaliser le check sum et la taille de celui-ci 
* \return La fonction retourne le checksum ICMP
*/
unsigned short checksum_ICMP(unsigned short *ptr,int nbBytes){
	//register permet de dire au compilateur que l'on souhaite que la variable soit mise dans le registre processeur
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum = 0;
	while(nbBytes>1){
	//on avance dans le pointeur et on incrémente la valeur de sum avec la valeur du pointeur à sa position
		sum+=*ptr++;
	//On décrémente le nb de Bytes de 2 étant donné que le pointeur est un short(donc tient sur 2 bytes, 16 bits)
		nbBytes-=2;
	}
	

	//Si on tombe sur un header impair alors on lui rajoute la donnée nécessaire pour qu'il soit pair
	if(nbBytes==1){
		oddbyte = 0;
		*((u_char *)&oddbyte) = *(u_char*)ptr;
		sum+=oddbyte;
	}

	//On bouge la somme de 16 bits vers la droite en gros divisé par 65536 (16 bits) et on lui ajoute et on ajoute le reste avec le low_order de 16 bits
	sum = (sum>>16)+(sum & 0xffff);
	//On redécalle le sum de 16bits 
	sum = sum + (sum>>16);
	//On fait le complément de la somme avec l'opérateur ~(bitwise NOT), autrement dit on donne l'inverse au niveau des bits 1=0 et 0=1;
	answer=(short)~sum;
	//On retourne le checksum
	return(answer);
}
/**
* \fn void makeTCP_segment(struct tcphdr *tcp_segment,uint16_t dest,uint32_t seq,uint32_t ack_seq,uint16_t fin,uint16_t syn,uint16_t ack,char *data,uint16_t psh)
* \brief Fonction de création de l'en tete TCP 
* \param La fonction recoit en parametre la structure du header, le port de destination, les flags TCP(sin, sin ack, ack), les données 
* \return La fonction ne retourne rien 
*/

void makeTCP_segment(struct tcphdr *tcp_segment,uint16_t dest,uint32_t seq,uint32_t ack_seq,uint16_t fin,uint16_t syn,uint16_t ack,char *data,uint16_t psh,struct iphdr *IP_header){
	//Header du segment TCP, TRAITEMENT à FAIRE le faire égale à un struct tcphdr avec la taille du datagram désiré et avec la taille de IP
	//On alloue l'emplacement memoire necessaire

	struct header_tcp_checksum tcp_cs;
	//Tableau de char pour la data que l'on souhaite
	char *fake_datagram;
	//La source du port aue l'on convertit en network byte order
	tcp_segment->source = htons(80);
	//Destination port
	tcp_segment->dest = htons(dest);
	tcp_segment->seq = seq;
	tcp_segment->ack_seq = ack_seq;
	//header size
	tcp_segment->doff = 5;
	//Flag TCP
	tcp_segment->fin = fin;
	tcp_segment->syn = syn;
	tcp_segment->rst = 0;
	tcp_segment->ack = ack;
	tcp_segment->psh = psh;
	tcp_segment->urg= 0;
	//Taille de la window, nombre d'octet que l'on souhaite recevoir 

	//On remplit le checksum plus tard
	tcp_segment->check = 0;
	tcp_segment->urg_ptr = 0;

	//Etant donné que l'on décalle de 16bits notre checksum nous devons calculer avec les adresses IP du header ip
	//On convertit notre adresse Ip source en adresse pour le réseau (donnée binaire)
	//BUG a régler ne prend pas normalement les adresses de broadcast 
	tcp_cs.address_source = inet_addr(inet_ntoa(getIp("eth0")));
	//adresse rentrée en brut ici PENSER à changer en passant l'adresse IP de destination enparamètre
	tcp_cs.address_destination = IP_header->saddr;
	//On calcule la taille du header plus la taille de la Data toujours de host-byte vers network-byte		
	tcp_cs.tcp_length = htons(sizeof(struct tcphdr) + strlen(data));
	tcp_cs.placeholder = 0;
	//Constante de numéro de protocole ici égal à 6
	tcp_cs.protocol = IPPROTO_IP;
	
	//On donne la taille du datagram complet avec tout les éléments nécessaire
	int size_datagram_cs = sizeof(struct header_tcp_checksum) + sizeof(struct tcphdr) + strlen(data);
	//on allloue la mémoire nécessaire à notre datagramme, afin de pouvoir réaliser un memcpy à cet emplacement mémoire
	fake_datagram = malloc(size_datagram_cs);
	//On copie notre header ip partiel au début de notre datagramme fake pour calculer le checksum
	memcpy(fake_datagram,(char*) &tcp_cs,sizeof(struct header_tcp_checksum));
	//On rajoute le header TCP à notre pseudo header IP pour avoir un header complet, + l'espace pour la data
	memcpy(fake_datagram + sizeof(struct header_tcp_checksum),tcp_segment,sizeof(struct tcphdr) + strlen(data));
	//On calcul notre checksum TCP
	tcp_segment->check = checksum((unsigned short*) fake_datagram, size_datagram_cs);

}
