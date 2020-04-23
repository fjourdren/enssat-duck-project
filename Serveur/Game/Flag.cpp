#include <mutex>

#include "Flag.h"
#include "Vec3.h"

Flag::Flag(int id, std::string type, std::string sound, Vec3 m_Position, Vec3 m_Orientation, bool found) : _id(id), _type(type), _sound(sound), _m_Position(m_Position), _m_Orientation(m_Orientation), _found(found) {

}


// getter & setter
int Flag::getId() {
    return this->_id;
}

void Flag::setId(int newId) {
    this->_mutex.lock();
    this->_id = newId;
    this->_mutex.unlock();
}



std::string Flag::getSound() {
    return this->_sound;
}

void Flag::setSound(std::string newSound) {
    this->_mutex.lock();
    this->_sound = newSound;
    this->_mutex.unlock();
}



Vec3 Flag::getM_Position() {
    return this->_m_Position;
}

void Flag::setM_Position(Vec3 newM_Position) {
    this->_mutex.lock();
    this->_m_Position = newM_Position;
    this->_mutex.unlock();
}



Vec3 Flag::getM_Orientation() {
    return this->_m_Orientation;
}

void Flag::setM_Orientation(Vec3 newM_Orientation) {
    this->_mutex.lock();
    this->_m_Orientation = newM_Orientation;
    this->_mutex.unlock();
}



bool Flag::getFound() {
    return this->_found;
}

void Flag::setFound(bool newFound) {
    this->_mutex.lock();
    this->_found = newFound;
    this->_mutex.unlock();
}