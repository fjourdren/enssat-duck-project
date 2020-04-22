#include "Vec3.h"

// classe très simple pour stocker les vecteurs 3 côté serveur (sans avoir à importer de lourdes bibliothèques)
Vec3::Vec3() {

}

Vec3::Vec3(double x, double y, double z) : _x(x), _y(y), _z(z) {

}


// getter & setter
double Vec3::getX() {
    return this->_x;
}

void Vec3::setX(double newX) {
    this->_x = newX;
}


double Vec3::getY() {
    return this->_y;
}

void Vec3::setY(double newY) {
    this->_y = newY;
}


double Vec3::getZ() {
    return this->_z;
}

void Vec3::setZ(double newZ) {
    this->_z = newZ;
}