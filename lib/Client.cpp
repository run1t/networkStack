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

/**
 * \fn Client::Client(string ip,int port)
 * \brief Constructeur de Client 
 * \param string ip adresse IP
 * \param int port numéro de port
 */
Client::Client(string ip,int port){
	srand (time(NULL));
	this->portLocal = (ushort)rand();
	this->ip = ip;
	this->port = port;
	this->stacker =  new Stack(ip,this->portLocal);
    this->stacker->addDataEvent(onDatass);
}

/**
 * \fn void Client::Send(string Message)
 * \brief Méthode afin d'envoyer un message
 * \param string Message
 */
void Client::Send(string Message){
	
}

/**
 * \fn void Client::addEventData (function<void(Connection*)> func)
 * \brief Méthode permettant de binder une fonction à l'event de réception de Data
 * \param function<void(Connection*)> func Fonction à passer en paramètre
 */
void Client::addEventData (function<void(Connection*)> func){
	this->onData = func;
}

/**
 * \fn string Client::getArpMac()
 * \brief Méthode permettant d'avoir l'adresse MAC du destinataire grâce à une requète ARP
 * \return string Nous renvoie l'adresse MAC sous forme de string
 */
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

	/* On ouvre l'ecoute des packet Ethernet */
 	if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) == -1) {
 		perror("listener: socket");	
 	}
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

 				if(eth.Type == 0x0806){
 					ARPFrame arp = *new ARPFrame(buf);
					if(arp.HardwareSize == 6 && arp.ProtocolSize == 4 && arp.opCode == 2){
 						if(arp.senderIp.compare(this->ip) == 0){
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

/**
 * \fn void Client::join()
 * \brief Méthode permettant de lancer la stack client
 */
void Client::join(){
	Client::client = this;
	TCPFrame tcp = *new TCPFrame();
	tcp.Flags = TCP_SYN;
	string arpMac = this->getArpMac();
	tcp.eth.dst = arpMac;
	tcp.ip.dst = this->ip;
	tcp.ip.src = PC::getIP();
	tcp.dst = this->portLocal;
	tcp.src = this->port;
	this->stacker->Send(tcp);
	this->stacker->receiver();
}
