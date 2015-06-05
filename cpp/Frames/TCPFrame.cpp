#include "TCPFrame.h"
int s_port;
	int d_port;
	int seq_number;
	int ack_number;
	int data_offset;
	int flags;
	int windows;
	int checksum;
	int urgentPointer;
	string data;

TCPFrame::TCPFrame(char* frame){

}

TCPFrame::TCPFrame(){

}

TCPFrame::~TCPFrame(){

}
