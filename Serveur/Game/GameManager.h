#ifndef DEF_GAMEMANAGER
#define DEF_GAMEMANAGER

#include <mutex>
#include <vector>

#include "Flag.h"
#include "Consts.h"

class GameManager {
    public:
        static GameManager* getinstance(); // singleton

        void restartCounter();  // redémarrage du timer de la partie
        int calculateTime(); // calcul du temps écoulé depuis le début de la partie
        Flag* getFlagById(int flagId); // récupère le pointeur flag par rapport à son ID
        unsigned int getNbFlagsFound();


        // getter & setter de l'état de la partie
        GameState getState();
        void setState(GameState newState);

        // getter & setter flags
        void addFlag(Flag* f);
        std::vector<Flag*> getFlags();
        bool removeFlag(int id);
        bool removeFlag(Flag* flagToRemove);
        void clearFlags();

        // getter & setter record
        int getRecord();
        void setRecord(int newRecord);
        void loadRecord();
        void writeRecord();
        void newPotencialRecord(int time);
        void destroyInstance(); // permet de détruire l'instance du singleton sans entrer dans des boucles de destruction entrainant un crash
        

    private:
        static GameManager* _instance; // variable du singleton
        GameManager(); // mise en privé du constructeur pour forcer à utiliser le singleton
        ~GameManager();

        std::mutex _mutex;   // mutex qui protège le singleton
        std::chrono::time_point<std::chrono::system_clock> _counter; // date début de la partie
        GameState _state = INIT;
        std::vector<Flag*> _flags;
        int _record;
};

#endif