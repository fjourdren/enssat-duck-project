#ifndef DEF_TCPServerSocket
#define DEF_TCPServerSocket

#include <sys/types.h>
#include <netinet/in.h>
#include <vector>

#include <thread>

class ClientSession;

class TCPServerSocket {
    public:
        TCPServerSocket(int port);
        void start(std::string configFile);
        void readFlagConfig(std::string configFileName);
        void readFlagConfigJSON(std::string configFileNameJson);
        void run();
        void broadcast(std::string contentPacket);
        void stop();
        void close();
        bool removeSession(int idSession);
        bool removeSession(ClientSession* sessionToRemove);
        ~TCPServerSocket();
    private:
        bool _running = true;
        int _port;
        int _socketId;
        struct sockaddr_in _address;
        int _addrlen = sizeof(this->_address);
        unsigned int _nextSessionId = 1;
        std::vector<ClientSession*> _sessions;
};

#endif