#include "particle.h"
#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include<stdlib.h>//for random numbers

using namespace std;

GLuint tex;
bool compareParticles(Particle particle1, Particle particle2)
{
	return particle1.dist > particle2.dist;
}

float randFloat(float min,float max)
{
    if (min==max)
        return min;
    return ( (float)(rand()%(int)(max-min+1)+min) +
             ((float)rand() / ((float)RAND_MAX + 1)));
}

int randInt(int min,int max)
{
    return  (rand()%(int)(max-min+1)+min);
}

void loadP(const char*img,const char* alphaImage)
{
    Image *pic=iloadBMP(alphaImage);
    Image *alpha=iloadBMP(alphaImage);

    char* pxls = new char[pic->width * pic->height * 4];
	for(int y = 0; y < pic->height; y++)
	{
		for(int x = 0; x < pic->width; x++)
		{
			for(int j = 0; j < 3; j++)
			{
				pxls[4 * (y * pic->width + x) + j] =
					pic->pixels[3 * (y * pic->width + x) + j];
			}
			pxls[4 * (y * pic->width + x) + 3] =
				alpha->pixels[3 * (y * pic->width + x)];
		}
	}
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGBA,
				 pic->width, pic->height,
				 0,
				 GL_RGBA,
				 GL_UNSIGNED_BYTE,
				 pxls);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

void drawP(Vec3f pos,float size,Vec3f colour,float alpha)
{
    float modelview[16];
	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	Vec3f x(modelview[0], modelview[4], modelview[8]);
    Vec3f y(modelview[1], modelview[5], modelview[9]);

    Vec3f a = pos + ((-x - y)*size);
    Vec3f b = pos + ((x - y)*size);
    Vec3f c = pos + ((x + y)*size);
    Vec3f d = pos + ((-x + y)*size);

	glLoadMatrixf(modelview);

    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(colour[0],colour[1],colour[2],alpha);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(a[0] , a[1] , a[2]);
        glTexCoord2f(0, 1);
        glVertex3f(b[0] , b[1] , b[2]);
        glTexCoord2f(1, 1);
        glVertex3f(c[0] , c[1], c[2]);
        glTexCoord2f(1, 0);
        glVertex3f(d[0], d[1], d[2]);
    glEnd();
    glColor4f(1,1,1,1);
    glPopMatrix();
}

void particleClass::setForceVec(Vec3f tmpForce)
{
    force=tmpForce;
}

void particleClass::run()
{
    if (alive<=lifeSpan)
    {
        for(int x=0;x<maxParticle;x++)
        {
            particles[x].velocity+=force;
            particles[x].pos+=particles[x].velocity;
            particles[x].timeAlive++;
            if (particles[x].timeAlive>=particles[x].lifeSpan)
                resetParticle(x);
        }
    }
    alive+=0.1;
}

void particleClass::draw()
{
    if (!end())
    {
        vector<Particle> sorter(particles,particles+maxParticle);
        for(int x=0;x<maxParticle;x++)
            sorter[x].dist=sqrt((cam[0]-sorter[x].pos[0])*(cam[0]-sorter[x].pos[0]) +
                                (cam[1]-sorter[x].pos[1])*(cam[1]-sorter[x].pos[1]) +
                                (cam[2]-sorter[x].pos[2])*(cam[2]-sorter[x].pos[2]) );

        sort(sorter.begin(),sorter.end(),compareParticles);
        for(int x=0;x<maxParticle;x++)
            drawP(sorter[x].pos,0.2,sorter[x].color,1-sorter[x].timeAlive/sorter[x].lifeSpan);
    }
}
void particleClass::setMaxParticle(int tmp)
{
    maxParticle=tmp;
    if (particles!=NULL)
        delete [] particles;
    particles=new Particle[maxParticle];
    for(int x=0;x<maxParticle;x++)
        resetParticle(x);
}

void particleClass::setLifeSpan(float lfSpan)
{
    lifeSpan=lfSpan;
}

void particleClass::startAt(Vec3f toStartAt)
{
    startPos=toStartAt;
}

void particleClass::resetParticle(int x)
{
    particles[x].pos=startPos;
    particles[x].velocity=Vec3f(randFloat(-1,1),
                                randFloat(-1,1),
                                randFloat(-1,1));
    particles[x].color=Vec3f(randInt(colLower[0],colUpper[0]),
                             randInt(colLower[1],colUpper[1]),
                             randInt(colLower[2],colUpper[2]));
    particles[x].color/=256;
    particles[x].velocity=particles[x].velocity.normalize();
    particles[x].velocity*=randFloat(0,1)/20;
    particles[x].timeAlive=0;
    particles[x].lifeSpan=randInt(1,100);
    particles[x].velocity+=force;
    particles[x].pos+=particles[x].velocity;
}

void particleClass::restart()
{
    alive=0;
}

bool particleClass::end()
{
    return alive>lifeSpan;
}

void particleClass::setColor(Vec3f low,Vec3f high)
{
    colUpper=high;
    colLower=low;
}

void particleClass::setCam(Vec3f camPos)
{
    cam=camPos;
}
