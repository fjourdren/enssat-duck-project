#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "TCPServerSocket.h"

#include "Utils.h"

int main(int argc, char **argv) {
    int port(25000);

    // conversion du port passé en paramètre en int
    if(argc >= 2) {
        try {
            port = std::stoi(argv[1]);
        } catch(std::exception const & e) {
            std::cerr << "ERREUR : Conversion des entrées utilisateurs impossibles." << std::endl;
            exit(EXIT_FAILURE);
        }
    }


    // utilisation d'un type de configuration particulier ou non
    std::string configFile = "configuration.json";
    if(argc >= 3) { // si il y a un fichier de configuration renseigné par l'utilisateur
        configFile = convertToString(argv[2], strlen(argv[2]));
    }


    // création du socket serveur
    TCPServerSocket s = TCPServerSocket(port, configFile);
    s.start();
    

    return 0;
}