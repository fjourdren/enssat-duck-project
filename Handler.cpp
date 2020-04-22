#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string.h>

#include "Handler.h"
#include "ClientSocket.h"
#include "Consts.h"

#include "Packet.h"
#include "PacketInitConnection.h"
#include "PacketSyncGame.h"

// reconstruit le message en objet puis exécute les actions
void Handler::handle(ClientSocket* cs, std::string message) {
    Packet* p = Handler::buildPacket(message);
    
    if(p != NULL) {
        p->action(cs);
    }
}


Packet* Handler::buildPacket(std::string message) {
    Packet* outputPacket;
    std::vector<std::string> parts = Handler::split(message, DEFAULT_CHAR_DELIMITER);

    // reconstruction des objets en objet
    if(parts[0] == "initConnection") {
        outputPacket = new PacketInitConnection(std::stoi(parts[1])); // uuidUser
    } else if(parts[0] == "syncGame") {
        GameState state = static_cast<GameState>(std::stoi(parts[2]));
        outputPacket = new PacketSyncGame(std::stoi(parts[1]), state); // secondes, gamestate
    } else {
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