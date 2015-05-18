#include <string>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <functional>
#include "Server.h"


using namespace std;

void onClient(int arg, int arg2){
    cout << "Un nouveau client est connécté" << endl;
}

int main()
{
    Server server = *new Server();
    server.onClient = onClient;
    server.listen();
    //trt
}

