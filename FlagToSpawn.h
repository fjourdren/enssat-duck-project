#ifndef DEF_FLAGTOSPAWN
#define DEF_FLAGTOSPAWN

#include <string>
#include <utils.h>

class FlagToSpawn {
    public:
        // on en a besoin en public pour facilment y avoir accès lors du spawn des canards
        int _id;
        std::string _type;
        std::string _sound;
        vec3 _position;
        vec3 _rotation;
        bool _found;

        FlagToSpawn(int id, std::string type, std::string sound, vec3 position, vec3 rotation, bool found);
};

#endif