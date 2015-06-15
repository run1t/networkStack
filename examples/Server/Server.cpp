#include "../../lib/Server.h"

/**
* On declare les action que l'on va faire lorsque l'on va recevoir
* de la data.
*/

void onData(Connection *connection){
    //Renvoi d'un Echo
    connection->Send(connection->Data);
}


int main()
{
    //Instanciation du Server , ecoute sur le port 80
    Server server = *new Server("192.168.1.27",80);
    
    //Ajout de notre fonction onData a notre gestionnaire d'evenement
    server.addEventData(onData);
    
    //Lancement de l'ecoute
    server.join();
}

