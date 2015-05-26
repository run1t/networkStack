#include "Server.h"

int main(int argc, char *argv[])
{	
	/**
	* Creation du Serveur 
	* - createServeur;
	* - char* ip;
	* - int port;
	* - function CallBack;
	* Cette Fonction va permettre de creer un serveur qui va écouter en 
	* permanence sur un certain port.
	*/ 
	int sock;
	struct Server server  = createServer("192.168.1.2",80);
	sock = initServer();
	listenOn(server,sock);


	return 0;
}
