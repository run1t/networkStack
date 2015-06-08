#include <string>
#include <functional>
#include <stdio.h>
#include <iostream>

using namespace std;
class Client
{
public:
    /***
    * Attributs du clients
    */
    int port;
    string ip;

    /*
    * Construction du client
    **/
    Client(int port, string ip);
    void Send(string message);

    //fonctions de callback
    function<void(Client)>	         onConnection;
    function<void(Client,string)>    onData;
    function<void()>	             onClose;
    function<void(string)>	         onError;

    //Declarations des evenements
    void addEventConnection (function<void(Client)> func);
    void addEventClose (function<void()> func);
    void addEventError (function<void(string)> func);
    void addEventData (function<void(Client,string)> func);

    //envoi de donnees
    //void send(string ip,int port,string message);
};
