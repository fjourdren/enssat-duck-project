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
#include "PacketFoundFlag.h"
#include "PacketSpawnFlag.h"
#include "PacketEndGame.h"

// reconstruit le message en objet puis exécute les actions
void Handler::handle(ClientSocket* cs, std::string message) {
    Packet* p = Handler::buildPacket(message); // on utilise un pointeur pour facilement par la suite vérifier qu'il a bien été construit
    
    if(p != NULL) { // l'utilisation d'un pointeur permet de réaliser cette condition
        p->action(cs);

        delete p; // supression de l'instance packet instancié avec new pour libérer la mémoire
    }
}


Packet* Handler::buildPacket(std::string message) {
    Packet* outputPacket = nullptr; // on utilise un pointeur pour facilement par la suite vérifier qu'il a bien été construit
    std::vector<std::string> parts = Handler::split(message, DEFAULT_CHAR_DELIMITER);

    try {
        // reconstruction des objets en objet
        if(parts[0] == "initConnection") {
            outputPacket = new PacketInitConnection(std::stoi(parts[1])); // uuidUser
        } else if(parts[0] == "syncGame") {
            GameState state = static_cast<GameState>(std::stoi(parts[2]));
            outputPacket = new PacketSyncGame(std::stoi(parts[1]), state); // secondes, gamestate
        } else if(parts[0] == "foundFlag") {
            outputPacket = new PacketFoundFlag(std::stoi(parts[1]), std::stoi(parts[2])); // idSender, flagId
        } else if(parts[0] == "spawnFlag") {
            vec3 position = vec3::fromValues(std::stod(parts[4]), std::stod(parts[5]), std::stod(parts[6]));
            vec3 rotation = vec3::fromValues(std::stod(parts[7]), std::stod(parts[8]), std::stod(parts[9]));
            bool found = (std::stoi(parts[10]) == 1);
            outputPacket = new PacketSpawnFlag(std::stoi(parts[1]), parts[2], parts[3], position, rotation, found); // id, type, sound, position, rotation, found
        } else if(parts[0] == "endGame") {
            outputPacket = new PacketEndGame(std::stoi(parts[1]), std::stoi(parts[2]), std::stoi(parts[3])); // nbCanard, time, record
        } else {
            std::cout << "[Handler] Construction de ce paquet impossible." << std::endl;
        }
    } catch(std::exception const & e) {
        std::cerr << "ERREUR : Reconstruction du paquet impossible." << std::endl;
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