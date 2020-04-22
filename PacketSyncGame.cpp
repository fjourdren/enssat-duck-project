#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>

#include "Packet.h"
#include "PacketSyncGame.h"
#include "ClientSocket.h"

#include "Consts.h"

PacketSyncGame::PacketSyncGame(unsigned int seconds, GameState state) : Packet("syncGame"), _seconds(seconds), _state(state) {

}


void PacketSyncGame::action(ClientSocket* cs) {
    // TODO : gérer le chrono

    // gestion du changement de l'état de jeu
    Scene* scene = cs->getScene();
    scene->setGameState(this->_state);

    std::cout << "Nouvel état de la partie : " << this->_state << std::endl;
}