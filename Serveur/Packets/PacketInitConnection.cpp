#include <iostream>
#include <stdio.h>
#include <string>

#include "Packet.h"
#include "PacketInitConnection.h"
#include "../ClientSession.h"

#include "../Consts.h"

PacketInitConnection::PacketInitConnection(unsigned int idSender) : Packet("initConnection"), _idSender(idSender) {

}


// construction du paquet en une chaine de caractère envoyable sur le réseau (sérialisation)
std::string PacketInitConnection::constructString(char delimiter) {
    std::string result = Packet::constructString(delimiter);

    result += delimiter;
    result += std::to_string(this->_idSender);

    return result;
}


PacketInitConnection::~PacketInitConnection() {}