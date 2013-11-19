#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include<fstream>
#include <string>
//#include "ePlayer.h"
#include "human.h"
#include "map.h"
using namespace std;
class playerManager
{
    public:
        void loadPlayers(const char* playerList,map* MAP,bool isNot2D);
        void drawAll();
        void processPlayers(Uint8 *keystates);
        float getPx();
        float getPy();
        float getAngle();
    private:
        map* curMap;
        int numEnemies;
        int curEnemy;
        bulletClass *bulletList;
        human *player;
        bool is3D;
};
#endif
