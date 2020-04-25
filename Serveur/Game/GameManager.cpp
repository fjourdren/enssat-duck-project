#include <iostream>
#include <fstream>
#include <stdio.h>
#include <mutex>
#include <time.h>
#include <unistd.h>
#include <chrono>
#include <ctime>

#include "GameManager.h"
#include "Consts.h"


// initialisation du pointeur de singleton à zéro
GameManager *GameManager::_instance = nullptr;


GameManager::GameManager() {}

// Redémarrage du timer de la partie
void GameManager::restartCounter() {
    this->_mutex.lock();
    this->_counter = std::chrono::system_clock::now();
    this->_mutex.unlock();
}

// Calcul du temps écoulé depuis le début de la partie
int GameManager::calculateTime() {
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    int elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds> (end - this->_counter).count();
    return elapsed_seconds;
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

// récupère le nombre de flag trouvé
unsigned int GameManager::getNbFlagsFound() {
    int output = 0;

    for(Flag* flag: this->_flags) {
        if(flag->getFound()) {
            output = output + 1;
        }
    }

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



// getter & setter record
int GameManager::getRecord() {
    int output;

    this->_mutex.lock();
    output = this->_record;
    this->_mutex.unlock();

    return output;
}

void GameManager::setRecord(int newRecord) {
    this->_mutex.lock();
    this->_record = newRecord;
    this->_mutex.unlock();
}

void GameManager::loadRecord() {
    this->_mutex.lock();

    std::fstream recordFile("record.txt", std::ios_base::in);

    int a;
    while (recordFile >> a) {
        this->_record = a;
        std::cout << "[Utils] Chargement du record : " << a << std::endl;
    }

    recordFile.close();

    this->_mutex.unlock();
}

void GameManager::writeRecord() {
    this->_mutex.lock();

    std::ofstream recordFile;
    recordFile.open ("record.txt");
    recordFile << this->_record;
    recordFile.close();

    this->_mutex.unlock();
}

void GameManager::newPotencialRecord(int time) {
    GameManager* gm = GameManager::getinstance();
    gm->loadRecord();

    // si il n'y a pas de record précédent
    if(this->_record == -1 || this->_record == NULL) {
        this->_record = time;

        // écriture du record
        gm->writeRecord();
    } else { // si un record existe
        if(time < this->_record) { // si le record est supérieur au temps réalisé
            this->_record = time;

            // écriture du record
            gm->writeRecord();
        }
    }
}