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

    // création du socket serveur
    TCPServerSocket s = TCPServerSocket(port);

    // utilisation d'un type de configuration particulier ou non
    if(argc >= 3) {
        std::string configFile = convertToString(argv[2], strlen(argv[2]));

        // chargement uniquement de fichiers json ou txt
        s.start(configFile);
    } else {
        s.start("configuration.json"); // sinon chargement du fichier configuration.json par défaut
    }
    

    return 0;
}