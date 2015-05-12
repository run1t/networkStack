#include "socket.h"
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
	strncpy(ifr.ifr_name,"en0",IFNAMSIZ-1);

	
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

struct tcphdr makeTCP_segment(){
	struct tcphdr tcp_segment;
	//La source du port aue l'on convertit en network byte order
	tcp_segment.source = htons(5000);
	//Destination port
	tcp_segment.dest = htons(2000);
	tcp_segment.seq = 0;
	tcp_segment.ack_seq = 0;
	//header size
	tcp_segment.doff = 5;
	//Flag TCP
	tcp_segment.fin = 0;
	tcp_segment.syn = 1;

	return tcp_segment;
}
