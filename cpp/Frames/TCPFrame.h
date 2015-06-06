#include <string>
#include <iostream>
#include <sstream>
#include "ETHFrame.h"
#include "IPFrame.h"
#if !defined( TCPFRAME_H )
#define TCPFRAME_H
using namespace std;
class TCPFrame
{
public:
	ETHFrame eth;
	IPFrame ip;
	int src;
	int dst;
	int seq_number;
	int ack_number;
	int HeaderLength;
	int Flags;
	int Windows;
	int Checksum;
	int urgentPointer;
	string data;

	TCPFrame(unsigned char* buffer,int size);
	TCPFrame();	
};

#endif // !defined( TCPFRAME_H )