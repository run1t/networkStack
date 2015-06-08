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
  	struct ifreq s;
  	string mac = "";
  	int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	strcpy(s.ifr_name, "eth0");
  	if (fd >= 0  && 0 == ioctl(fd, SIOCGIFHWADDR, &s))
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


string PC::getDefaultInterface(){
	//Buffer contenant le texte de sortie
	char buffEth0[20];
	char buffWlan0[20];

	int fdProc = open("/sys/class/net/eth0/operstate",O_RDONLY);
	if(fdProc == -1){
		perror("Failed opening file operstate eth0");
	}
	int nbBytesRead = read(fdProc,buffEth0,20);
	if(nbBytesRead == -1){
		perror("failed Reading");	
	}else{
		int i = 0;
		while(buffEth0[i] != '\n'){
			i++;
		}
		int closeSuccess = close(fdProc);
		if(closeSuccess == - 1){
			printf("error closing the file");
		}
	}
	int fdProc1 = open("/sys/class/net/eth0/operstate",O_RDONLY);
	if(fdProc1 == -1){
		perror("Failed opening file operstate eth0");
	}
	int nbBytesRead1 = read(fdProc1,buffWlan0,20);
	if(nbBytesRead1 == -1){
		perror("failed Reading");	
	}else{
		int i = 0;
		while(buffWlan0[i] != '\n'){
			i++;
		}
		int closeSuccess = close(fdProc1);
		if(closeSuccess == - 1){
			printf("error closing the file");
		}
	}
	if(!string(buffEth0).compare("up")){
		return "eth0";
	}else if(!string(buffWlan0).compare("up")){
		return "wlan0";
	}else{
		return "none";
	}
	
}

