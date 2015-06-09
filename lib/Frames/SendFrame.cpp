#include "SendFrame.h"	

SendFrame::SendFrame(TCPFrame tcp){
	this->first = true;
	this->retransmission = -1;
	this->lastSend = time(0);
	this->tcp = tcp;
}

bool SendFrame::canRetransmit(){	
	cout << " ok " << this->lastSend << "  :  " << time(0)  << "  ok " << endl;
	return this->lastSend - time(0) > 3;
}

bool SendFrame::retransmissionLimit(){
	return this->retransmission <= 5;
}

void SendFrame::timeCalc(){
	this->lastSend = time(0);
}