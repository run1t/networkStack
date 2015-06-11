/*!
 * \file stack.cpp
 * \brief Fichier de gestion de l'envoie et reception de packet
 * \author LE NOC Reunan, HIPEAU Kevin, VIAUD Thommas, TRICHARD Guillaume
 * \version 1.0
 */

#include "Stack.h"

/**
* On va mettre dans ce fichier toutes les mecanique de 
* la stack
*/

//Permet de gérer les signaux
void handle_SIGINT(int signal);

/**
 * \fn Stack::Stack(string ip,int port)
 * \brief Fonction de gestion du socket
 *
 * \param la fonction prend en parametre l'ip et le port 
 * \return La fonction retourne la stack
 */
Stack::Stack(string ip,int port){
	this->port = port;
	/* on mets en place le socket */
	int sockfd;
	int sockopt;

	//Desactivation 
	PC::desactivateICMP();
	PC::desactivateRST();


	struct sigaction siga;

	siga.sa_handler = &handle_SIGINT;
	siga.sa_flags = SA_RESTART;
	//On prend en  compte tout les signaux
	sigfillset(&siga.sa_mask);
	if(sigaction(SIGINT,&siga,NULL) == -1){
		printf("Error handling SIGINT\n");
	}
	/* */
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

	this->sock = sockfd;
};


/**
 * \fn void Stack::receiver()
 * \brief Fonction de gestion de réception de packet IP et ICMP, elle permet d'écouter, trier et répondre correctement
 *
 * \param la fonction ne prend aucun parametre
 * \return La fonction ne retoure rien
 */
void Stack::receiver(){
	uint8_t buf[1024];
	cout << PC::getIP() << endl;
	cout << PC::getMAC() << endl;
	while(1){
			int	numbytes = recvfrom(sock, buf, 1024, 0, NULL, NULL);
			//On verifie que l'on a bien des données


			if(numbytes > 0){
				ETHFrame eth = *new ETHFrame(buf);
				//c'est pour nous ?

				if(eth.dst.compare(PC::getMAC()) == 0){

					if(eth.Type == ETHERTYPE_IPv4){
						
						IPFrame ip = *new IPFrame(buf);

						if(ip.dst.compare(PC::getIP()) == 0){
							
							//on check lsi c'est pour
							if(ip.Protocol == 6){
								TCPFrame tcp = *new TCPFrame(buf);
								TCPFrame tcpRes = tcp;

								if(this->port == tcp.dst){
									/** on recupere la connection associer a notre propre **/
									if(this->getConnection(tcp.src)->port < 0){
										this->addConnection(new Connection(tcp.src));
									}else{
										this->getConnection(tcp.src)->HandleConnection(tcp);
									}

								}
							}
							//On a de l'ICMP
							else if(ip.Protocol == 1){
									ICMPFrame icmp = *new ICMPFrame(buf);
									ICMPFrame icmpRes = icmp;
									icmpRes.eth.src = icmp.eth.dst;
									icmpRes.eth.dst = icmp.eth.src;

									icmpRes.ip.src =  icmp.ip.dst;
									icmpRes.ip.dst =  icmp.ip.src;
									if(icmp.Type == 8){
										icmpRes.Type = 0;
										this->Send(icmpRes);
									}
									
								}
						}
					}
				}else{
					if(eth.Type == 0x0806){
						ARPFrame arp = *new ARPFrame(buf);
						//On prend que ethernet et ipv4
						if(arp.HardwareSize == 6 && arp.ProtocolSize == 4 && arp.opCode == 1){
							
							if(arp.targetIp.compare(PC::getIP()) == 0){
								ARPFrame arpRes = arp;
								arpRes.eth.dst = arp.eth.src;
								arpRes.eth.src = PC::getMAC();
								arpRes.opCode = 2;
								arpRes.senderMac = PC::getMAC();
								arpRes.senderIp = arp.targetIp;
								arpRes.targetMac = arp.senderMac;
								arpRes.targetIp = arp.senderIp;
								this->Send(arpRes);
							}	
						}
						
					}
				}
			}
		}
}


/**
 * \fn void Stack::Send(TCPFrame tcp)
 * \brief Fonction d'envoie de trame TCP
 *
 * \param la fonction prend en parametre le treame TCP
 * \return La fonction ne retoure rien
 */
