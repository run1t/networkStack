#include "PC.h"
string PC::getIP(){
	int sockfd;
	struct ifreq ifr;
	unsigned char *macDst;
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name,"eth0",IFNAMSIZ-1);
	sockfd = socket(AF_INET,SOCK_STREAM,0);

	if(sockfd == -1){
		perror("Error socket opening MAC Address");
	}
	ioctl(sockfd,SIOCGIFHWADDR,&ifr);
	close(sockfd);
	
	/******/
	return (char *)ifr.ifr_hwaddr.sa_data;	

}

string PC::getMAC(){

}