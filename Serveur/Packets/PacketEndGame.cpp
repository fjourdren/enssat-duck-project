#include <iostream>
#include <stdio.h>
#include <string>

#include "Packet.h"
#include "PacketEndGame.h"

#include "../ClientSession.h"
#include "../Consts.h"
#include "../Game/Consts.h"

PacketEndGame::PacketEndGame(unsigned int nbCanard, int time, int record) : Packet("endGame"), _nbCanard(nbCanard), _time(time), _record(record) {

}


// construction du paquet en une chaine de caractère envoyable sur le réseau (sérialisation)
std::string PacketEndGame::constructString(char delimiter) {
    std::string result = Packet::constructString(delimiter);

    result += delimiter;
    result += std::to_string(this->_nbCanard);
    result += delimiter;
    result += std::to_string(this->_time);
    result += delimiter;
    result += std::to_string(this->_record);

    return result;
}


PacketEndGame::~PacketEndGame() {}