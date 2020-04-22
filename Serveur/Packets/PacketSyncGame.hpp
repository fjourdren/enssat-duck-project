#ifndef DEF_PACKETSYNCGAME
#define DEF_PACKETSYNCGAME

#include <string>

#include "Packet.hpp"

#include "../ClientSession.hpp"
#include "../Consts.hpp"
#include "../Game/Consts.hpp"

class PacketSyncGame : public Packet {
    public:
        PacketSyncGame(unsigned int seconds, GameState state);
        virtual std::string constructString(char delimiter); // construction du paquet en une chaine de caractère envoyable sur le réseau (sérialisation)
        //virtual void action(ClientSession* cs); // method qui indique les actions à réaliser lorsque le paquet est reçu
    private:
        unsigned int _seconds;
        GameState _state;
};

#endif