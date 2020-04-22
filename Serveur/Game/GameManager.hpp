#ifndef DEF_GAMEMANAGER
#define DEF_GAMEMANAGER

#include <mutex>
#include <vector>

#include "Flag.hpp"
#include "Consts.hpp"

class GameManager {
    public:
        static GameManager* getinstance();  // get istance du singleton
        void restartCounter();  // redémarrage du timer de la partie
        double calculateTime(); // calcul du temps écoulé depuis le début de la partie
        // getter & setter de l'état de la partie
        GameState getState();
        void setState(GameState newState);
        void addFlag(Flag* f);
        bool removeFlag(int id);
        bool removeFlag(Flag* flagToRemove);
        void clearFlags();

    private:
        static GameManager* _instance; // variable du singleton
        std::mutex _mutex;   // mutex qui protège le singleton
        clock_t _counter; // date début de la partie
        GameState _state = INIT;
        std::vector<Flag*> _flags;

        GameManager(); // mise en privé du constructeur pour forcer à utiliser le singleton
};


// Implementation Singleton
GameManager* GameManager::_instance = 0;

GameManager* GameManager::getinstance() {
    if (_instance == 0) { // vérifie si il y une instance du GameManager
        _instance = new GameManager();  // construction de l'instance si elle n'existe pas
    }

    return _instance;
}

GameManager::GameManager() {}

#endif