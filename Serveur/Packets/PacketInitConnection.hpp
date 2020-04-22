#ifndef DEF_PACKETINITCONNECTION
#define DEF_PACKETINITCONNECTION

#include <string>

#include "Packet.hpp"
#include "../ClientSession.hpp"

class PacketInitConnection : public Packet {
    public:
        PacketInitConnection(unsigned int idSender);
        virtual std::string constructString(char delimiter); // construction du paquet en une chaine de caractère envoyable sur le réseau (sérialisation)
        //virtual void action(ClientSession* cs); // method qui indique les actions à réaliser lorsque le paquet est reçu
    private:
        unsigned int _idSender;
};

#endif