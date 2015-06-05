#include <string>
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

};