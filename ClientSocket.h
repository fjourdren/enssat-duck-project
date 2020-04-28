#ifndef DEF_CLIENTSOCKET
#define DEF_CLIENTSOCKET

#include <thread>
#include <iostream>
#include <string>

#include "Scene.h"

class ClientSocket {
    public:
        ClientSocket(char* ip, int port);
        void startThread();
        void stopThread(); // force l'arret du thread
        void waitThread(); // attente de l'arret du thread
        int send(std::string message); // envoie d'un message
        int read(std::string* message); // réception d'un message
        void close(); // fermeture de la connexion
        int getSocket();
        int getIdClient();
        void setIdClient(unsigned int newIdClient);
        Scene* getScene();
        void setScene(Scene* newScene);
        ~ClientSocket();
    private:
        std::thread _thread;
        Scene* _scene;
        unsigned int _idClient = 0;
        char* _ip;
        int _port;
        bool _running = false; // bool pour savoir si le thread doit continue ou non
        int _sock;  // id du socket vers le serveur

        void connect();
        static void* run(ClientSocket* cs); // fonction static pour pouvoir être exécuté dans un thread
};

#endif