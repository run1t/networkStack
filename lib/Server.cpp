
//
//  Server.cpp
//  callback Test
//
//  Created by Reunan Le noc on 02/06/15.
//  Copyright (c) 2015 Reunan Le noc. All rights reserved.
//
#include "Server.h"



/**
* Gestion de la stack IP
*/

Server::Server(string ip, int port){
    
	Stack stack = new Stack(); 


    stack.addSynEvent = 
   	stack.addDataEvent = 
    stack.addFinEvent = 
}


/**
* Gestion des evenements qui sorte 
* de notre pile
*/


void Server::addEventClient (function<void(Client)> func){
	this->onClient = func;
}

void Server::addEventData   (function<void(string,Client)> func){
	this->onData = func;
}

void Server::addEventLeave  (function<void(Client)> func){
	this->onLeave = func;
}

void Server::addEventPing   (function<void(string)> func){
	this->onPing = func;
}

void Server::addEventARP    (function<void(string)> func){
	this->onARP = func;
}

/**
* Gestion de l'envoi des données
*/

//Envoi des données
void Server::send (string message,Client client){

}