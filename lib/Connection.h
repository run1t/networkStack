/*!
 * \file Connection.h
 * \brief Fichier de gestion de la connexion
 * \author LE NOC Reunan, HIPEAU Kevin, VIAUD Thommas, TRICHARD Guillaume
 * \version 1.0
 */
#define TCP_ACK 16
#define TCP_PSH 8
#define TCP_RST 4
#define TCP_SYN 2
#define TCP_FIN 1
#define CLIENT 1
#define SERVER 2

#define SYN_SENT 1
#define SYN_RECEIVED 2
#define ESTABLISHED 3
#define FIN_WAIT_1 4
#define FIN_WAIT_2 5
#define CLOSE_WAIT 6
#define CLOSING 7
#define LAST_ACK 8
#define TIME_WAIT 9
#define CLOSED 10
#include <string>
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <functional>
#include <vector>
#include "PC.h"
#include "Frames/TCPFrame.h"
#include <vector>
extern "C" {    // another way
	#include <sys/socket.h>
    #include <netpacket/packet.h>
    #include <net/ethernet.h>

};

#if !defined( CONNECTION_H )
#define CONNECTION_H
class Connection
{
	public:
	static int ConnectionNumber;
	int port;
	int State;
	bool DataOn;
	int Type;
	TCPFrame Response;
	string Data;
	vector<TCPFrame> Frames;

	Connection(int port);
	void HandleConnection(TCPFrame tcpframe);
	void Send(TCPFrame tcp);
	void Send(string data);
	void Close();
	void SendHTTP(string data);
	string getData();
};
#endif
