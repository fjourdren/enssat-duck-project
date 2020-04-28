#ifndef DEF_HANDLER
#define DEF_HANDLER

#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>

#include "Packet.h"

class Handler {
    public:
        Handler(ClientSocket* clientSocket);

        void startThread();
        void stopThread();
        void waitThread();

        static void run(Handler* handler); // boucle qui lit les message dans la queue

        // management de la queue
        std::string dequeueMessage();
        void queueMessage(std::string message);
        int getQueueSize();
        ClientSocket* getClientSocket();
        bool getRunning();


        ~Handler();
    private:
        std::mutex _mutex;
        std::thread _thread;
        std::queue<std::string> _queue;
        ClientSocket* _clientSocket;
        bool _running;

        static Packet* buildPacket(std::string message);
};

#endif