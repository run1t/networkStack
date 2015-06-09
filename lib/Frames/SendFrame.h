#include "TCPFrame.h"
#include <ctime>
#include <iostream>
using namespace std;

class SendFrame
{
public:
	int retransmission;
	time_t lastSend;
	bool first;
	TCPFrame tcp;
	SendFrame(TCPFrame tcp);
	bool retransmissionLimit();
	bool canRetransmit();
	void timeCalc();
};