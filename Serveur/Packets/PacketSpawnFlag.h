#ifndef DEF_PACKETSPAWNFLAG
#define DEF_PACKETSPAWNFLAG

#include <string>

#include "Packet.h"

#include "../ClientSession.h"
#include "../Game/Vec3.h"

class PacketSpawnFlag : public Packet {
    public:
        PacketSpawnFlag(int id, std::string type, std::string sound, Vec3 position, Vec3 rotation, bool found);
        virtual std::string constructString(char delimiter); // construction du paquet en une chaine de caractère envoyable sur le réseau (sérialisation)
        //virtual void action(ClientSession* cs); // method qui indique les actions à réaliser lorsque le paquet est reçu 
        virtual ~PacketSpawnFlag();
    private:
        int _id;
        std::string _type;
        std::string _sound;
        Vec3 _position;
        Vec3 _rotation;
        bool _found;
};

#endif