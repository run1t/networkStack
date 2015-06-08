/*!
 * \file Client.cpp
 * \brief Fichier de gestion de la couche client
 * \author LE NOC Reunan, HIPEAU Kevin, VIAUD Thommas
 * \version 1.0
 */
#include "Client.h"
Client::Client(int port, string ip){
	this->port = port;
	this->ip = ip;
}

void Client::Send(string Message){
	
}

//Declarations des evenements
void Client::addEventConnection (function<void(Client)> func){
	this->onConnection = func;
}

void Client::addEventClose (function<void()> func){
	this->onClose = func;
}

void Client::addEventError (function<void(string)> func){
	this->onError = func;
}

void Client::addEventData (function<void(Client,string)> func){
	this->onData = func;
}
