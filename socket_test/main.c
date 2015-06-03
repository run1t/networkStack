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
	*/ 	
	int sock;
	struct Servers server  = createServer("192.168.1.2",80);
	sock = initServer();
	

	while(1){
		struct responseStack stack = listenOn(server,sock);
		if(stack.Type == 1){

			printf("Connection");
			printf("New user connected\n");			
		}
		else if(stack.Type == 2){
			printf("Data received\n");	
		}else if(stack.Type == 3){
			printf("Deconnection\n");
		}else if(stack.Type == 0){
			printf("Error\n");
		}
			
	}
	return 0;
}

//Gestion de la fermeture du programme
void handle_SIGINT(int signal){
	if(signal == SIGINT){
		printf("Terminating the TCP/IP Stack\n");

		//On remet le icmp_echo_ignore_all à 0 pour le désactiver
		//Cela permet de réactiver la réponse au ping
		int fdProc = open("/proc/sys/net/ipv4/icmp_echo_ignore_all",O_WRONLY);
		if(fdProc == -1){
			perror("Error opening the file /proc/sys/net/ipv4/icmp_echo_ignore_all\n");
		}
		int nbByteWrite = write(fdProc,"0",1);
		if(nbByteWrite == 0){
			printf("No write\n");
		}
		else if(nbByteWrite == -1){
			perror("Error writing to file\n");
		}
		else if(nbByteWrite > 0){
			printf("Succes writing to file\n");
			int closeSuccess = close(fdProc);
			if(closeSuccess == 0){
				printf("File closed successfully\n");
			}else if(closeSuccess == -1){
				printf("Error closing the file\n");
			}
		}
		exit(0);
	}
}
