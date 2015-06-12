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
#include "Connection.h"
using namespace std;

#endif /* defined(__callback_Test__Server__) */




class Server
{
public:
    static Server* server;

    //Stockage des fonctions de callbacks
    //Gestion des evenements du client
    function<void(Connection*)> onData;
    Stack* stacker;
    list<Client> Clients = *new list<Client>();

    // Constructeurs
    Server(string ip,int port);
    

    
    //Declaration des evenements
    void addEventData   (function<void(Connection*)> func);


    //Envoi des données
    void send (string message,Client client);
    void join();
    //Gestion de la pile utilisateur
    void addClient(Client client);
    void removeClient(Client client);
    Client getClient(int id);

};