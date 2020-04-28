#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string.h>
#include <unistd.h>

#include "Handler.h"
#include "ClientSocket.h"
#include "Consts.h"
#include "Utils.h"

#include "Packet.h"
#include "PacketInitConnection.h"
#include "PacketSyncGame.h"
#include "PacketFoundFlag.h"
#include "PacketSpawnFlag.h"
#include "PacketEndGame.h"


Handler::Handler(ClientSocket* clientSocket): _clientSocket(clientSocket) {

}

void Handler::startThread() {
    this->_running = true;
    
    std::thread threadInstance(Handler::run, this);
    this->_thread = std::move(threadInstance); // déplace le thread vers la variable de l'instance
    this->_thread.detach();
}

void Handler::stopThread() {
    this->_mutex.lock();
	this->_running = false;
    this->_mutex.unlock();
}

void Handler::waitThread() {
    this->_thread.join();
}


// exécution du thread qui va réaliser les actions liés au message. Obligatoirement en static, on passe donc this en paramètre
void Handler::run(Handler* handler) {
    std::cout << "test" << std::endl;

    while(handler->getRunning()) {
        if(handler->getQueueSize() > 0) { // si il y a des messages en attente
            std::string message = handler->dequeueMessage();

            std::cout << "[Handler Client] Réception : " << message << std::endl;

            // construction
            Packet* p = Handler::buildPacket(message); // on utilise un pointeur pour facilement par la suite vérifier qu'il a bien été construit

            // exécution action
            if(p != NULL) {  // l'utilisation d'un pointeur permet de réaliser cette condition
                p->action(handler->getClientSocket());
                delete p; // comme on utilise new pour obtenir un pointeur, on suprime l'objet après son utilisation
            }
        } else {
            usleep(10); // processeur au repos si aucun message n'a été trouvé
        }
        
    }
}


// gestion de la queue
std::string Handler::dequeueMessage() {
    std::string output = "";

    this->_mutex.lock();
    output = this->_queue.front();
    this->_queue.pop();
    this->_mutex.unlock();

    return output;
}

void Handler::queueMessage(std::string message) {
    this->_mutex.lock();
    this->_queue.push(message);
    this->_mutex.unlock();
}

int Handler::getQueueSize() {
    int output = 0;

    this->_mutex.lock();
    output = this->_queue.size();
    this->_mutex.unlock();

    return output;
}


// getter ClientSocket
ClientSocket* Handler::getClientSocket() {
    ClientSocket* output;

    this->_mutex.lock();
    output = this->_clientSocket;
    this->_mutex.unlock();

    return output;
}


bool Handler::getRunning() {
    bool output;

    this->_mutex.lock();
    output = this->_running;
    this->_mutex.unlock();

    return output;
}



Packet* Handler::buildPacket(std::string message) {
    Packet* outputPacket = nullptr; // on utilise un pointeur pour facilement par la suite vérifier qu'il a bien été construit
    std::vector<std::string> parts = split(message, DEFAULT_CHAR_DELIMITER);

    try {
        // reconstruction des objets en objet
        if(parts[0] == "initConnection") {
            outputPacket = new PacketInitConnection(std::stoi(parts[1])); // uuidUser
        } else if(parts[0] == "syncGame") {
            GameState state = static_cast<GameState>(std::stoi(parts[2]));
            outputPacket = new PacketSyncGame(std::stoi(parts[1]), state); // secondes, gamestate
        } else if(parts[0] == "foundFlag") {
            outputPacket = new PacketFoundFlag(std::stoi(parts[1]), std::stoi(parts[2])); // idSender, flagId
        } else if(parts[0] == "spawnFlag") {
            vec3 position = vec3::fromValues(std::stod(parts[4]), std::stod(parts[5]), std::stod(parts[6]));
            vec3 rotation = vec3::fromValues(std::stod(parts[7]), std::stod(parts[8]), std::stod(parts[9]));
            bool found = (std::stoi(parts[10]) == 1);
            outputPacket = new PacketSpawnFlag(std::stoi(parts[1]), parts[2], parts[3], position, rotation, found); // id, type, sound, position, rotation, found
        } else if(parts[0] == "endGame") {
            outputPacket = new PacketEndGame(std::stoi(parts[1]), std::stoi(parts[2]), std::stoi(parts[3])); // nbCanard, time, record
        } else {
            std::cout << "[Handler] Construction de ce paquet impossible." << std::endl;
        }
    } catch(std::exception const & e) {
        std::cerr << "ERREUR : Reconstruction du paquet impossible." << std::endl;
    }

    return outputPacket;
}