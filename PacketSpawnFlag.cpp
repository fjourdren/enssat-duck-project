#include <iostream>
#include <stdio.h>
#include <string>

#include "Packet.h"
#include "PacketSpawnFlag.h"

#include "ClientSocket.h"

PacketSpawnFlag::PacketSpawnFlag(int id, std::string type, std::string sound, vec3 position, vec3 rotation, bool found) : Packet("spawnFlag"), _id(id), _type(type), _sound(sound), _position(position), _rotation(rotation), _found(found) {

}


void PacketSpawnFlag::action(ClientSocket* cs) {
    // création d'un flag en attente de spawn et ajout à la liste des flags en attente de spawn (car spawn dans la boucle principale obligatoire !)
    FlagToSpawn* ds = new FlagToSpawn(this->_id, this->_type, this->_sound, this->_position, this->_rotation, this->_found);
    cs->getScene()->addFlagToSpawn(ds);
}


PacketSpawnFlag::~PacketSpawnFlag() {}