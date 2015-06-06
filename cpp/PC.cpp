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
			printf("%s",buffEth0[i]);
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
			printf("%s",buffWlan0[i]);
			i++;
		}
		int closeSuccess = close(fdProc1);
		if(closeSuccess == - 1){
			printf("error closing the file");
		}
	}
	if(buffEth0 == "up"){
		return "eth0";
	}else if(buffWlan0 == "up"){
		return "wlan0";
	}
}
