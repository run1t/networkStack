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

int main()
{
    Server server = *new Server();
    server.addEventClient(onClient);
    server.addEventData(onData);
    server.addEventLeave(onLeave);
    server.listen();
    //trt
}

