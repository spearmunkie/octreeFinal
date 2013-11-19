#ifndef BULLETCLASS_H
#define BULLETCLASS_H

#include "vec3f.h"
#include "particle.h"
#include "map.h"
#include <math.h>
#include <vector>

using namespace std;

struct bullet
{
    Vec3f pos,velocity;
};

class bulletClass
{
    public:
        bulletClass(bool isNot2D);
        void addBullet(Vec3f p,Vec3f v);
        void run(map* cur);
        void draw(Vec3f camPos);
        int getNumBullets();
        bool playerHit(float x,float y, float z,float radius);
    private:
        vector<bullet> bullets;
        bool is3D;
        particleClass blood,explosion;

};
#endif]
