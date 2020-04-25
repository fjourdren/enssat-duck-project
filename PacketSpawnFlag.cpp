#include <iostream>
#include <stdio.h>
#include <string>

#include "Duck.h"

#include "Packet.h"
#include "PacketSpawnFlag.h"

#include "ClientSocket.h"

PacketSpawnFlag::PacketSpawnFlag(int id, std::string type, std::string sound, vec3 position, vec3 rotation, bool found) : Packet("spawnFlag"), _id(id), _type(type), _sound(sound), _position(position), _rotation(rotation), _found(found) {

}


void PacketSpawnFlag::action(ClientSocket* cs) {
    // si on veut un flag de type cannard
    if(this->_type == "d") {
        //Duck* duck = new Duck(this->_sound);
        /*duck->setPosition(this->_position);
        duck->setOrientation(this->_rotation);
        duck->setPosDuck(1);
        duck->setDraw(this->_found);
        duck->setSound(true);
        duck->setId(this->_id);*/

        //cs->getScene()->addDuck(duck);
        std::cout << "[Game] Ajout d'un canard envoyé par le serveur." << std::endl;
    }
    
}