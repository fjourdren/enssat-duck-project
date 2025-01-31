#ifndef DEF_PACKETENDGAME
#define DEF_PACKETENDGAME

#include <string>

#include "Packet.h"
#include "ClientSocket.h"

#include "Consts.h"

class PacketEndGame : public Packet {
    public:
        PacketEndGame(unsigned int nbCanard, int time, int record);
        //virtual std::string constructString(char delimiter); // construction du paquet en une chaine de caractère envoyable sur le réseau (sérialisation)
        virtual void action(ClientSocket* cs); // method qui indique les actions à réaliser lorsque le paquet est reçu
        virtual ~PacketEndGame();
    private:
        unsigned int _nbCanard;
        int _time;
        int _record;
};

#endif