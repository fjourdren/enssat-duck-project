#include <iostream>
#include <stdio.h>
#include <string>

#include "Packet.hpp"
#include "PacketDemo.hpp"
#include "../ClientSession.hpp"

#include "../Consts.hpp"

PacketDemo::PacketDemo(unsigned int idSender, std::string message) : Packet("PacketDemo"), _idSender(idSender), _message(message) {

}


// construction du paquet en une chaine de caractère envoyable sur le réseau (sérialisation)
std::string PacketDemo::constructString(char delimiter) {
    std::string result = Packet::constructString(delimiter);

    result += delimiter;
    result += std::to_string(this->_idSender);
    result += delimiter;
    result += this->_message;

    return result;
}


void PacketDemo::action(ClientSession* cs) {
    this->_message += "-b";
    std::string contentPacket = this->constructString(DEFAULT_CHAR_DELIMITER);

    // vérification de la longueur du paquet
    if(contentPacket.size() + 1 > DEFAULT_SOCKET_BUFFER) {
        std::cout << "[Handler] Construction de ce paquet impossible (chaine de caractère trop longue)." << std::endl;
    } else {
        cs->send(contentPacket);
    }
}