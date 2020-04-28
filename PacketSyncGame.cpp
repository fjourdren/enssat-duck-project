#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>

#include "Packet.h"
#include "PacketSyncGame.h"
#include "ClientSocket.h"

#include "Consts.h"

PacketSyncGame::PacketSyncGame(int record, GameState state) : Packet("syncGame"), _record(record), _state(state) {

}


void PacketSyncGame::action(ClientSocket* cs) {
    // gestion du changement de l'état de jeu
    Scene* scene = cs->getScene();
    scene->setGameState(this->_state);

    if(this->_state == INIT) {
        // remet la caméra à sa position initiale
        cs->getScene()->resetCam();

        std::cout << "[Game] Téléportation de la caméra." << std::endl;

        // reset des flags
        cs->getScene()->clearDucks();

        std::cout << "[Game] Reset des canards." << std::endl;
    }
    

    // affichage des informations dans la console du client
    std::cout << "[Game] Nouvel état de la partie : " << PacketSyncGame::conversionGameMode(this->_state) << std::endl;

    if(this->_record == -1) {
        std::cout << "[Game] Ce serveur n'a pas de record enregistré." << std::endl;
    } else {
        std::cout << "[Game] Record actuel du serveur : " << this->_record << " secondes." << std::endl;
    }
}



// permet de FACILITER la transformation de l'id d'état de la partie en string affichable par l'utilisateur
std::string PacketSyncGame::conversionGameMode(int gm) {
    std::string GameStateString[] = {
        "UNDEFINED",
        "INIT",
        "RUN"
    };

    return GameStateString[gm];
}