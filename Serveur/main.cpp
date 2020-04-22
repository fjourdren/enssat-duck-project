#include <iostream>
#include <stdlib.h>

#include <unistd.h>

#include "TCPServerSocket.hpp"

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
    s.start();

    return 0;
}