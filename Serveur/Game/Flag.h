#ifndef DEF_FLAG
#define DEF_FLAG

#include <string>
#include <mutex>

#include "Vec3.h"

class Flag {
    public:
        Flag(int id, std::string type, std::string sound, Vec3 m_Position, Vec3 m_Orientation, bool found = false);
        int getId();
        void setId(int newId);
        std::string getSound();
        void setSound(std::string newSound);
        Vec3 getM_Position();
        void setM_Position(Vec3 newM_Position);
        Vec3 getM_Orientation();
        void setM_Orientation(Vec3 newM_Orientation);
        bool getFound();
        void setFound(bool newFound);

    private:
        std::mutex _mutex;   // mutex qui protège le flag
        int _id;
        std::string _type;
        std::string _sound;
        Vec3 _m_Position;
        Vec3 _m_Orientation;
        bool _found = false; // indique si le flag a déjà été trouvé
};

#endif