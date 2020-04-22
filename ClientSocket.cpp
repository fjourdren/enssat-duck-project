#include <thread>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#include "ClientSocket.h"
#include "Handler.h"
#include "Utils.h"

#include "Consts.h"

ClientSocket::ClientSocket(char* ip, int port): _ip(ip), _port(port) {
    
}


void ClientSocket::startThread() {
    /* Création du thread avec pthread.h
    pthread_t* threadInstance = &(this->_thread);
    typedef void * (*THREADFUNCPTR)(void *);
    if(pthread_create(threadInstance, 
                   NULL, 
                   (THREADFUNCPTR) &ClientSocket::run, 
                   this) == -1) {
        std::cerr << "[Client] Erreur lors de la création du thread client." << std::endl;
    }*/
    
    std::thread threadInstance(ClientSocket::run, this);
    this->_thread = std::move(threadInstance); // déplace le thread dans la variable de l'instance
    this->_thread.detach();
}


void ClientSocket::stopThread() {
    this->_running = false;
}


void ClientSocket::waitThread() {
    /* Attente du thread avec pthread.h
    if (pthread_join(this->_thread, NULL)) {
        std::cout << "[Client] Erreur : Impossible de joindre le thread." << std::endl;
	    exit(EXIT_FAILURE);
    }*/

    this->_thread.join();
}


// envoi d'un message au serveur
int ClientSocket::send(std::string message) {
    std::cout << "[Client] Envoi : " << message << std::endl;
    int code = ::send(this->_sock, message.c_str(), (message.size() + 1), 0); // :: pour ne pas appeler la méthode locale
	if(code < 0) {
		std::cout << "[Client] Erreur lors de l'envoi " << strerror(errno) << "." << std::endl;
	}

    return code;
}


// lecture d'un message venu du serveur
int ClientSocket::read(std::string* message) {
	char buffer[DEFAULT_SOCKET_BUFFER];

	// lecture du message reçu et du nombre d'octets
	int bytes_read = recv(this->_sock, buffer, sizeof(buffer), 0);
	*message = convertToString(buffer, strlen(buffer));
    return bytes_read;
}


void ClientSocket::close() {
    ::close(this->_sock); // :: permet ici de ne pas prendre la méthode locale
}


void ClientSocket::connect() {
    struct sockaddr_in serv_addr;

    if ((this->_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "[Client] Erreur lors de la création du socket." << std::endl;
        exit(EXIT_FAILURE);
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(this->_port);
       
    // conversion de l'adresse en binaire (IPv4 & IPv6)
    if(inet_pton(AF_INET, this->_ip, &serv_addr.sin_addr)<=0) { 
        std::cerr << "[Client] Adresse invalide." << std::endl;
        exit(EXIT_FAILURE);
    }
   
    if (::connect(this->_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {  // :: permet ici de ne pas prendre la méthode locale
        std::cerr << "[Client] Serveur injoignable." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "[Client] Connecté à " << this->_ip << ":" << this->_port << "." << std::endl;
}


void* ClientSocket::run(ClientSocket* cs) { // fonction static pour pouvoir être exécuté dans un thread
    std::cout << "[Client] Démarrage du thread" << std::endl;

    cs->_running = true;
    cs->connect();

    // boucle de lecture dans le thread
	while(cs->_running) {
		std::string message = "";
		int bytes_read = cs->read(&message);

		// on retourne l'erreur si il y en a une
		if (bytes_read < 1) {
			if(bytes_read == 0) {
				std::cout << "[Client] Le serveur a fermé la connexion." << std::endl;
				cs->_running = false;
				cs->close();
			} else {
				std::cout << "[Client] Erreur lors de la réception : " << strerror(errno) << "." << std::endl;
                cs->_running = false;
				cs->close();
            }

		} else {
			std::cout << "[Client] Réception : " << message << std::endl;

            // reconstruction de l'objet et traitement
			Handler::handle(cs, message);

            message = "";
		}
	}

    std::cout << "[Client] Fin du thread." << std::endl;

    return NULL;
}


// getter de l'id du socket (permet de faire attendre la connexion au thread principal)
int ClientSocket::getSocket() {
    return this->_sock;
}


int ClientSocket::getIdClient() {
    return this->_idClient;
}

void ClientSocket::setIdClient(unsigned int newIdClient) {
    this->_idClient = newIdClient;
}

Scene* ClientSocket::getScene() {
    return this->_scene;
}

void ClientSocket::setScene(Scene* newScene) {
    this->_scene = newScene;
}