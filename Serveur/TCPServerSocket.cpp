#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netdb.h>
#include <vector>
#include <errno.h>

#include <string.h>

#include "TCPServerSocket.hpp"
#include "ClientSession.hpp"

#include "Consts.hpp"

TCPServerSocket::TCPServerSocket(int port): _port(port) {
    
}


void TCPServerSocket::start() {
    // init vars
    int opt = 1;
       
    // Création du socket
    if ((_socketId = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "[Serveur] Erreur création socket" << std::endl;
        exit(EXIT_FAILURE);
    }
       
    // Configuration des options du socket
    if (setsockopt(_socketId, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "[Serveur] Erreur dans la mise en place des options de socket dans setsockopt" << std::endl;
        exit(EXIT_FAILURE);
    }

    // init des variables pour address
    this->_address.sin_family      = AF_INET; 
    this->_address.sin_addr.s_addr = INADDR_ANY; 
    this->_address.sin_port        = htons(this->_port);
       
    // Attache le socket au port sélectionné
    if (bind(this->_socketId, (struct sockaddr *) &this->_address, sizeof(this->_address)) < 0) {
        std::cerr << "[Serveur] Erreur lors du binding" << std::endl;
        exit(EXIT_FAILURE);
    }

    // démarre l'écoute du serveur, longueur de la queue d'attente à la connexion de 3 places
    if (listen(this->_socketId, 3) < 0) {
        std::cerr << "[Serveur] Échec du démarrage l'écoute." << std::endl;
        exit(EXIT_FAILURE);
    } else {
        std::cout << "[Serveur] Écoute en cours sur le port " << this->_port << std::endl;
    }

    this->run();
}


void TCPServerSocket::run() { // fonction pour accepter la connexion de nouveau clients
    int newClientSocket; // socket de la connexion avec le client

    // boucle qui accepte des clients en boucle
    while(this->_running && (newClientSocket = accept(this->_socketId, (struct sockaddr *) &this->_address, (socklen_t*) &this->_addrlen))) {
        // on accepte les nouvelles connexions, si il y a une erreur, on l'affiche. Sinon, on créer le thread pour ce client.
        if (newClientSocket < 0) { 
            std::cerr << "[Serveur] Erreur lors de la connexion d'un nouveau client" << std::endl; // en cas d'erreur, on affiche l'erreur dans la console puis on passe à la connexion suivante
        } else {
            std::cout << "[Serveur] Nouveau client" << std::endl;

            // création de la session
            ClientSession* session = new ClientSession(this, this->_nextSessionId, &newClientSocket);
            session->startThread();
            this->_nextSessionId += 1;

            // ajout de la session à la liste des sessions en cours
            this->_sessions.push_back(session);
        }
    }

    this->close();
}


void TCPServerSocket::stop() {
    this->_running = false;
}


void TCPServerSocket::close() {
    // Arret de toures les threads sessions
    auto sessionStop = this->_sessions.begin();
	while (sessionStop != this->_sessions.end()) {
        (*sessionStop)->stopThread();
    }


    // Attente des threads et fermeture des sockets
    auto sessionWait = this->_sessions.begin();
	while (sessionWait != this->_sessions.end()) {
        (*sessionWait)->waitThread();
        (*sessionWait)->close();
    }


    // reset de la liste des sessions
    this->_sessions.clear();

    // fermeture du socket serveur
    ::close(this->_socketId);
}


bool TCPServerSocket::removeSession(int idSession) {
    auto session = this->_sessions.begin();
	while (session != this->_sessions.end()) {
        if((*session)->getId() == idSession) {
            this->_sessions.erase(session);
            std::cout << "[ClientSession " << idSession << "] Retiré de la liste des sessions." << std::endl;
            return true;
        }
    }

    return false;
}


bool TCPServerSocket::removeSession(ClientSession* sessionToRemove) {
    auto session = this->_sessions.begin();
	while (session != this->_sessions.end()) {
        if((*session)->getId() == sessionToRemove->getId()) {
            this->_sessions.erase(session);
            std::cout << "[ClientSession " << sessionToRemove->getId() << "] Retiré de la liste des sessions." << std::endl;
            return true;
        }
    }

    return false;
}