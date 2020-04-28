#include <stdio.h>
#include <string>
#include <iostream>

#include "Packet.h"
#include "ClientSocket.h"

Packet::Packet(std::string type) : _type(type) {
    
}


std::string Packet::constructString(char delimiter) {
    std::string result = "";

    // concatenation
    result += this->_type;
    
    return result;
}


void Packet::action(ClientSocket* cs) {
    std::cout << "[Handler] Erreur, un paquet nul a été reçu." << std::endl;
}


Packet::~Packet() {}