#include <iostream>
#include <stdio.h>
#include <string>

#include "Packet.h"
#include "PacketSyncGame.h"

#include "../ClientSession.h"
#include "../Consts.h"
#include "../Game/Consts.h"

PacketSyncGame::PacketSyncGame(unsigned int seconds, GameState state) : Packet("syncGame"), _seconds(seconds), _state(state) {

}


// construction du paquet en une chaine de caractère envoyable sur le réseau (sérialisation)
std::string PacketSyncGame::constructString(char delimiter) {
    std::string result = Packet::constructString(delimiter);

    result += delimiter;
    result += std::to_string(this->_seconds);
    result += delimiter;
    unsigned int state = static_cast<unsigned int>(this->_state);
    result += std::to_string(state);

    return result;
}