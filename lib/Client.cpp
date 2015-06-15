/*!
 * \file Client.cpp
 * \brief Fichier de gestion de la couche client
 * \author LE NOC Reunan, HIPEAU Kevin, VIAUD Thommas, TRICHARD Guillaume
 * \version 1.0
 */
#include "Client.h"
Client* Client::client;
void onDatass(Connection* connection){
	Client::client->onData(connection);
}


Client::Client(string ip,int port){
	srand (time(NULL));
	this->portLocal = (ushort)rand();
	this->ip = ip;
	this->port = port;
	this->stacker =  new Stack(ip,this->portLocal);
    this->stacker->addDataEvent(onDatass);
}

void Client::Send(string Message){
	
}

void Client::addEventData (function<void(Connection*)> func){
	this->onData = func;
}

string Client::getArpMac(){
	PC::desactivateICMP();
 	PC::desactivateRST();
	PC::desactivateARP();
	ARPFrame arp = *new ARPFrame();
	arp.targetIp = this->ip;

	arp.Send();
	int sockfd;
 	int sockopt;
	struct ifreq ifopts;	
 	struct ifreq if_ip;
 	struct sockaddr_ll sa;

	/* On configure le nom de notre interface */
 	char ifName[IFNAMSIZ];
 	strcpy(ifName, DEFAULT_IF);
 	memset(&if_ip, 0, sizeof(struct ifreq));

	/* On ouver l'ecoute des packet Ethernet */
 	if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) == -1) {
 		perror("listener: socket");	
 	}

	/* On met l'interface en promiscuis mode ce qui 
	permet d"couter tout les connections escequue 
	c'est vraiment necessaire ?*/

	strncpy(ifopts.ifr_name, ifName, IFNAMSIZ-1);
	ioctl(sockfd, SIOCGIFFLAGS, &ifopts);
	ifopts.ifr_flags |= IFF_PROMISC;
	ioctl(sockfd, SIOCSIFFLAGS, &ifopts);


	/* Gere un cas de fermeture prematuré*/
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

	uint8_t buf[1024];
 
 	while(1){
 		int	numbytes = recvfrom(sockfd, buf, 1024, 0, NULL, NULL);
			//On verifie que l'on a bien des données

 		if(numbytes > 0){

 			ETHFrame eth = *new ETHFrame(buf);
				//c'est pour nous ?

 				if(eth.Type == 0x0806){

 					ARPFrame arp = *new ARPFrame(buf);
 						cout << "test" << arp.senderIp << endl;
					if(arp.HardwareSize == 6 && arp.ProtocolSize == 4 && arp.opCode == 2){
						cout << "test" << arp.senderIp << endl;
 						if(arp.senderIp.compare(this->ip) == 0){
 							cout << "okayy" << endl;
 							PC::activateICMP();
						 	PC::activateRST();
							PC::activateARP();
							close(sockfd);
 							return arp.senderMac;
 						}	
 					}
 				}
 			
 		}
 	}	
 	close(sockfd);
	return "FF:FF:FF:FF:FF:FF";
}

void Client::join(){
	Client::client = this;
	TCPFrame tcp = *new TCPFrame();
	tcp.Flags = TCP_SYN;
	string arpMac = this->getArpMac();
	cout << "MAc arp" << arpMac << endl;
	tcp.eth.dst = arpMac;
	cout << tcp.eth.dst << endl;
	tcp.ip.dst = this->ip;
	tcp.ip.src = PC::getIP();
	tcp.dst = this->portLocal;
	tcp.src = this->port;
	this->stacker->Send(tcp);
	this->stacker->receiver();
}