void Stack::Send(TCPFrame tcp){
	#define DEFAULT_IF	"eth0"
	
	
	int sockfd;
	struct ifreq if_idx;
	struct sockaddr_ll socket_address;
	char ifName[IFNAMSIZ];
	
	
	strcpy(ifName, DEFAULT_IF);
 
	/* Open RAW socket to send on */
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
	    perror("socket");
	}
 
	/* Get the index of the interface to send on */
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");

	unsigned char* datagram = tcp.toFrame();


	/* Index of the network device */
	socket_address.sll_ifindex = if_idx.ifr_ifindex;
	/* Address length*/
	socket_address.sll_halen = ETH_ALEN;
	/* Destination MAC */
	socket_address.sll_addr[0] = datagram[0];
	socket_address.sll_addr[1] = datagram[1];
	socket_address.sll_addr[2] = datagram[2];
	socket_address.sll_addr[3] = datagram[3];
	socket_address.sll_addr[4] = datagram[4];
	socket_address.sll_addr[5] = datagram[5];
 
	/* Send packet */
	sendto(sockfd, datagram, tcp.frameLength, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll));
	  
	close(sockfd);
}
/**
 * \fn void Stack::Send(ICMPFrame icmp)
 * \brief Fonction d'envoie de trame ICMP
 *
 * \param la fonction prend en parametre le trame ICMP
 * \return La fonction ne retoure rien
 */
void Stack::Send(ICMPFrame icmp){
	#define DEFAULT_IF	"eth0"
	int sockfd;
	struct ifreq if_idx;
	struct sockaddr_ll socket_address;
	char ifName[IFNAMSIZ];
	strcpy(ifName, DEFAULT_IF);
 
	/* Open RAW socket to send on */
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
	    perror("socket");
	}
 
	/* Get the index of the interface to send on */
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");

	/* Construct the Ethernet header */
	unsigned char* datagram = icmp.toFrame();
 
	/* Index of the network device */
	socket_address.sll_ifindex = if_idx.ifr_ifindex;
	/* Address length*/
	socket_address.sll_halen = ETH_ALEN;
	/* Destination MAC */
	socket_address.sll_addr[0] = datagram[0];
	socket_address.sll_addr[1] = datagram[1];
	socket_address.sll_addr[2] = datagram[2];
	socket_address.sll_addr[3] = datagram[3];
	socket_address.sll_addr[4] = datagram[4];
	socket_address.sll_addr[5] = datagram[5];
 
	/* Send packet */
	sendto(sockfd, datagram, icmp.frameLength, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll));
	  

	close(sockfd);
}
/**
 * \fn void Stack::Send(ARPFrame arp)
 * \brief Fonction d'envoie de trame ARP
 *
 * \param la fonction prend en parametre le trame ARP
 * \return La fonction ne retoure rien
 */
void Stack::Send(ARPFrame arp){
	#define DEFAULT_IF	"eth0"
	
	int sockfd;
	struct ifreq if_idx;
	struct sockaddr_ll socket_address;
	char ifName[IFNAMSIZ];
	
	strcpy(ifName, DEFAULT_IF);
 
	/* Open RAW socket to send on */
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
	    perror("socket");
	}
 
	/* Get the index of the interface to send on */
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");
	
	unsigned char* datagram = arp.toFrame();

	/* Index of the network device */
	socket_address.sll_ifindex = if_idx.ifr_ifindex;
	/* Address length*/
	socket_address.sll_halen = ETH_ALEN;
	/* Destination MAC */
	socket_address.sll_addr[0] = datagram[0];
	socket_address.sll_addr[1] = datagram[1];
	socket_address.sll_addr[2] = datagram[2];
	socket_address.sll_addr[3] = datagram[3];
	socket_address.sll_addr[4] = datagram[4];
	socket_address.sll_addr[5] = datagram[5];
 
	/* Send packet */
	sendto(sockfd, datagram, arp.frameLength, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll));

	close(sockfd);	
	
}


void Stack::addSynEvent (function<void()> func){
	this->onSyn = func;
}

void Stack::addDataEvent(function<void(string)> func){
	this->onData = func;
}

void Stack::addFinEvent (function<void()> func){
	this->onFin = func;
}


Connection* Stack::getConnection(int port){
	for(size_t i = 0; i < this->Connections.size() ; i++){
		if(this->Connections.at(i)->port == port)
			return this->Connections.at(i);
	}
	return new Connection(-200);
}

void Stack::addConnection(Connection *connection){
	Connection::ConnectionNumber++;
	cout << "Nombre de connection : " << Connection::ConnectionNumber << endl;
	this->Connections.push_back(connection);
}

//Gestion de la fermeture du programme
void handle_SIGINT(int signal){
	if(signal == SIGINT){
		PC::activateRST();
		PC::activateICMP();
		printf("Terminating the TCP/IP Stack\n");
		exit(0);
	}
}

