#include "human.h"
#include <math.h>
const float piover180 = 0.0174532925f;
const double DEG_TO_RAD=57.29578;

human::human(float startX, float startY,float startZ,const char* picName)
    :playerBase(startX,startY,startZ,picName)
{
}

void human::think(const map* curMap, Uint8 *keystates,bulletClass* gun)
{
    float speed=1;
    vx=0;
    vy=0;
    bool standing=true;
    if (isShooting)
    {
        standing=false;
        if (tris.doneAnim)
        {
            isShooting=false;
            tris.doneAnim=false;
            canShoot=true;
        }
    }

    if (keystates[SDLK_UP])
    {
        standing=false;
        vy=speed*sin(angle/DEG_TO_RAD);
        vx=speed*cos(angle/DEG_TO_RAD);
        if (is3D&&!isShooting)
        {
            tris.setAnimation("run");
            weapon.setAnimation("run");
        }
    }
    if (keystates[SDLK_DOWN])
    {
        standing=false;
        vy=-speed*sin(angle/DEG_TO_RAD);
        vx=-speed*cos(angle/DEG_TO_RAD);
        if (is3D&&!isShooting)
        {
            tris.setAnimation("run");
            weapon.setAnimation("run");
        }
    }
    if (keystates[SDLK_SPACE]&&canShoot)
    {
        standing=false;
        float gspeed=4;
        float gvy=gspeed*sin(angle/DEG_TO_RAD);
        float gvx=gspeed*cos(angle/DEG_TO_RAD);

        float tspeed=2;
        float tvy=tspeed*sin(angle/DEG_TO_RAD);
        float tvx=tspeed*cos(angle/DEG_TO_RAD);
        canShoot=false;
        isShooting=true;
        if (is3D)
        {
            tris.setAnimation("attack");
            weapon.setAnimation("attack");
        }
        gun->addBullet(Vec3f(x+tvx,y+tvy,z*16),Vec3f(gvx,gvy,0));
    }
    if (keystates[SDLK_RIGHT])
       angle+=5;
    if (keystates[SDLK_LEFT])
       angle-=5;
    if (is3D&&standing)
    {
        tris.setAnimation("stand");
        weapon.setAnimation("stand");
    }
    collide(curMap);
}
