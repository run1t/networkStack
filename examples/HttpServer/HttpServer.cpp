#include <fstream>
#include <streambuf>
#include <string>
#include "../../lib/Server.h"
using namespace std;

void onData(Connection *connection){

    if (connection->Data.find("index.html") != std::string::npos) {

    	//Lecture du fichiers index.html
        ifstream t("HttpServer/index.html");
        string file((istreambuf_iterator<char>(t)),
        			istreambuf_iterator<char>());
        
        //Envoi de la requête HTTP;
        connection->SendHTTP(file);

    }else{

    	//Envoi d'une 404 si la page n'est pas trouvé
    	connection->SendHTTP("404 Not Found");
    }
 
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

