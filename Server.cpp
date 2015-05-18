
//
//  Server.cpp
//  callback Test
//
//  Created by Reunan Le noc on 02/06/15.
//  Copyright (c) 2015 Reunan Le noc. All rights reserved.
//

#include "Server.h"
vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
  
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
  
  return internal;
}
/**
* Gestion de la stack IP
*/

void Stack(void* args){
 	Server *s = static_cast<Server*>(args);
 	int sock;
	sock = initServer();
	struct Servers server  = createServer("192.168.1.2",80);
	cout << "Initialisation du Server" << endl;
    while (1) {
        
		struct responseStack stack = listenOn(server,sock);


		if(stack.Type == 1){
				cout << "/********* Type Tcp **********/" << endl;
			switch(stack.Tcp.Type){
				case 1:
		   		cout << "/* Connection" << endl;
		   		cout << "/* Id :" << stack.Tcp.id << endl;
		   		cout << "/* Port :" << stack.Tcp.port << endl;
		   		cout << "/* IP :" << stack.Tcp.Ip << endl;
		   		cout << "/***************************/\n\n" << endl;
				break;
				case 2:
				cout << "/* Message" << endl;
		   		cout << "/* Id :" << stack.Tcp.id << endl;
		   		cout << "/* Port :" << stack.Tcp.port << endl;
		   		cout << "/* IP :" << stack.Tcp.Ip << endl;
		   		cout << "/* Message : " << stack.Tcp.message << endl;
		   		cout << "/***************************/\n\n" << endl;
				break;
				case 3:
				cout << "/* Close" << endl;
		   		cout << "/* Id :" << stack.Tcp.id << endl;
		   		cout << "/* Port :" << stack.Tcp.port << endl;
		   		cout << "/* IP :" << stack.Tcp.Ip << endl;
		   		cout << "/***************************/\n\n" << endl;
				break;
			}
			
		}
		/*vector<string> sep = split(ok, ':');
  		string commande(ok);
		if(strcmp (ok,"NULL") != 0){

			if(strcmp(ok,"Connection") == 0){
				Client newClient = *new Client(124,80,"192.168.1.2");
				if(s->onClient != NULL)
					s->onClient(newClient);
				s->addClient(newClient);
			}

			else if(sep.at(0).compare("Data") == 0){
				if(s->onData != NULL)
					s->onData(sep.at(1),*new Client(21,80,"123.2.3.4."));
			}

			else if(strcmp(ok,"close") == 0){
				if(s->onLeave != NULL)
					s->onLeave(*new Client(21,80,"123.2.3.4."));
			}
		}*/
		
	
    }
}


Server::Server(){
    
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

/*
* Gestion de la pile utilsateur
*/
void Server::addClient(Client client){

}


void Server::removeClient(Client client){

}

Client Server::getClient(int id){

}


void Server::listen(){
	unsigned short *ok;
    thread t1(Stack,this);
    t1.join();
}

