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
#include "./Packets/PacketFoundFlag.h"

// reconstruit le message en objet puis exécute les actions
void Handler::handle(ClientSession* cs, std::string message) {
    Packet* p = Handler::buildPacket(message); // on utilise un pointeur pour facilement par la suite vérifier qu'il a bien été construit

    if(p != NULL) {  // l'utilisation d'un pointeur permet de réaliser cette condition
        p->action(cs);
        delete p; // comme on utilise new pour obtenir un pointeur, on suprime l'objet après son utilisation
    }
}


Packet* Handler::buildPacket(std::string message) {
    Packet* outputPacket = nullptr; // l'utilisation d'un pointeur permet de vérifier que l'objet packet a été construit sans erreur. Et si c'est le cas, on ne fait juste rien => permet de ne pas faire crasher l'ensemble des sessions de joueurs.
    std::vector<std::string> parts = Handler::split(message, DEFAULT_CHAR_DELIMITER);

    try {
        // reconstruction des objets en objet
        if(parts[0] == "foundFlag") {
            outputPacket = new PacketFoundFlag(std::stoi(parts[1]), std::stoi(parts[2])); // idSender, flagId
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