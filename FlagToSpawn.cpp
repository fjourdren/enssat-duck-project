#include "FlagToSpawn.h"

#include <string>
#include <utils.h>

FlagToSpawn::FlagToSpawn(int id, std::string type, std::string sound, vec3 position, vec3 rotation, bool draw) : _id(id), _type(type), _sound(sound), _position(position), _rotation(rotation), _draw(draw) {

}