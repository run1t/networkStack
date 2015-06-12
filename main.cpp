#include <string>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <functional>
#include "lib/Server.h"
#include <fstream>
#include <streambuf>

using namespace std;

void onData(Connection *connection){
    
    if (connection->Data.find("index.html") != std::string::npos) {
        std::ifstream t("index.html");
        std::string str((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());
        //Should output 'this'
        connection->SendHTTP(str);
    }else if(connection->Data.find("pause") != std::string::npos){
        cout << "pause" << endl;
        connection->SendHTTP("ok");
    }else if(connection->Data.find("play") != std::string::npos){
        cout << "play" << endl;
        connection->SendHTTP("ok");
    }
 
}

int main()
{
    Server server = *new Server("10.17.16.22",80);
    server.addEventData(onData);
    server.join();
}

