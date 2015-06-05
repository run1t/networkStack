#include <string>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <functional>
#include "cpp/Server.h"


using namespace std;

void onClient(Client client){
    cout << "Un nouveau client est connécté" << endl;
}

void onData(string data,Client client){
 
	if(data.compare("hello") == 0){
		client.Send("Server:Bonjour petit client");
	}else if(data.compare("un") == 0){
		client.Send("Deux");
	}
}

void onLeave(Client client){
	cout << "Close" << endl;
}

void onConnection(){
	cout << "I'm connected !" << endl;
}

int main()
{
/*
    Server server = *new Server();
    server.addEventClient(onClient);
    server.addEventData(onData);
    server.addEventLeave(onLeave);
    server.listen();
*/

	//Test Client
	Client client = *new Client(1,5654,"10.17.40.3");
	client.addEventConnection(onConnection);
}

