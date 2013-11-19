#ifndef PLAYERBASE_H
#define PLAYERBASE_H
#include "map.h"
#include "MD2.h"
#include "bulletClass.h"
class playerBase
{
    public:
        playerBase();
        playerBase(float startX, float startY,float startZ,const char* charInfo);
        void draw();
        float getX();
        float getY();
        float getZ();
        float getAngle();
        float getHealth();
        void animate();
        void setIs3D(bool isNot2D);
        void generateTextures();
        void load(float startX, float startY,float startZ,const char* charInfo);
    protected:
        void collide(const map* curMap);
        bool isColliding(float vx, float vy,const map* curMap);
        bool is3D,isShooting,canShoot;
        float x,y,z,vx,vy,angle,health;
        sprite pic;
        MD2Model tris,weapon;
        string trisTex,weaponTex;
};
#endif
