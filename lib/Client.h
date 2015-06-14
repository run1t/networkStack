
#include <string>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Stack.h"
#include "Connection.h"
#include "Frames/TCPFrame.h"
#include "Frames/ARPFrame.h"

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
    ushort portLocal;
    //fonctions de callback
    function<void(Connection*)> onConnection;
    function<void(Connection*)> onData;

    //Declarations des evenements
    void addEventConnection (function<void(Connection*)> func);
    void addEventData (function<void(Connection*)> func);
    void join();
    string getArpMac();

};
