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
				//c'est pour nous ?
				if(eth.dst.compare(PC::getMAC()) == 0){

					if(eth.Type == ETHERTYPE_IPv4){
						
						IPFrame ip = *new IPFrame(buf,numbytes);

						if(ip.dst.compare(PC::getIP()) == 0){
							
							//on check lsi c'est pour
							if(ip.Protocol == 6){
								TCPFrame tcp = *new TCPFrame(buf,numbytes);
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
										this->Sender(tcpRes);

									}else if(tcp.Flags == (TCP_PSH | TCP_ACK)){
										
										tcpRes.Flags = TCP_ACK | TCP_PSH;
										tcpRes.seq_number = tcp.ack_number;
										tcpRes.ack_number = tcp.seq_number + tcp.data.length();
										tcpRes.data =  "HTTP/1.x 200 OK\n"
													    "Transfer-Encoding: chunked\n"
													    "Date: Sat, 28 Nov 2009 04:36:25 GMT\n"
														"Server: LiteSpeed\n"
														"Connection: close\n"
														"X-Powered-By: W3 Total Cache/0.8\n"
														"Pragma: public\n"
														"Expires: Sat, 28 Nov 2009 05:36:25 GMT\n"
														"Etag: \"pub1259380237;gz\"\n"
														"Cache-Control: max-age=3600, public\n"
														"Content-Type: text/html; charset=UTF-8\n"
														"Last-Modified: Sat, 28 Nov 2009 03:50:37 GMT\n"
														"X-Pingback: http://net.tutsplus.com/xmlrpc.php\n"
														"Content-Encoding: gzip\n"
														"Vary: Accept-Encoding, Cookie, User-Agent\n"
														 
														"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
														"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
														"<head>\n"
														"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n"
														"<title>Top 20+ MySQL Best Practices - Nettuts+</title>;\n";
										this->Sender(tcpRes);
										

									}else if(tcp.Flags == TCP_ACK){
										cout << "on a un ACK" << endl;
									}else if(tcp.Flags == (TCP_ACK | TCP_FIN)){
										tcpRes.Flags = TCP_ACK | TCP_FIN;
										tcpRes.seq_number = tcp.ack_number;
										tcpRes.ack_number = tcp.seq_number + 1;
										tcpRes.data = "";
										this->Sender(tcpRes);
									}

								
								}
								/*cout << "IP id :" << tcp.ip.Id << endl;
								cout << "port source : " << tcp.src << endl;
								cout << "port Destination : " << tcp.dst << endl;
								cout << "seq number : " << tcp.seq_number << endl;
								cout << "ack number : " << tcp.ack_number << endl;
								cout << "Flags number : " << tcp.Flags << endl;
								cout << "Windows Size :" <<tcp.Windows << endl;
								cout << "Checksum : " << tcp.Checksum << endl;
								cout << "Pointer : " << tcp.urgentPointer << endl;
								cout << "HeaderLength :" << tcp.HeaderLength << endl;
								cout << "Data : " << tcp.data << endl;*/

							}
							//On a de l'ICMP
							else if(ip.Protocol == 1){
									ICMPFrame icmp = *new ICMPFrame(buf,numbytes);
									cout << "Type ICMP : " << icmp.Type << endl;
									cout << "Type Code : " << icmp.Code << endl;
									cout << "Type Checksum : " <<  icmp.Checksum << endl;
									cout << "Type ID : " <<  icmp.Id << endl;
									cout << "Type seq : " << icmp.sequence << endl;
								}
						}
					}
				}
			}
				//on check si c'est de l'ipv4
		}
}

void Stack::Sender(TCPFrame tcp){
	#define DEFAULT_IF	"eth0"
	#define BUF_SIZ		1024
	#define MY_DEST_MAC0	0x12
	#define MY_DEST_MAC1	0x33
	#define MY_DEST_MAC2	0x44
	#define MY_DEST_MAC3	0x55
	#define MY_DEST_MAC4	0x34
	#define MY_DEST_MAC5	0x65
	
	int sockfd;
	struct ifreq if_idx;
	struct ifreq if_mac;
	int tx_len = 0;
	char sendbuf[BUF_SIZ];
	struct ether_header *eh = (struct ether_header *) sendbuf;
	struct iphdr *iph = (struct iphdr *) (sendbuf + sizeof(struct ether_header));
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
	/* Get the MAC address of the interface to send on */
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
	    perror("SIOCGIFHWADDR");
 
	/* Construct the Ethernet header */
	memset(sendbuf, 0, BUF_SIZ);
	unsigned char* datagram = tcp.toFrame();

	/* Ethertype field */
	eh->ether_type = htons(ETH_P_IP);
	tx_len += sizeof(struct ether_header);
 
	/* Packet data */
	
	memcpy(&sendbuf[tx_len],datagram,tcp.frameLength);
 
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
