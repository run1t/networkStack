#include "Client.h"
Client::Client(int id,int port, string ip,int lastAck){
	this->id = id;
	this->port = port;
	this->ip = ip;
	this->lastAck = lastAck;
}

void Client::Send(string Message){
	sendClient(this->port, (char*)this->ip.c_str() , (char*)Message.c_str(),this->lastAck);
}