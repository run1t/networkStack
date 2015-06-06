#include <string>
#include <functional>
#include <stdio.h>
#include <iostream>

extern "C" {    // another way
	#include "../c/Server.h"
   };
using namespace std;
class Client
{
public:
    /***
    * Attributs du clients
    */

    int id;
    int port;
    string ip;
    int lastAck;

    /*
    * Construction du client
    **/
    Client(int id,int port, string ip,int lastAck);
    void Send(string message);

    //foncctions de callback
    function<void()>	onConnection;
    function<void()>	onDisconnect;
    function<void()>	onError;

    //Declarations des evenements
    //void addEventConnection (function<void()> func);
    //void addEventDisconnect (function<void()> func);
    //void addEventError (function<void()> func);

    //envoi de donnees
    //void send(string ip,int port,string message);
};
