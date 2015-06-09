/*!
 * \file PC.cpp
 * \brief Fichier de récupération des informations du pc 
 * \author LE NOC Reunan, HIPEAU Kevin, VIAUD Thommas, TRICHARD Guillaume
 * \version 1.0
 */

#include "PC.h"

/**
 * \fn string PC::getIP()
 * \brief Fonction de récupération de l'adresse ip de destination 
 *
 * \param la fonction ne nécessite aucun paramètres
 * \return La fonction retourne l'adresse IP 
 */
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

/**
 * \fn string PC::getMAC()
 * \brief Fonction de récupération de l'adresse MAC de la machine  
 *
 * \param la fonction ne nécessite aucun paramètres
 * \return La fonction retourne l'adresse mac de la machine 
 */
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

/**
 * \fn string PC::getMAC()
 * \brief Fonction de récupération de l'interface réseau 
 *
 * \param la fonction ne nécessite aucun paramètres
 * \return La fonction retourne l'interface réseau de la machine
 */
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

int PC::desactivateRST(){
	int result;
	string command = "iptables -A OUTPUT -o " + PC::getDefaultInterface() + " -p tcp --tcp-flags RST RST -j DROP";
	result = system(command.c_str());
	//retourne -1 si erreur
	return result;
}

int PC::activateRST(){
	int result;
	string command = "iptables -A OUTPUT -o " + PC::getDefaultInterface() + " -p tcp --tcp-flags RST RST -j ACCEPT";
	result = system(command.c_str());
	//retourne -1 si erreur
	return result;
}

int PC::desactivateICMP(){
	int fdProc = open("/proc/sys/net/ipv4/icmp_echo_ignore_all",O_WRONLY);
		if(fdProc == -1){
			perror("Error opening the file /proc/sys/net/ipv4/icmp_echo_ignore_all\n");
			return -1;
		}
		int nbByteWrite = write(fdProc,"1",1);
		if(nbByteWrite == 0){
			printf("No write\n");
			return -1;
		}
		else if(nbByteWrite == -1){
			perror("Error writing to file\n");
			return -1;
		}
		else if(nbByteWrite > 0){
			printf("Succes writing to file\n");
			int closeSuccess = close(fdProc);
			if(closeSuccess == 0){
				printf("File closed successfully\n");
				return 0;
			}else if(closeSuccess == -1){
				printf("Error closing the file\n");
				return -1;
			}
		}
		return 0;
}

int PC::activateICMP(){
	int fdProc = open("/proc/sys/net/ipv4/icmp_echo_ignore_all",O_WRONLY);
		if(fdProc == -1){
			perror("Error opening the file /proc/sys/net/ipv4/icmp_echo_ignore_all\n");
			return -1;
		}
		int nbByteWrite = write(fdProc,"0",1);
		if(nbByteWrite == 0){
			printf("No write\n");
			return -1;
		}
		else if(nbByteWrite == -1){
			perror("Error writing to file\n");
			return -1;
		}
		else if(nbByteWrite > 0){
			printf("Succes writing to file\n");
			int closeSuccess = close(fdProc);
			if(closeSuccess == 0){
				printf("File closed successfully\n");
				return 0;
			}else if(closeSuccess == -1){
				printf("Error closing the file\n");
				return -1;
			}
		}
		return 0;
}
