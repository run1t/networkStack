#TCP/IP Stack

##Configuration requise
Afin de pouvoir faire en sorte que la stack TCP/IP fonctionne il vous faut imperativement :

```
Linux
iptables
être root
```

## Introduction
Cette stack TCP/IP ecrite en c++ vous permettera de créer un serveur TCP et un client TCP , la stack reponds aussi automatiquement au requetes ARP et ICMP.

### Création d'un serveur TCP

```
#include "lib/Server.h"
void onData(Connection *connection){
    if(connection->Data.find("pause") != std::string::npos){
        connection->Send("Echo pause");
    }else if(connection->Data.find("play") != std::string::npos){
        cout << "play" << endl;
        connection->Send("Echo play");
    }
}

int main()
{
    Server server = *new Server("192.168.1.27",80);
    server.addEventData(onData);
    server.join();
}
```

### Création d'un Client TCP

```
#include "lib/Client.h"
void onData(Connection *connection){
    if(connection->Data.find("Connected") != std::string::npos){
        connection->Send("Connection active");
    }else if(connection->Data.find("Close") != std::string::npos){
        connection->Close();
    }
}

int main()
{
    Client client = *new Client("192.168.1.78",1337);
    client.addEventData(onData);
    client.join();

}
```

## Server et Client
### addEventData(connection)
Cette methode qui est la même pour le client et le serveur va permettre la gestion des données arrivant sur le client ou le server c'est a cet endroit que l'on va mettre le code qui va permettre la communication entre nos servers et nos clients.

## Connection
La classe connection represente la connection ouverte soit entre le serveur et le client soit entre le client et le server.

### ->Data
Renvoi les données de la connection courante sous forme de `string`

### ->Close
Ferme la connection courante


