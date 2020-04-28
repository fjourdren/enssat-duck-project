#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string.h>
#include <unistd.h>
#include <queue>

#include "Handler.h"

#include "Utils.h"

#include "Consts.h"

#include "./Packets/Packet.h"
#include "./Packets/PacketFoundFlag.h"


Handler::Handler(ClientSession* clientSession): _clientSession(clientSession) {

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
    while(handler->getRunning()) {
        if(handler->getQueueSize() > 0) { // si il y a des messages en attente
            std::string message = handler->dequeueMessage();

            std::cout << "[Handler ClientSession " << handler->getClientSession()->getId() << "] Réception : " << message << std::endl;

            // construction
            Packet* p = Handler::buildPacket(message); // on utilise un pointeur pour facilement par la suite vérifier qu'il a bien été construit

            // exécution action
            if(p != NULL) {  // l'utilisation d'un pointeur permet de réaliser cette condition
                p->action(handler->getClientSession());
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


// getter ClientSession
ClientSession* Handler::getClientSession() {
    ClientSession* output;

    this->_mutex.lock();
    output = this->_clientSession;
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


// construction du paquet
Packet* Handler::buildPacket(std::string message) {
    Packet* outputPacket = nullptr; // l'utilisation d'un pointeur permet de vérifier que l'objet packet a été construit sans erreur. Et si c'est le cas, on ne fait juste rien => permet de ne pas faire crasher l'ensemble des sessions de joueurs.
    std::vector<std::string> parts = split(message, DEFAULT_CHAR_DELIMITER);

    try {
        // reconstruction des objets en objet
        if(parts[0] == "foundFlag") {
            outputPacket = new PacketFoundFlag(std::stoi(parts[1]), std::stoi(parts[2])); // idSender, flagId
        } else {
            std::cout << "[Handler] Construction de ce paquet impossible." << std::endl;
        }
    } catch(std::exception const & e) {
        std::cerr << "ERREUR : Reconstruction du paquet impossible." << std::endl;
    }

    return outputPacket;
}



Handler::~Handler() {
    delete this->_clientSession;
}