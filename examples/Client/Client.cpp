#include "../../lib/Client.h"

void onData(Connection *connection){

    if(connection->Data.find("Connected") != std::string::npos){
        //Reponse Send
        connection->Send("Connected");
    }else if(connection->Data.find("Close") != std::string::npos){
        //Fermeture de la connection 
        connection->Close();
    }
}



int main()
{
    //Connection a notre server a l'ip 192.168.1.78 sur le port 1337
    Client client = *new Client("10.17.17.17",1337);

    //Ajout de notre fonction onData a notre gestionnaire d'evenement
    client.addEventData(onData);

    //Lancement de l'ecoute
    client.join();

}

