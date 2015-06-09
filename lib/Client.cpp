/*!
 * \file Client.cpp
 * \brief Fichier de gestion de la couche client
 * \author LE NOC Reunan, HIPEAU Kevin, VIAUD Thommas, TRICHARD Guillaume
 * \version 1.0
 */
#include "Client.h"
Client* Client::client;
void onDatass(Connection* connection){
	Client::client->onData(connection);
}


Client::Client(string ip,int port){
	this->ipLocal = 58240;
	this->ip = ip;
	this->port = port;
	this->stacker =  new Stack(ip,this->ipLocal);
    this->stacker->addDataEvent(onDatass);
}

void Client::Send(string Message){
	
}

//Declarations des evenements
void Client::addEventConnection (function<void(Connection*)> func){
	this->onConnection = func;
}


void Client::addEventData (function<void(Connection*)> func){
	this->onData = func;
}

void Client::join(){
	Client::client = this;
	TCPFrame tcp = *new TCPFrame();
	tcp.Flags = TCP_SYN;
	tcp.eth.dst = "c8:e0:eb:14:57:f3";
	tcp.ip.dst = this->ip;
	tcp.ip.src = "192.168.1.26";
	tcp.dst = this->ipLocal;
	tcp.src = this->port;
	this->stacker->Send(tcp);
	this->stacker->receiver();
}
