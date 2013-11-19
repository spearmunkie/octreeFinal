#ifndef PARTICLE_H
#define PARTICLE_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "vec3f.h"
#include "imageloader.h"

void loadP(const char*image,const char* alphaImage);
void drawP(Vec3f pos,float size,Vec3f colour,float alpha);

struct Particle
{
	Vec3f pos;
	Vec3f velocity;
	Vec3f color;
	float timeAlive;
	float lifeSpan;
	float dist;
};

class particleClass
{
    public:
        particleClass() {particles = NULL;}
        void setForceVec(Vec3f tmpForce);
        void run();
        void draw();
        void setMaxParticle(int tmp);
        void setLifeSpan(float lifeSpan);
        void setCam(Vec3f camPos);
        void startAt(Vec3f toStartAt);
        void resetParticle(int x);
        void restart();
        bool end();
        void setColor(Vec3f low,Vec3f high);
    private:
        Particle* particles;
        int maxParticle;
        Vec3f startPos,cam,force,colUpper,colLower;
        float lifeSpan,newTime,oldTime,alive;
};
#endif
