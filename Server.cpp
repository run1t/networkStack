
//
//  Server.cpp
//  callback Test
//
//  Created by Reunan Le noc on 02/06/15.
//  Copyright (c) 2015 Reunan Le noc. All rights reserved.
//

#include "Server.h"

void Stack(void* args){
 	Server *s = static_cast<Server*>(args);
 	int sock;
	sock = initServer();
	struct Servers server  = createServer("192.168.1.2",80);
	cout << "Initialisation du Server" << endl;
    while (1) {
        
		char* ok = listenOn(server,sock);
		
		if(strcmp (ok,"NULL") != 0){

			if(strcmp(ok,"Connection") == 0){
				s->onClient(2,4);
			}
		}
		
	
    }
}


Server::Server(){
    
}

void Server::on(string event, function<void (int, int)> func){
        this->onClient = func;
}

void Server::listen(){
	unsigned short *ok;
    thread t1(Stack,this);
    t1.join();
}

