#include "Stack.h"

/**
* On va mettre dans se fichier toutes les mecanique de 
* la stack
*/

Stack::Stack(string ip,int port){
	this->port = port;
	/* on mets en place le socket */
	int sockfd;
	int sockopt;


	int fdProc = open("/proc/sys/net/ipv4/icmp_echo_ignore_all",O_WRONLY);
		if(fdProc == -1){
			perror("Error opening the file /proc/sys/net/ipv4/icmp_echo_ignore_all\n");
		}
		int nbByteWrite = write(fdProc,"1",1);
		if(nbByteWrite == 0){
			printf("No write\n");
		}
		else if(nbByteWrite == -1){
			perror("Error writing to file\n");
		}
		else if(nbByteWrite > 0){
			printf("Succes writing to file\n");
			int closeSuccess = close(fdProc);
			if(closeSuccess == 0){
				printf("File closed successfully\n");
			}else if(closeSuccess == -1){
				printf("Error closing the file\n");
			}
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
									//On va repondre a notre SYN
									tcpRes.eth.src = tcp.eth.dst;
									tcpRes.eth.dst = tcp.eth.src;

									tcpRes.ip.src =  tcp.ip.dst;
									tcpRes.ip.dst =  tcp.ip.src;	
									
									if(tcp.Flags == TCP_SYN ){
										
										tcpRes.Flags = TCP_SYN | TCP_ACK;
										tcpRes.seq_number = tcp.ack_number;
										tcpRes.ack_number = tcp.seq_number + 1;
										this->Send(tcpRes);

									}else if(tcp.Flags == (TCP_PSH | TCP_ACK)){
										
										tcpRes.Flags = TCP_ACK | TCP_PSH;
										tcpRes.seq_number = tcp.ack_number;
										tcpRes.ack_number = tcp.seq_number + tcp.data.length();
										tcpRes.data =   "HTTP/1.1 200 OK\n"
														"Content-Type: text/html;charset=utf-8\n"
														"\n"
														"<html></html>\n"
														"<body><div style=\"color:red;\">Un div stylé</div>\n";
											
										this->Send(tcpRes);

									

										tcpRes.Flags = TCP_ACK | TCP_FIN;
										tcpRes.seq_number = tcp.ack_number;
										tcpRes.ack_number = tcp.seq_number + tcp.data.length();
										this->Send(tcpRes);
										

									}else if(tcp.Flags == TCP_ACK){
										cout << "on a un ACK" << endl;
									}else if(tcp.Flags == (TCP_ACK | TCP_FIN)){
										tcpRes.Flags = TCP_ACK | TCP_FIN;
										tcpRes.seq_number = tcp.ack_number;
										tcpRes.ack_number = tcp.seq_number + 1;
										tcpRes.data = "";
										this->Send(tcpRes);
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
							cout << arp.targetIp << "  :  " << PC::getIP() << endl;
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
	if (sendto(sockfd, datagram, tcp.frameLength, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0){
	   	cout << "Error" << endl;
	}else{
		cout << "Success" << endl;
	}

	close(sockfd);
	
	
}

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
	if (sendto(sockfd, datagram, icmp.frameLength, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0){
	  	cout << "Error" << endl;
	}else{
		cout << "Success" << endl;
	}

	close(sockfd);
}

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
	if (sendto(sockfd, datagram, arp.frameLength, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0){
	   	cout << "Error" << endl;
	}else{
		cout << "Success" << endl;
	}

	close(sockfd);	
	
}
