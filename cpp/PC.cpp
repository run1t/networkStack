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
	/**
	* On recupère l'adresse Mac de la machine
	*/ 

   	#define MAC_STRING_LENGTH 13
  	char *ret ;
  	struct ifreq s;
  	string mac = "";
  	int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

  	strcpy(s.ifr_name, "eth0");
  	if (fd >= 0 && ret && 0 == ioctl(fd, SIOCGIFHWADDR, &s))
  	{
    	int i;
    	for (i = 0; i < 6; ++i){
    		stringstream stream;
			stream << hex << uppercase << (int ) s.ifr_addr.sa_data[i];
			string result( stream.str() );
			if(result.length() == 1){
				result = "0" + result;
			}
			if( i < 5){
				mac += result + ":";
			}else{
				mac += result;
			}
      		
    	}
  	}
  	else
  	{
   		perror("malloc/socket/ioctl failed");
  	}
  	return string(mac);
}