#include "Server.h"
#include "icmp.h"
#include <signal.h>

void handle_SIGINT(int signal);

int main(int argc, char *argv[])
{	
	//Structure pour gérer les signaux
	struct sigaction sa;

	sa.sa_handler = &handle_SIGINT;
	sa.sa_flags = SA_RESTART;
	//On prend en compte tout les signaux
	sigfillset(&sa.sa_mask);
	if(sigaction(SIGINT,&sa,NULL) == -1){
		printf("Error handling SIGINT\n");
	}
	/**
	* Creation du Serveur 
	* - createServeur;
	* - char* ip;
	* - int port;
	* - function CallBack;
	* Cette Fonction va permettre de creer un serveur qui va écouter en 
	* permanence sur un certain port.
	*/ 	int sock;
	struct Server server  = createServer("192.168.1.2",80);
	sock = initServer();
	listenOn(server,sock);
	//sendICMP_request();	


	return 0;
}

//Gestion de la fermeture du programme
void handle_SIGINT(int signal){
	if(signal == SIGINT){
		printf("Terminating the TCP/IP Stack\n");
		exit(0);
	}
}
