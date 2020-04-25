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



std::string Flag::getType() {
    std::string output;

    this->_mutex.lock();
    output = this->_type;
    this->_mutex.unlock();

    return output;
}

void Flag::setType(std::string newType) {
    this->_mutex.lock();
    this->_type = newType;
    this->_mutex.unlock();
}



std::string Flag::getSound() {
    std::string output;

    this->_mutex.lock();
    output = this->_sound;
    this->_mutex.unlock();

    return output;
}

void Flag::setSound(std::string newSound) {
    this->_mutex.lock();
    this->_sound = newSound;
    this->_mutex.unlock();
}



Vec3 Flag::getM_Position() {
    Vec3 output;

    this->_mutex.lock();
    output = this->_m_Position;
    this->_mutex.unlock();

    return output;
}

void Flag::setM_Position(Vec3 newM_Position) {
    this->_mutex.lock();
    this->_m_Position = newM_Position;
    this->_mutex.unlock();
}



Vec3 Flag::getM_Orientation() {
    Vec3 output;

    this->_mutex.lock();
    output = this->_m_Orientation;
    this->_mutex.unlock();

    return output;
}

void Flag::setM_Orientation(Vec3 newM_Orientation) {
    this->_mutex.lock();
    this->_m_Orientation = newM_Orientation;
    this->_mutex.unlock();
}



bool Flag::getFound() {
    bool output;

    this->_mutex.lock();
    output = this->_found;
    this->_mutex.unlock();

    return output;
}

void Flag::setFound(bool newFound) {
    this->_mutex.lock();
    this->_found = newFound;
    this->_mutex.unlock();
}