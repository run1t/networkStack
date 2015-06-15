
//
//  Server.cpp
//  callback Test
//
//  Created by Reunan Le noc on 02/06/15.
//  Copyright (c) 2015 Reunan Le noc. All rights reserved.
//
#include "Server.h"
Server* Server::server;

/**
 * \fn void Server::onDatas(Connection *connection)
 * \brief Fonction permettant de réagir sur la réception de Data
 * \param Connection *connection
 */
void onDatas(Connection *connection){
	cout << "on a notre premier niveau "<< endl;
	Server::server->onData(connection);
}


/**
* Gestion de la stack IP
*/

/**
 * \fn Server::Server(string ip, int port)
 * \brief Constructeur de la classe Server
 * \param ip ip du serveur
 * \param port port du serveur
 */
Server::Server(string ip, int port){
 	this->stacker =  new Stack(ip,port);
    this->stacker->addDataEvent(onDatas);
}



/**
* Gestion des evenements qui sorte 
* de notre pile
*/


/**
 * \fn void Server::addEventData  (function<void(Connection*)> func)
 * \brief bind une fonction à l'évenement de réception de Data
 * \param function fonction à bindé
 */
void Server::addEventData  (function<void(Connection*)> func){
	this->onData = func;
}

/**
 * \fn void Server::join()
 * \brief lance l'écoute
 */
void Server::join(){
	Server::server = this;
	this->stacker->receiver();
}
