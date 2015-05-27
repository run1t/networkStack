#include "tcp.h"
#include "ethernet.h"

unsigned char * getMac_address(char* interface)
{
	int sockfd;
	//Interface request pour avoir l'adresse Mac
	struct ifreq ifr;

	//Variablequi va contenir notre adresse MAC
	unsigned char *macAddress;

	ifr.ifr_addr.sa_family = AF_INET;
	//On copie dans ifr.ifr_name le nom de l'interface à requéter taille spécifié dans IFNAMSIZ
	strncpy(ifr.ifr_name,interface,IFNAMSIZ-1);

	//On créée le socket
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1){
		perror("Error socket opening MAC Address");
	}
	//On demande au kernel l'adresse MAC de l'interface désiré. Il va répondre dans l'interface fournie
	ioctl(sockfd,SIOCGIFHWADDR,&ifr);
	//On ferme le socket
	close(sockfd);
	
	macAddress = (unsigned char *)ifr.ifr_hwaddr.sa_data;

	//Mis pour raison de test A ENLEVER
	printf("MAC : %.2x %.2x %.2X %.2X %.2X %.2X\n",macAddress[0],macAddress[1],macAddress[2],macAddress[3],macAddress[4],macAddress[5]);
	return macAddress;
}