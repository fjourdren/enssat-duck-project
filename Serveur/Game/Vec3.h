#ifndef DEF_VEC3
#define DEF_VEC3

class Vec3 {
    public:
        Vec3();
        Vec3(double x, double y, double z);
        double getX();
        void setX(double newX);
        double getY();
        void setY(double newY);
        double getZ();
        void setZ(double newZ);
        ~Vec3();

    private:
        double _x;
        double _y;
        double _z;
};

#endif