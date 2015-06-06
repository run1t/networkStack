#include <string>
#include <iostream>
#include <sstream>
#include "ETHFrame.h"
#include "IPFrame.h"
#include <vector>
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
	int frameLength;

	TCPFrame(unsigned char* buffer,int size);
	TCPFrame();
	unsigned char* toFrame();
};

#endif // !defined( TCPFRAME_H )