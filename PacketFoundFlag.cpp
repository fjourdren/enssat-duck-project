#include <iostream>
#include <stdio.h>
#include <string>

#include "Packet.h"
#include "PacketFoundFlag.h"

#include "ClientSocket.h"

PacketFoundFlag::PacketFoundFlag(unsigned int idSender, unsigned int flagId) : Packet("foundFlag"), _idSender(idSender), _flagId(flagId) {

}


// construction du paquet en une chaine de caractère envoyable sur le réseau (sérialisation)
std::string PacketFoundFlag::constructString(char delimiter) {
    std::string result = Packet::constructString(delimiter);

    result += delimiter;
    result += std::to_string(this->_idSender);
    result += delimiter;
    result += std::to_string(this->_flagId);

    return result;
}


void PacketFoundFlag::action(ClientSocket* cs) {
    // on cherche dans la liste le flag avec l'id
    Duck* d = cs->getScene()->getDuckById(this->_flagId);

    if(d != nullptr) {
        // on affiche le canard
        d->setDraw(true);

        // on met un message d'information pour le joueur
        if(cs->getIdClient() == this->_idSender) {
            std::cout << "[Game] Vous avez trouvé le canard " << this->_flagId << "." << std::endl;
        } else {
            std::cout << "[Game] Le canard " << this->_flagId << " a été trouvé par le joueur " << this->_idSender << "." << std::endl;
        }
    }
}


PacketFoundFlag::~PacketFoundFlag() {}