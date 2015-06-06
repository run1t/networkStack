#include "PC.h"
string PC::getIP(){
	int fd;
 	struct ifreq ifr;
 	fd = socket(AF_INET, SOCK_DGRAM, 0);
 	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
	ioctl(fd, SIOCGIFADDR, &ifr);
	close(fd);

	/**
	* Recuperation de l'ip destination
	*/ 
	return inet_ntoa( ( (struct sockaddr_in *) &ifr.ifr_addr) ->sin_addr) ;
}

string PC::getMAC(){

}