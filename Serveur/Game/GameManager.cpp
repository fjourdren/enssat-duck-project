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

    this->_mutex.lock();
    for(Flag* flag: this->_flags) {
        if(flag->getId() == flagId) {
            output = flag;
            break;
        }
    }
    this->_mutex.unlock();

    return output;
}



// getter & setter de l'état de la partie
GameState GameManager::getState() {
    GameState output;
    
    this->_mutex.lock();
    output = this->_state;
    this->_mutex.unlock();

    return output;
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
    std::vector<Flag*> output;
    
    this->_mutex.lock();
    output = this->_flags;
    this->_mutex.unlock();

    return output;
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