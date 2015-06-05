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

    int id;
    int port;
    string ip;

    /*
    * Construction du client
    **/
    Client(int id,int port, string ip);

    //foncctions de callback
    function<void()>	onConnection;
    function<void()>	onDisconnect;
    function<void()>	onError;

    //Declarations des evenements
    void addEventConnection (function<void()> func);
    void addEventDisconnect (function<void()> func);
    void addEventError (function<void()> func);

    //envoi de donnees
    void send(string ip,int port,string message);
};
