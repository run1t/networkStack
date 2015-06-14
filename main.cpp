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
    if(connection->Data.find("pause") != std::string::npos){
        cout << "pause" << endl;
        string command = "su reunan -c 'rhythmbox-client --pause'";
        int result = system(command.c_str());
        connection->SendHTTP("ok");
    }else if(connection->Data.find("play") != std::string::npos){
        cout << "play" << endl;
        string command = "su reunan -c 'rhythmbox-client --play'";
        int result = system(command.c_str());
        cout << result << endl;
        connection->SendHTTP("ok");
    }else if (connection->Data.find("index.html") != std::string::npos) {
        std::ifstream t("index.html");
        std::string str((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());
        //Should output 'this'
        connection->SendHTTP(str);
    }
 
}

void onConnection(Connection *connection){
    if(connection->Data.find("pause") != std::string::npos){
        cout << "pause" << endl;
        string command = "su reunan -c 'rhythmbox-client --pause'";
        int result = system(command.c_str());
        connection->SendHTTP("ok");
    }else if(connection->Data.find("play") != std::string::npos){
        cout << "play" << endl;
        string command = "su reunan -c 'rhythmbox-client --play'";
        int result = system(command.c_str());
        cout << result << endl;
        connection->SendHTTP("ok");
    }else if (connection->Data.find("index.html") != std::string::npos) {
        std::ifstream t("index.html");
        std::string str((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());
        //Should output 'this'
        connection->SendHTTP(str);
    }
 
}

int main()
{
    /*Server server = *new Server("192.168.1.27",80);
    server.addEventData(onData);
    server.join();*/

    Client client = *new Client("192.168.1.27",80);
    client.addEventData(onData);
    client.addEventConnection(onConnection);
    client.join();

}

