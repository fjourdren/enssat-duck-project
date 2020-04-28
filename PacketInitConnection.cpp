#include <iostream>
#include <stdio.h>
#include <string>

#include "Packet.h"
#include "PacketInitConnection.h"
#include "ClientSocket.h"

#include "Consts.h"

PacketInitConnection::PacketInitConnection(unsigned int idSender) : Packet("initConnection"), _idSender(idSender) {

}


void PacketInitConnection::action(ClientSocket* cs) {
    cs->setIdClient(this->_idSender);
}


PacketInitConnection::~PacketInitConnection() {}