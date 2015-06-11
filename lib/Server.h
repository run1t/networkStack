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
#include <vector>
#include <sstream>
#include "Stack.h"
#include "Client.h"
using namespace std;
extern "C" {    // another way
    #include <netpacket/packet.h>
};
#endif /* defined(__callback_Test__Server__) */




class Server
{
public:
    //Stockage des fonctions de callbacks
    //Gestion des evenements du client
    function<void(Client)>        onClient;
    function<void(string,Client)> onData;
    function<void(Client)>        onLeave;

    //Gestion des evenements automatique
    function<void(string)> onPing;
    function<void(string)> onARP;

    list<Client> Clients = *new list<Client>();

    // Constructeurs
    Server();
    
    void listen();
    
    //Declaration des evenements
    void addEventClient (function<void(Client)> func);
    void addEventData   (function<void(string,Client)> func);
    void addEventLeave  (function<void(Client)> func);
    void addEventPing   (function<void(string)> func);
    void addEventARP    (function<void(string)> func);

    //Envoi des données
    void send (string message,Client client);

    //Gestion de la pile utilisateur
    void addClient(Client client);
    void removeClient(Client client);
    Client getClient(int id);

};