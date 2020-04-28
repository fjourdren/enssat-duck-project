#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>

#include "Packet.h"
#include "PacketFoundFlag.h"
#include "PacketSyncGame.h"
#include "PacketSpawnFlag.h"
#include "PacketEndGame.h"

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
            if(gamemanager->getNbFlagsFound() == gamemanager->getFlags().size()) {
                unsigned int time = (unsigned int) gamemanager->calculateTime(); // temps pour trouver tous les canards
                std::cout << "[Game] Tous les canards ont été trouvés en " << time << " secondes ." << std::endl;


                // gestion d'un potentiel nouveau record
                gamemanager->newPotencialRecord(time);


                // envoi de endgame
                unsigned int nbCanard = gamemanager->getFlags().size();

                PacketEndGame packetEndGame = PacketEndGame(nbCanard, time, gamemanager->getRecord()); // TODO record
                std::string contentPacketEndGame = packetEndGame.constructString(DEFAULT_CHAR_DELIMITER);
                cs->getTcpServerSocket()->broadcast(contentPacketEndGame);

                usleep(100000); // attente entre chaque envoi de message

                // passage de la partie en état INIT
                PacketSyncGame packetSyncINIT = PacketSyncGame(gamemanager->getRecord(), INIT);
                std::string contentPacketSyncINIT = packetSyncINIT.constructString(DEFAULT_CHAR_DELIMITER);
                cs->getTcpServerSocket()->broadcast(contentPacketSyncINIT);

                usleep(10000);

                // chargement de la config
                gamemanager->clearFlags();
                cs->getTcpServerSocket()->readFlagConfig("configuration.txt");

                usleep(10000);

                // envoie des canards aux clients
                std::vector<Flag*> flags = gamemanager->getFlags();
                for(Flag* flag: flags) {
                    PacketSpawnFlag packetFlag = PacketSpawnFlag(flag->getId(), flag->getType(), flag->getSound(), flag->getM_Position(), flag->getM_Orientation(), flag->getFound());
                    std::string contentPacketFlag = packetFlag.constructString(DEFAULT_CHAR_DELIMITER);
                    cs->getTcpServerSocket()->broadcast(contentPacketFlag);
                    usleep(10000);
                }

                usleep(10000);

                // passage de la partie en état RUN
                PacketSyncGame packetSyncRUN = PacketSyncGame(gamemanager->getRecord(), RUN);
                std::string contentPacketSyncRUN = packetSyncRUN.constructString(DEFAULT_CHAR_DELIMITER);
                cs->getTcpServerSocket()->broadcast(contentPacketSyncRUN);


                // démarage du compteur de la nouvelle partie
                GameManager::getinstance()->restartCounter();
            }
        }
    } else {
        std::cout << "[Game] Flag non trouvé." << std::endl;
    }
}


PacketFoundFlag::~PacketFoundFlag() {}