#include "Stack.h"

/**
* On va mettre dans se fichier toutes les mecanique de 
* la stack
*/

Stack::Stack(string ip,int port){
	/* on mets en place le socket */
	int sockfd;
	int sockopt;


	/* */
	struct ifreq ifopts;	
	struct ifreq if_ip;
	struct sockaddr_ll sa;
	
	/* On configure le nom de notre interface */
	char ifName[IFNAMSIZ];
	strcpy(ifName, DEFAULT_IF);
	memset(&if_ip, 0, sizeof(struct ifreq));
 
	/* On ouver l'ecoute des packet Ethernet */
	if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETHER_TYPE))) == -1) {
		perror("listener: socket");	
	}
 
	/* On met l'interface en promiscuis mode ce qui 
	permet d"couter tout les connections escequue 
	c'est vraiment necessaire ?*/

	strncpy(ifopts.ifr_name, ifName, IFNAMSIZ-1);
	ioctl(sockfd, SIOCGIFFLAGS, &ifopts);
	ifopts.ifr_flags |= IFF_PROMISC;
	ioctl(sockfd, SIOCSIFFLAGS, &ifopts);


	/* Gere un cas de fermeture prematurer*/
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) == -1) {
		perror("setsockopt");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	/* On "Bind" notre socket avec notre device */
	if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, ifName, IFNAMSIZ-1) == -1)	{
		perror("SO_BINDTODEVICE");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	memset(&sa, 0, sizeof (sa));
	sa.sll_family = AF_PACKET;
	sa.sll_ifindex = if_ip.ifr_ifindex;
	sa.sll_protocol = htons(ETH_P_ALL);

	this->sock = sockfd;
};

void Stack::receiver(){
	uint8_t buf[1024];
	int	numbytes = recvfrom(sock, buf, 1024, 0, NULL, NULL);
	
	//On verifie que l'on a bien des données
	if(numbytes > 0){
		ETHFrame eth = *new ETHFrame(buf,numbytes);
		if(eth.Type == ETHERTYPE_IPv4){
			IPFrame ip = *new IPFrame(buf,numbytes);
			cout << "ip Version : "  << ip.Version << endl;
			cout << "Header length : "  << ip.HeaderLength << endl;
			cout << "Total length : " << ip.TotalLength << endl;
			cout << "Id :" << ip.Id << endl;
			cout << "Reserved :" << ip.Flag_Reserved << endl;
			cout << "DF :" << ip.Flag_DF << endl;
			cout << "MF :" << ip.Flag_MF << endl;
			cout << "TTL :" << ip.TTL << endl;
			cout << "Protocol :" << ip.Protocol << endl;
			cout << "Checksum :" << ip.Checksum << endl;
			cout << "Ip source :" << ip.src << endl;
			cout << "Ip Destination :" << ip.dst << endl;
			//cout << "Position Fragment" << ip.PositionFragment << endl;
		}
	}else{
		cout << "on a pas des données" << endl;
	}
}
