#include <string>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <functional>
#include "Server.h"


using namespace std;

void onClient(Client client){
    cout << "Un nouveau client est connécté" << endl;
}

void onData(string data,Client client){
 
	if(data.compare("hello") == 0){
		cout << "bonjour petit client !" << endl;
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

