#include <string>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <functional>
#include "cpp/Stack.h"


using namespace std;


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
    Stack stack = * new Stack("192.168.1.27",80);
    stack.receiver();
    //trt
}

