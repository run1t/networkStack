
#include <string>
#include <functional>
#include <stdio.h>
#include <iostream>
#include "Stack.h"
#include "Connection.h"
#include "Frames/TCPFrame.h"

using namespace std;
class Client
{
public:
    /***
    * Attributs du clients
    */
    int port;
    string ip;

    static Client* client;
    Stack* stacker;
    
    /*
    * Construction du client
    **/
    Client(string ip,int port);
    void Send(string message);
    int ipLocal;
    //fonctions de callback
    function<void(Connection*)> onConnection;
    function<void(Connection*)> onData;

    //Declarations des evenements
    void addEventConnection (function<void(Connection*)> func);
    void addEventData (function<void(Connection*)> func);
    void join();

};
