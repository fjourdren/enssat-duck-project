#ifndef DEF_PACKET
#define DEF_PACKET

#include <string>

#include "../ClientSession.h"

// classe parente de tous les paquets échangés
class Packet {
    public:
        Packet();
        Packet(std::string type);
        virtual std::string constructString(char delimiter); // construction du paquet en une chaine de caractère envoyable sur le réseau (sérialisation)
        virtual void action(ClientSession* cs); // method qui indique les actions à réaliser lorsque le paquet est reçu

    protected:
        std::string _type;        
};

#endif