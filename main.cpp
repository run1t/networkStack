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

int main()
{
    Server server = *new Server();
    server.addEventClient(onClient);
    server.addEventData(onData);
    server.addEventLeave(onLeave);
    server.listen();
    //trt
}

