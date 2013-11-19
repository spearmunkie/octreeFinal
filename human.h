#ifndef HUMAN_H
#define HUMAN_H

#include "playerBase.h"

class human: public playerBase
{
    public:
        human(float startX=1, float startY=1,float startZ=0,const char* charInfo="config/humanInfo.txt");
        void think(const map* curMap, Uint8* keystates, bulletClass* gun);
};
#endif
