/*!
 * \file Connection.cpp
 * \brief Gère les connections
 * \author LE NOC Reunan, HIPEAU Kevin, VIAUD Thommas, TRICHARD Guillaume
 * \version 1.0
 */
#include "Connection.h"
int Connection::ConnectionNumber  = 0;

/**
 * \fn Connection::Connection(int port)
 * \brief Constructeur d'une connection
 *
 * \param port Prend le port en paramètre
 */
Connection::Connection(int port){
	this->port = port;
	this->State = CLOSED;
}

/**
 * \fn void Connection::HandleConnection(TCPFrame tcp)
 * \brief Gère la Connection en fonction de l'ètat de la connection
 *
 * \param tcp Prend un segment TCP
 */
void Connection::HandleConnection(TCPFrame tcp){
	//On declare notre Trame de reponse
	TCPFrame tcpRes = tcp;
	tcpRes.eth.src = tcp.eth.dst;
	tcpRes.eth.dst = tcp.eth.src;

	tcpRes.ip.src =  tcp.ip.dst;
	tcpRes.ip.dst =  tcp.ip.src;	
	
	//On fait un switch entre l'etat fin wait et Established
	if(tcp.Flags == (TCP_FIN | TCP_ACK)){
		this->State = FIN_WAIT_1;
	}

	this->DataOn = false;
	switch(State){
		case CLOSED:
			//On regarde si c'est un SYN
			
			if(tcp.Flags == TCP_SYN){	
				//On renvoi en consequence
				tcpRes.Flags = TCP_SYN | TCP_ACK;
				tcpRes.seq_number = tcp.ack_number;
				tcpRes.ack_number = tcp.seq_number + 1;
				this->Send(tcpRes);
				this->Type = SERVER;
				this->Frames.push_back(tcpRes);
				this->State = SYN_RECEIVED;
			}else if(tcp.Flags == (TCP_SYN | TCP_ACK)){

				tcpRes.Flags = TCP_ACK;
				tcpRes.seq_number = tcp.ack_number;
				tcpRes.ack_number = tcp.seq_number + 1;
				this->Send(tcpRes);
				this->Type = CLIENT;
				this->Frames.push_back(tcpRes);
				this->State = ESTABLISHED;
			}// Sinon on renvoi un RESET
		break;
			
		case SYN_SENT:
		break;
		
		case SYN_RECEIVED:
			// Le serveur a recu une demande de synchro
			// on va vérifier l'état de l'ack de la reponse du client
			if((tcp.Flags = TCP_ACK) && (this->Frames.back().seq_number == tcp.ack_number-1) && (this->Frames.back().ack_number == tcp.seq_number)){
				this->State = ESTABLISHED;
			}
		break;
		
		case ESTABLISHED:
			if(this->Type == SERVER){
				if(tcp.Flags ==(TCP_ACK | TCP_PSH)){

					tcpRes.seq_number = tcp.ack_number;
					tcpRes.ack_number = tcp.seq_number + tcp.data.length();
					this->Data = tcp.data;
					this->Response = tcpRes;

					this->DataOn = true;
				}
			}else{
				if(tcp.Flags ==(TCP_ACK | TCP_PSH)){

					tcpRes.seq_number = tcp.ack_number;
					tcpRes.ack_number = tcp.seq_number + tcp.data.length();
					tcpRes.data = "";
					this->Data = tcp.data;
					this->Response = tcpRes;
					tcpRes.Flags = TCP_ACK;
					this->DataOn = true;
					this->Send(tcpRes);
				}
			}
		break;
		
		case FIN_WAIT_1:
			if(tcp.Flags ==(TCP_ACK | TCP_FIN)){
				tcpRes.Flags = TCP_ACK | TCP_FIN;
				tcpRes.seq_number = tcp.ack_number;
				tcpRes.ack_number = tcp.seq_number + 1;
				tcpRes.data = "";
				this->State = FIN_WAIT_2; 
				this->Send(tcpRes);
			}
				
		break;
		
		case FIN_WAIT_2:
			this->State = CLOSED;
		break;
		
		case CLOSE_WAIT:
		
		break;
		
		case CLOSING:
		
		break;
		
		case LAST_ACK:
		
		break;
		
		case TIME_WAIT:
		
		break;
		
	}
}

/**
 * \fn string Connection::getData()
 * \brief Permet de récupérer les data de la connection
 */
string Connection::getData(){
	return Data;
}

/**
 * \fn void Connection::SendHTTP(string data)
 * \brief Méthode permettant d'envoyer un paquet contenant du protocole HTTP
 * \param string data Data à envoyer
 */
void Connection::SendHTTP(string data){
	this->Response.Windows = 65300;
	this->Response.data = data;
	this->Send(this->Response);
	
	this->Response.Flags = TCP_ACK | TCP_FIN;
	this->Send(this->Response);
}

/**
 * \fn void Connection::Send(string data)
 * \brief Méthode envoyant de la Data brute sans protocole supérieur. Jusqu'au protocole TCP
 * \param string data Data à envoyer
 */
void Connection::Send(string data){
	this->Response.Windows = 65300;
	this->Response.data = data;
	this->Send(this->Response);
	
}

/**
 * \fn void Connection::Close()
 * \brief Méthode permettant de fermer la connection
 */
void Connection::Close(){
	this->Response.Flags = TCP_FIN | TCP_ACK;
	this->Send(this->Response);
}

/**
 * \fn void Connection::Send(TCPFrame tcp)
 * \brief Envoi d'un segment TCP
 *
 * \param tcp Segment TCP à envoyer
 */
void Connection::Send(TCPFrame tcp){
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
