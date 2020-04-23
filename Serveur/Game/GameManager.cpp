#include <iostream>
#include <stdio.h>
#include <mutex>
#include <time.h>

#include "GameManager.h"
#include "Consts.h"


// initialisation du pointeur de singleton à zéro
GameManager *GameManager::_instance = nullptr;


GameManager::GameManager() {}

// Redémarrage du timer de la partie
void GameManager::restartCounter() {
    this->_mutex.lock();
    this->_counter = clock();
    this->_mutex.unlock();
}

// Calcul du temps écoulé depuis le début de la partie
double GameManager::calculateTime() {
    return (double)(clock() - this->_counter)/CLOCKS_PER_SEC;
}

// get un flag par son id
Flag* GameManager::getFlagById(int flagId) {
    Flag* output = nullptr;

    for(Flag* flag: this->_flags) {
        if(flag->getId() == flagId) {
            output = flag;
            break;
        }
    }

    return output;
}

// regarde dans le vecteur si le flag est présent
bool GameManager::flagFounded(int id) {
    bool output = false;

    this->_mutex.lock();

    for(int flagid: this->_flagsFound) {
        if(flagid == id) {
            output = true;
            break;
        }
    }

    this->_mutex.unlock();
    return output;
}



// getter & setter de l'état de la partie
GameState GameManager::getState() {
    return this->_state;
}

void GameManager::setState(GameState newState) {
    this->_mutex.lock();
    this->_state = newState;
    this->_mutex.unlock();
}



// getter & setter vector flag
void GameManager::addFlag(Flag* f) {
    this->_mutex.lock();
    this->_flags.push_back(f);
    this->_mutex.unlock();
}

std::vector<Flag*> GameManager::getFlags() {
    return this->_flags;
}

bool GameManager::removeFlag(int idToRemove) {
    bool output = false;

    this->_mutex.lock();

    auto flag = this->_flags.begin();
	while(flag != this->_flags.end()) {
        if((*flag)->getId() == idToRemove) {
            this->_flags.erase(flag);
            std::cout << "[ClientSession " << (*flag)->getId() << "] Retiré de la liste des drapeaux à trouver." << std::endl;
            output = true;
            break;
        }
    }

    this->_mutex.unlock();
    return output;
}

bool GameManager::removeFlag(Flag* flagToRemove) {
    bool output = false;

    this->_mutex.lock();

    auto flag = this->_flags.begin();
	while(flag != this->_flags.end()) {
        if((*flag)->getId() == flagToRemove->getId()) {
            this->_flags.erase(flag);
            std::cout << "[ClientSession " << flagToRemove->getId() << "] Retiré de la liste des drapeaux à trouver." << std::endl;
            output = true;
            break;
        }
    }

    this->_mutex.unlock();
    return output;
}

void GameManager::clearFlags() {
    this->_mutex.lock();
    this->_flags.clear();
    this->_mutex.unlock();
}




// getter & setter flagsfound
void GameManager::addFlagFound(int id) {
    this->_mutex.lock();
    this->_flagsFound.push_back(id);
    this->_mutex.unlock();
}

std::vector<int> GameManager::getFlagsFound() {
    return this->_flagsFound;
}

bool GameManager::removeFlagFound(int idToRemove) {
    bool output = false;

    this->_mutex.lock();

    auto flagId = this->_flagsFound.begin();
	while(flagId != this->_flagsFound.end()) {
        if(*flagId == idToRemove) {
            this->_flagsFound.erase(flagId);
            std::cout << "[ClientSession " << *flagId << "] Retiré de la liste des drapeaux trouvés." << std::endl;
            output = true;
            break;
        }
    }

    this->_mutex.unlock();
    return output;
}

void GameManager::clearFlagsFound() {
    this->_mutex.lock();
    this->_flagsFound.clear();
    this->_mutex.unlock();
}