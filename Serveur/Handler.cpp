#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string.h>

#include "Handler.h"
#include "ClientSession.h"

#include "Consts.h"

#include "./Packets/Packet.h"
#include "./Packets/PacketDemo.h"

// reconstruit le message en objet puis exécute les actions
void Handler::handle(ClientSession* cs, std::string message) {
    Packet* p = Handler::buildPacket(message);

    if(p != NULL) {
        p->action(cs);
    }
}


Packet* Handler::buildPacket(std::string message) {
    Packet* outputPacket;
    std::vector<std::string> parts = Handler::split(message, DEFAULT_CHAR_DELIMITER);

    // reconstruction des objets en objet
    if(parts[0] == "PacketDemo") {
        outputPacket = new PacketDemo(std::stoi(parts[1]), parts[2]); // uuidUser, message
    } else {
        std::cout << "[Handler] Construction de ce paquet impossible (erreur de type)." << std::endl;
        return NULL;
    }

    return outputPacket;
}


// découpe d'un string avec un délimiteur (pour la désérialisation)
std::vector<std::string> Handler::split(std::string message, char delimiter) {
    std::vector<std::string> vout;
    unsigned int start = 0, end = 0;

    while(end < message.length()) {
        end = message.find(delimiter, start);
        vout.push_back(message.substr(start, end - start));
        start = end + 1;
    }

    vout.push_back(message.substr(start));

    return vout;
}