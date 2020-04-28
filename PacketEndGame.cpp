#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>

#include "Packet.h"
#include "PacketEndGame.h"
#include "ClientSocket.h"

#include "Consts.h"

PacketEndGame::PacketEndGame(unsigned int nbCanard, int time, int record) : Packet("endGame"), _nbCanard(nbCanard), _time(time), _record(record) {

}


void PacketEndGame::action(ClientSocket* cs) {
    std::cout << "[Game] Fin de la partie, les " << this->_nbCanard << " canards ont été trouvés en " << this->_time << " secondes." << std::endl;

    if(this->_record != -1) {
        if(this->_record == this->_time) {
            std::cout << "[Game] Vous réalisez le nouveau record du serveur avec " << this->_time << " secondes." << std::endl;
        } else {
            std::cout << "[Game] Vous ne battez pas le record du serveur qui est de  " << this->_record << " secondes." << std::endl;
        }
    }
}


PacketEndGame::~PacketEndGame() {}