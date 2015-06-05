#include "Client.h"
Client::Client(int id,int port, string ip){
	this->id = id;
	this->port = port;
	this->ip = ip;
}

void Client::addEventConnection (function<void()> func){
	this->onConnection = func;
}
void Client::addEventDisconnect (function<void()> func){
	this->onDisconnect = func;
}
void Client::addEventError (function<void()> func){
	this->onError = func;	
}

void Client::send(string ip,int port,string message){

}
