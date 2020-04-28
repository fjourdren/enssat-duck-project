#include <stdio.h>
#include <string>
#include <iostream>

#include "Packet.h"
#include "../ClientSession.h"

Packet::Packet(std::string type) : _type(type) {
    
}


std::string Packet::constructString(char delimiter) {
    std::string result = "";

    (void) delimiter; // retirer le warning de non utilisation d'une variable (pour ne pas casser l'héritage)

    // concatenation
    result += this->_type;
    
    return result;
}


void Packet::action(ClientSession* cs) {
    (void) cs; // retirer le warning de non utilisation d'une variable (pour ne pas casser l'héritage)

    std::cout << "[Handler] Erreur, un paquet nul a été reçu." << std::endl;
}


Packet::~Packet() {}