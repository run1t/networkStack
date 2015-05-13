#include "tcp.h"
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
	printf("%d\n",sockfd);
	
	//On demande au kernel l'adresse IP, et il nous remplit la structure ifr
	ioctl(sockfd,SIOCGIFADDR,&ifr);

	//inet_ntoa sert à transformer une addresse en byte en texte readable, on lui passe l'addresse de notre adresse ipV4, mais on cast par sockaddr_in pour avoir accès à sin_addr
	return ((struct sockaddr_in * )&ifr.ifr_addr)->sin_addr;
}

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

struct tcphdr makeTCP_segment(uint16_t dest,uint32_t seq,uint32_t ack_seq,uint16_t fin,uint16_t syn,uint16_t ack){
	struct tcphdr tcp_segment;
	//La source du port aue l'on convertit en network byte order
	tcp_segment.source = htons(1337);
	//Destination port
	tcp_segment.dest = htons(dest);
	tcp_segment.seq = seq;
	tcp_segment.ack_seq = ack_seq;
	//header size
	tcp_segment.doff = 5;
	//Flag TCP
	tcp_segment.fin = fin;
	tcp_segment.syn = syn;
	tcp_segment.rst = 0;
	tcp_segment.psh = 0;
	tcp_segment.ack = ack;
	tcp_segment.urg= 0;
	//Taille de la window, nombre d'octet que l'on souhaite recevoir 
	tcp_segment.window = htons(4000);
	//On remplit le checksum plus tard
	tcp_segment.check = 0;
	tcp_segment.urg_ptr = 0;
	
	return tcp_segment;
}
