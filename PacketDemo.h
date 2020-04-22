#ifndef DEF_PACKETDEMO
#define DEF_PACKETDEMO

#include <string>

#include "Packet.h"
#include "ClientSocket.h"

class PacketDemo : public Packet {
    public:
        PacketDemo(unsigned int idSender, std::string message);
        virtual std::string constructString(char delimiter); // construction du paquet en une chaine de caractère envoyable sur le réseau (sérialisation)
        virtual void action(ClientSocket* cs); // method qui indique les actions à réaliser lorsque le paquet est reçu

    private:
        std::string _message;
        unsigned int _idSender;
};

#endif