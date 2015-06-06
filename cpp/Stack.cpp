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
	cout << PC::getIP() << endl;
	cout << PC::getMAC() << endl;
	while(1){
			int	numbytes = recvfrom(sock, buf, 1024, 0, NULL, NULL);
			//On verifie que l'on a bien des données
			if(numbytes > 0){
				ETHFrame eth = *new ETHFrame(buf,numbytes);
				if(eth.Type == ETHERTYPE_IPv4){
					IPFrame ip = *new IPFrame(buf,numbytes);
				
					if(ip.Protocol == 6){
						TCPFrame tcp = *new TCPFrame(buf,numbytes);
						cout << "IP id :" << tcp.ip.Id << endl;
						cout << "port source : " << tcp.src << endl;
						cout << "port Destination : " << tcp.dst << endl;
						cout << "seq number : " << tcp.seq_number << endl;
						cout << "ack number : " << tcp.ack_number << endl;
						cout << "Flags number : " << tcp.Flags << endl;
						cout << "Windows Size :" <<tcp.Windows << endl;
						cout << "Checksum : " << tcp.Checksum << endl;
						cout << "Pointer : " << tcp.urgentPointer << endl;
						cout << "HeaderLength :" << tcp.HeaderLength << endl;
						cout << "Data : " << tcp.data << endl;
					
						
					}
				}
			}else{
				cout << "on a pas des données" << endl;
			}
		}
}
