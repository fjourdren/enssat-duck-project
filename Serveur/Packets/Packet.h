#ifndef DEF_PACKET
#define DEF_PACKET

#include <string>

class ClientSession;

// classe parente de tous les paquets échangés
class Packet {
    public:
        Packet();
        Packet(std::string type);
        virtual std::string constructString(char delimiter); // construction du paquet en une chaine de caractère envoyable sur le réseau (sérialisation)
        virtual void action(ClientSession* cs); // methode qui indique les actions à réaliser lorsque le paquet est reçu
        virtual ~Packet(); // Cela permet d'utiliser les destructeurs des classes filles.
    protected:
        std::string _type;        
};

#endif