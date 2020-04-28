#ifndef DEF_CLIENTSESSION
#define DEF_CLIENTSESSION

#include <thread>
#include <mutex>

#include "TCPServerSocket.h"

#include "Handler.h"

class ClientSession {
    public:
        ClientSession(TCPServerSocket* TCPServerSocket, unsigned int id, int* sock);
        int getId(); // getter id
        void startThread(); // fonction destiné à démarer le thread de la ClientSession
        void stopThread(); // force l'arret du thread
        void waitThread(); // attente de l'arret du thread
        int send(std::string message); // envoie d'un message
        int read(std::string* message); // réception d'un message
        void close(); // fermeture du socket de ClientSession
        void closeAndDestroy(); // fermeture du socket et destruction de la session
        unsigned int getSocket(); // getter socket
        TCPServerSocket* getTcpServerSocket();
        Handler* getHandler();
        ~ClientSession();
    private:
        std::mutex _mutex;
        TCPServerSocket* _tcpServerSocket;
        Handler* _handler;
        unsigned int _id; // id de la session
        std::thread _thread;
        int _sock = -1;  // id du socket vers le client
        std::thread _ClientSessionThread; // id du thread utilisé pour lire les messages reçus
        bool _running = false; // booléen qui permet d'arréter la boucle dans le thread

        static void* run(ClientSession* cs); // fonction static pour pouvoir être exécuté dans un thread
};

#endif