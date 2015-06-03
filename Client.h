#include <string>
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

};