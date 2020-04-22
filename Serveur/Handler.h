#ifndef DEF_HANDLER
#define DEF_HANDLER

#include <string>
#include <vector>

#include "ClientSession.h"
#include "./Packets/Packet.h"

class Handler {
    public:
        static void handle(ClientSession* sc, std::string message);
    private:
        static Packet* buildPacket(std::string message);
        static std::vector<std::string> split(std::string message, char delimiter); // découpe d'un string avec un délimiteur (pour la désérialisation)  
};

#endif