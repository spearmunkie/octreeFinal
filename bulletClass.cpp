#include "bulletClass.h"

bulletClass::bulletClass(bool isNot2D)
{
    is3D=isNot2D;
    if (is3D)
    {
        explosion.setColor(Vec3f(255,200,0),Vec3f(255,255,0));
        explosion.setForceVec(Vec3f(0,-0.001,0));
        explosion.setLifeSpan(5);
        explosion.setMaxParticle(150);
        /*explosion.startAt(Vec3f((float)(-1*16+7.5)/16*-8+4,-4,(float)(-1*16+7.5)/16*-8));
        explosion.restart();*/

        /*blood.setColor(Vec3f(0,0,0),Vec3f(255,0,0));
        blood.setForceVec(Vec3f(0,-0.001,0));
        blood.setLifeSpan(15);
        blood.setMaxParticle(150);
        blood.startAt(Vec3f((float)(-1*16+7.5)/16*-8+4,-4,(float)(-1*16+7.5)/16*-8));
        blood.restart();*/
    }
}

void bulletClass::addBullet(Vec3f p,Vec3f v)
{
    bullet tmp;
    tmp.pos=p;
    tmp.velocity=v;
    bullets.push_back(tmp);
}

void bulletClass::run(map* cur)
{
    vector<int> toRemove;
    for(int x=0;x<bullets.size();x++)
    {
        int tmp=cur->getProperty((bullets[x].pos[0]+bullets[x].velocity[0])/16,
                                  (bullets[x].pos[1]+bullets[x].velocity[1])/16,
                                  (bullets[x].pos[2]+bullets[x].velocity[2])/16);
        if (tmp!=0&&tmp!=3)
        {
            toRemove.push_back(x);
            explosion.startAt(Vec3f((float)(bullets[x].pos[0])/16*-8+4,bullets[x].pos[2]-4,(float)(bullets[x].pos[1])/16*-8));
            explosion.restart();
        }
        else
            bullets[x].pos+=bullets[x].velocity;
    }
    for(int x=0;x<toRemove.size();x++)
    {
        for(int y=toRemove[x];y<bullets.size()-1;y++)
        {
            bullets[y]=bullets[y+1];
        }
        bullets.pop_back();
    }
    blood.run();
    explosion.run();
}

void bulletClass::draw(Vec3f camPos)
{
    for(int x=0;x<bullets.size();x++)
        if(is3D)
            drawP(Vec3f((float)(bullets[x].pos[0])/16*-8+4,bullets[x].pos[2]-4,(float)(bullets[x].pos[1])/16*-8),0.5,Vec3f(0.5,0.01,0.0),1);

    blood.setCam(camPos);
    blood.draw();
    explosion.setCam(camPos);
    explosion.draw();
}

float dist(float x1,float y1,float x2,float y2)
{
    return sqrt( (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

bool bulletClass::playerHit(float x,float y, float z,float radius)
{
    vector<int> toRemove;
    bool answer=false;
    for(int x=0;x<getNumBullets();x++)
        if ((int)bullets[x].pos[2]==z)
            if (dist(bullets[x].pos[0],bullets[x].pos[1],x,y<radius))
            {
                toRemove.push_back(x);
                answer= true;
                break;
            }
    for(int x=0;x<toRemove.size();x++)
    {
        for(int y=toRemove[x];y<bullets.size()-1;y++)
        {
            bullets[y]=bullets[y+1];
        }
        bullets.pop_back();
    }
    return false;
}

int bulletClass::getNumBullets()
{
    return bullets.size();
}
