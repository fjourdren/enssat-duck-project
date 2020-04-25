#ifndef DEF_PacketSpawnFlag
#define DEF_PacketSpawnFlag

#include <string>

#include "Packet.h"
#include "ClientSocket.h"

class PacketSpawnFlag : public Packet {
    public:
        PacketSpawnFlag(int id, std::string type, std::string sound, vec3 position, vec3 rotation, bool found);
        //virtual std::string constructString(char delimiter); // construction du paquet en une chaine de caractère envoyable sur le réseau (sérialisation)
        virtual void action(ClientSocket* cs); // method qui indique les actions à réaliser lorsque le paquet est reçu

    private:
        int _id;
        std::string _type;
        std::string _sound;
        vec3 _position;
        vec3 _rotation;
        bool _found;
};

#endif