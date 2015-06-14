
//
//  Server.cpp
//  callback Test
//
//  Created by Reunan Le noc on 02/06/15.
//  Copyright (c) 2015 Reunan Le noc. All rights reserved.
//
#include "Server.h"
Server* Server::server;
void onDatas(Connection *connection){
	cout << "on a notre premier niveau "<< endl;
	Server::server->onData(connection);
}


/**
* Gestion de la stack IP
*/

Server::Server(string ip, int port){
 	this->stacker =  new Stack(ip,port);
    this->stacker->addDataEvent(onDatas);
}



/**
* Gestion des evenements qui sorte 
* de notre pile
*/


void Server::addEventData  (function<void(Connection*)> func){
	this->onData = func;
}


void Server::join(){
	Server::server = this;
	this->stacker->receiver();
}

