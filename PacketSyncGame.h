#ifndef DEF_PACKETSYNCGAME
#define DEF_PACKETSYNCGAME

#include <string>

#include "Packet.h"
#include "ClientSocket.h"

#include "Consts.h"

class PacketSyncGame : public Packet {
    public:
        PacketSyncGame(int record, GameState state);
        //virtual std::string constructString(char delimiter); // construction du paquet en une chaine de caractère envoyable sur le réseau (sérialisation)
        virtual void action(ClientSocket* cs); // method qui indique les actions à réaliser lorsque le paquet est reçu
    private:
        int _record;
        GameState _state;
        static std::string conversionGameMode(int gm);
};

#endif