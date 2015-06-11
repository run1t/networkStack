#define TCP_ACK 16
#define TCP_PSH 8
#define TCP_RST 4
#define TCP_SYN 2
#define TCP_FIN 1


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

class Connection
{
	public:
	static int ConnectionNumber;
	int port;
	int State;
	vector<TCPFrame> Frames;

	Connection(int port);
	void HandleConnection(TCPFrame tcpframe);
	void Send(TCPFrame tcp);
};