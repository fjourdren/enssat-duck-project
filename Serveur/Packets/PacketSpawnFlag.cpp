#include <iostream>
#include <stdio.h>
#include <string>

#include "Packet.h"
#include "PacketSpawnFlag.h"

#include "../ClientSession.h"
#include "../Consts.h"
#include "../Game/Consts.h"

PacketSpawnFlag::PacketSpawnFlag(int id, std::string type, std::string sound, Vec3 position, Vec3 rotation, bool found) : Packet("spawnFlag"), _id(id), _type(type), _sound(sound), _position(position), _rotation(rotation), _found(found) {

}


// construction du paquet en une chaine de caractère envoyable sur le réseau (sérialisation)
std::string PacketSpawnFlag::constructString(char delimiter) {
    std::string result = Packet::constructString(delimiter);

    result += delimiter;
    result += std::to_string(this->_id);
    result += delimiter;
    result += this->_type;
    result += delimiter;
    result += this->_sound;


    result += delimiter;
    result += std::to_string(this->_position.getX());
    result += delimiter;
    result += std::to_string(this->_position.getY());
    result += delimiter;
    result += std::to_string(this->_position.getZ());

    result += delimiter;
    result += std::to_string(this->_rotation.getX());
    result += delimiter;
    result += std::to_string(this->_rotation.getY());
    result += delimiter;
    result += std::to_string(this->_rotation.getZ());

    result += delimiter;
    result += this->_found ? "1" : "0"; // conversion d'un booléen en chaine de caractères

    return result;
}


PacketSpawnFlag::~PacketSpawnFlag() {}