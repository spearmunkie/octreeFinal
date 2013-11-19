#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <fstream>
#include "font.h"
#include "map.h"
#include "polygonManager.h"
#include "frustum.h"
#include "human.h"
#include "bulletClass.h"
#include "camera.h"
using namespace std;
class Game
{
    public:
        Game(int w,int h,int bpp);
        void init(bool isNot2D);
        void resize(int width, int height);
        void load(const char* level);
        void draw();
        void mainLoop();
        void useBruteForce() {useOct=false;}
        void useOctree() {useOct=true;}
        void setFpsCAP(int t){fpsCAP=t;}
        void setDrawDist(int t){drawDist=t;}
    private:
        font text;
        bulletClass* curBullets;
        polygonManager polyMngr;
        map *level;
        SDL_Event event;
        Uint8 *keystates;
        bool is3D,useOct;
        int nReg,oReg;
        frustum curFrustum;
        human* player;
        bool freeCam;
        int mX,mY;
        camera cam;
        int SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,oldTime,newTime,ff,fps,fpsCAP,drawDist;
};
#endif
