#ifndef DEF_PacketFoundFlag
#define DEF_PacketFoundFlag

#include <string>

#include "Packet.h"
#include "../ClientSession.h"

class PacketFoundFlag : public Packet {
    public:
        PacketFoundFlag(unsigned int idSender, unsigned int flagId);
        virtual std::string constructString(char delimiter); // construction du paquet en une chaine de caractère envoyable sur le réseau (sérialisation)
        virtual void action(ClientSession* cs); // method qui indique les actions à réaliser lorsque le paquet est reçu

    private:
        unsigned int _idSender;
        unsigned int _flagId;
};

#endif