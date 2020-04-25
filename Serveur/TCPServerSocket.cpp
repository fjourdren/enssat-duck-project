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

#include <fstream>
#include <sstream>
#include <iostream>

#include <string.h>

#include "TCPServerSocket.h"
#include "ClientSession.h"

#include "Consts.h"

#include "Game/GameManager.h"
#include "Game/Vec3.h"
#include "Game/Flag.h"

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


    // lecture du fichier de configuration
    this->readFlagConfig("configuration.txt");

    // lecture du record
    GameManager* gm = GameManager::getinstance();
    gm->loadRecord();

    // démarage du compteur de la partie
    gm->restartCounter();

    // Démarrage du serveur
    this->run();
}


// lecture du fichier de configuration contenant l'ensemble des flags
void TCPServerSocket::readFlagConfig(std::string configFileName) {
    // Paramètres de lecture du fichier
    std::fstream file;
    std::string line;
    std::string token;

    // Paramètres du flag (objectif)
    std::string soundDuck;
    std::string type = "d";
    Vec3 position = Vec3();
    Vec3 orientation = Vec3();
    int idFlag = 0;

    // Ouverture du fichier
    file.open(configFileName); 

    // Lecture du fichier
    while(getline(file, line)) {
        std::istringstream buffer(line);

        // Lecture ligne par ligne et affectation
        while(std::getline(buffer, token, ':')){
            std::getline(buffer, token, ':');
            soundDuck = token;


            std::getline(buffer, token, ':');
            position.setX(atoi(token.c_str()));

            std::getline(buffer, token, ':');
            position.setY(atoi(token.c_str()));

            std::getline(buffer, token, ':');
            position.setZ(atoi(token.c_str()));



            std::getline(buffer, token, ':');
            orientation.setX(atoi(token.c_str()));

            std::getline(buffer, token, ':');
            orientation.setY(atoi(token.c_str()));

            std::getline(buffer, token, ':');
            orientation.setZ(atoi(token.c_str()));
            

            idFlag = idFlag + 1;
        }


        Flag* f = new Flag(idFlag, type, soundDuck, position, orientation);


        // Changement de case du vector
        GameManager::getinstance()->addFlag(f);
    }
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


// broadcast d'un paquet à toutes les sessions
void TCPServerSocket::broadcast(std::string contentPacket) {
    // vérification de la longueur du paquet
    for(ClientSession* session: this->_sessions) {
        session->send(contentPacket);
    }
}


// arret du thread
void TCPServerSocket::stop() {
    this->_running = false;
}


// fermeture du socket et de toutes les sessions
void TCPServerSocket::close() {
    // Arret de toures les threads sessions
    for(ClientSession* sessionStop: this->_sessions) {
        sessionStop->stopThread();
    }


    // Attente des threads et fermeture des sockets
    for(ClientSession* sessionWait: this->_sessions) {
        sessionWait->waitThread();
        sessionWait->close();
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