//
//  Server.h
//  callback Test
//
//  Created by Reunan Le noc on 02/06/15.
//  Copyright (c) 2015 Reunan Le noc. All rights reserved.
//

#ifndef __callback_Test__Server__
#define __callback_Test__Server__

#include <stdio.h>
#include <string>
#include <list>
#include <functional>
#include <thread>
#include <chrono>
#include <iostream>
#include "Client.h"
using namespace std;

extern "C" {    // another way
	#include "socket_test/Server.h"
   };

#endif /* defined(__callback_Test__Server__) */




class Server
{
public:
    //Stockage des fonctions de callbacks
    //Gestion des evenements du client
    function<void(Client)> onClient;
    function<void(string,Client)> onData;
    function<void(Client) onLeave;

    //Gestion des evenements automatique
    function<void(string)> onPing;
    function<void(string)> onARP;

    list<Client> Clients = new list<Client>();

    // Constructeurs
    Server();
    
    void listen();
    void addEventClient(function<void(Client)> func);
    void addEventData(function<void(Client)> func);
    void addEventLeave(function<void(Client)> func);
    void addEventLeave(function<void(Client)> func);
    void addEventLeave(function<void(Client)> func);
};