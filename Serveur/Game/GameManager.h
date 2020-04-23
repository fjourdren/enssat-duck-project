#ifndef DEF_GAMEMANAGER
#define DEF_GAMEMANAGER

#include <mutex>
#include <vector>

#include "Flag.h"
#include "Consts.h"

class GameManager {
    public:
        //static GameManager* getinstance();  // get istance du singleton
        void restartCounter();  // redémarrage du timer de la partie
        double calculateTime(); // calcul du temps écoulé depuis le début de la partie
        Flag* getFlagById(int flagId); // récupère le pointeur flag par rapport à son ID


        // getter & setter de l'état de la partie
        GameState getState();
        void setState(GameState newState);

        // getter & setter flags
        void addFlag(Flag* f);
        std::vector<Flag*> getFlags();
        bool removeFlag(int id);
        bool removeFlag(Flag* flagToRemove);
        void clearFlags();

        // singleton
        static GameManager *getinstance() {
            if (!_instance)
                _instance = new GameManager;
            return _instance;
        }

    private:
        static GameManager* _instance; // variable du singleton
        GameManager(); // mise en privé du constructeur pour forcer à utiliser le singleton

        std::mutex _mutex;   // mutex qui protège le singleton
        clock_t _counter; // date début de la partie
        GameState _state = INIT;
        std::vector<Flag*> _flags;
};

#endif