#include <iostream>
#include <stdio.h>
#include <string>

#include "Packet.h"
#include "PacketFoundFlag.h"

#include "../Game/GameManager.h"

#include "../ClientSession.h"

#include "../Consts.h"

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


void PacketFoundFlag::action(ClientSession* cs) {
    // on cherche le pointeur du flag ayant l'id
    GameManager* gamemanager = GameManager::getinstance();
    Flag* f = gamemanager->getFlagById(this->_flagId);

    // si le flag a été trouvé
    if(f != nullptr) {
        // si il n'est pas dans la liste des flags trouvés
        if(f->getFound() == false) {
            // on rajoute le flag aux flags trouvés
            f->setFound(true);

            // on broadcast à tous les clients
            std::string contentPacket = this->constructString(DEFAULT_CHAR_DELIMITER);
            cs->getTcpServerSocket()->broadcast(contentPacket);

            // on met un message dans la console
            std::cout << "[Game] Le canard " << this->_flagId << " a été trouvé par le joueur " << this->_idSender << "." << std::endl;

            // tous les flags ont ils étaient trouvés ? Si oui => fin de la partie
            /*if(gamemanager->getFlagsFound().size() >= gamemanager->getFlags().size()) {
                // TODO
                std::cout << "[Game] Tous les canards ont été trouvés." << std::endl;
            }*/
        }
    } else {
        std::cout << "[Game] Flag non trouvé." << std::endl;
    }
}