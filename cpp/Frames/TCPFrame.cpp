#include "TCPFrame.h"


TCPFrame::TCPFrame(unsigned char* buffer,int size){
	
	/**
	* On recupere les autres couches 
	*/

	this->eth = *new ETHFrame(buffer,size);
	this->ip = *new IPFrame(buffer,size);

	/**
	* On recupere TCP
	*/
	int begin = this->ip.HeaderLength+13;

	this->src = buffer[begin+1]*256 + buffer[begin+2];
	this->dst = buffer[begin+3]*256 + buffer[begin+4];
	this->seq_number = (buffer[begin+5] << 24) | (buffer[begin+6] << 16) | (buffer[begin+7] << 8) | buffer[begin+8];
	this->ack_number = (buffer[begin+9] << 24) | (buffer[begin+10] << 16) | (buffer[begin+11] << 8) | buffer[begin+12];
	this->HeaderLength = (buffer[begin+13] >> 4)*4;
	this->Flags = buffer[begin+14];
	this->Windows = buffer[begin+15]*256 + buffer[begin+16];
	this->Checksum = buffer[begin+17]*256 + buffer[begin+18];
	this->urgentPointer = buffer[begin+19]*256 + buffer[begin+20];

	//On recupere le message TCP
	this->data = "";
	for(int i = this->ip.HeaderLength+this->HeaderLength+14 ; i <= this->ip.TotalLength+13 ; i++){
		this->data += buffer[i];
	}
}

TCPFrame::TCPFrame(){

}