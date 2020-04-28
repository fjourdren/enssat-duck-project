#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <thread>

#include "TCPServerSocket.h"
#include "ClientSession.h"
#include "Utils.h"

#include "Consts.h"

#include "Packets/PacketInitConnection.h"
#include "Packets/PacketSyncGame.h"
#include "Packets/PacketSpawnFlag.h"

#include "Game/Flag.h"
#include "Game/GameManager.h"

using namespace std;

ClientSession::ClientSession(TCPServerSocket *TCPServerSocket, unsigned int id, int* sock) : _tcpServerSocket(TCPServerSocket), _id(id), _sock(*sock) {
    this->_handler = new Handler(this);
}


int ClientSession::getId() {
	int output;

	this->_mutex.lock();
	output = this->_id;
    this->_mutex.unlock();

	return output;
}


// fonction destiné à démarer le thread de la ClientSession
void ClientSession::startThread() {
	/* Création du thread avec pthread.h
	pthread_t* threadInstance = &(this->_thread);
    typedef void * (*THREADFUNCPTR)(void *);
    if(pthread_create(threadInstance, 
                   NULL, 
                   (THREADFUNCPTR) &ClientSession::run, 
                   this) == -1) {
        std::cerr << "[Session] Erreur lors de la création du thread session." << std::endl;
    }*/

	std::thread threadInstance(ClientSession::run, this);
    this->_thread = std::move(threadInstance); // déplace le thread vers la variable de l'instance
    this->_thread.detach();


	// démarage du thread Handler
	this->_handler->startThread();
}



// force l'arret du thread
void ClientSession::stopThread() {
	this->_mutex.lock();
	this->_running = false;
    this->_mutex.unlock();	
}


// attente de l'arret du thread
void ClientSession::waitThread() {
	/* Attente du thread avec pthread.h
	if (pthread_join(this->_thread, NULL)) {
        std::cout << "[Client] Erreur : Impossible de joindre le thread." << std::endl;
	    exit(EXIT_FAILURE);
    }*/

	this->_thread.join();
}


// envoie d'un message
int ClientSession::send(std::string message) {
	int code;

	// vérification de la longueur du paquet
    if(message.size() + 1 > DEFAULT_SOCKET_BUFFER) {
        std::cout << "[Handler] Construction de ce paquet impossible (chaine de caractère trop longue)." << std::endl;
    } else {
        code = ::send(this->_sock, message.c_str(), (message.size() + 1), 0); // :: pour ne pas utiliser le send de la classe
		if(code < 0) {
			std::cout << "[ClientSession " << this->_id << "] Erreur lors de l'envoi." << std::endl;
		} else {
			std::cout << "[ClientSession " << this->_id << "] Envoi : " << message << std::endl;
		}
    }

	usleep(100); // ajout d'une temporisation pour laisser le temps au client d'ajouer le message à sa queue de traitement

    return code;
}


// réception d'un message
int ClientSession::read(std::string* message) {
	char buffer[DEFAULT_SOCKET_BUFFER];

	// lecture du message reçu et du nombre d'octets
	int bytes_read = recv(this->_sock, buffer, sizeof(buffer), 0);
	*message = convertToString(buffer, strlen(buffer));
	return bytes_read;
}


// fermeture du cocket de ClientSession
void ClientSession::close() {
	::close(this->_sock); // fermeture du socket, :: permet ici de ne pas utiliser la méthode locale
}


// fermeture du socket et destruction de la session
void ClientSession::closeAndDestroy() {
	this->close();
	this->_tcpServerSocket->removeSession(this); // destruction de la session côté serveur
}


// fonction statique pour pouvoir être exécuté dans un thread
void* ClientSession::run(ClientSession* cs) {
	std::cout << "[ClientSession " << cs->_id << "] Démarrage du thread" << std::endl;

    cs->_running = true;
	std::string message = "";


	// Création du paquet pour envoyer son ID au client
	PacketInitConnection packetInit = PacketInitConnection(cs->_id);
	std::string contentPacketInit = packetInit.constructString(DEFAULT_CHAR_DELIMITER);
    cs->send(contentPacketInit);


	// Création du paquet pour envoyer le chronométrage et l'état de la partie à l'utilisateur
	GameManager* gm = GameManager::getinstance();
	PacketSyncGame packetSync = PacketSyncGame(gm->getRecord(), RUN);
	std::string contentPacketSync = packetSync.constructString(DEFAULT_CHAR_DELIMITER);
    cs->send(contentPacketSync);


	// envoi des emplacements de flags
	GameManager* gamemanager = GameManager::getinstance();
    std::vector<Flag*> flags = gamemanager->getFlags();
	for(Flag* flag: flags) {
		PacketSpawnFlag packetFlag = PacketSpawnFlag(flag->getId(), flag->getType(), flag->getSound(), flag->getM_Position(), flag->getM_Orientation(), flag->getFound());
		std::string contentPacketFlag = packetFlag.constructString(DEFAULT_CHAR_DELIMITER);
		cs->send(contentPacketFlag);
	}



	// boucle de lecture dans le thread
	while(cs->_running) {
		int bytes_read = cs->read(&message);
		
		// on retourne l'erreur si il y en a une
		if (bytes_read < 1) {
			if(bytes_read == 0) {
				std::cout << "[ClientSession " << cs->_id << "] Le client a fermé la connexion." << std::endl;
				cs->_running = false;
				cs->close();
				cs->_tcpServerSocket->removeSession(cs); // destruction de la session côté serveur
			} else {
				std::cout << "[ClientSession " << cs->_id << "] Erreur lors de la réception : " << strerror(errno) << "." << std::endl;
				cs->_running = false;
				cs->close();
				cs->_tcpServerSocket->removeSession(cs); // destruction de la session côté serveur
			}

		} else {
			std::cout << "[ClientSession " << cs->_id << "] Réception : " << message << "." << std::endl;

			// ajout du message à la queue du handler
			cs->getHandler()->queueMessage(message);

			message = "";
		}
	}

	std::cout << "[ClientSession " << cs->_id << "] Fin du thread." << std::endl;

	return NULL;
}


// getter socket pour connaitre l'état actuel du socket
unsigned int ClientSession::getSocket() {
	unsigned int output;

	this->_mutex.lock();
	output = this->_sock;
    this->_mutex.unlock();

	return output;
}


TCPServerSocket* ClientSession::getTcpServerSocket() {
	TCPServerSocket* output;

	this->_mutex.lock();
	output = this->_tcpServerSocket;
    this->_mutex.unlock();

	return output;
}


Handler* ClientSession::getHandler() {
	Handler* output;

	this->_mutex.lock();
	output = this->_handler;
    this->_mutex.unlock();

	return output;
}


ClientSession::~ClientSession() {
	delete this->_handler;
}