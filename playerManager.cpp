#include "playerManager.h"

void playerManager::loadPlayers(const char* playerList,map* MAP,bool isNot2D)
{
    curMap=MAP;
    is3D=isNot2D;
    ifstream fin(playerList);
    string playerInfo,enemyInfo;

    fin>>playerInfo>>enemyInfo;
    float px,py,pz;
    fin>>px>>py>>pz;

    if (player!=NULL)
        delete player;

    player->setIs3D(is3D);
    player->load(px,py,pz,playerInfo.c_str());
    if (is3D)
        player->generateTextures();

}

void playerManager::drawAll()
{
    player->draw();
}

void playerManager::processPlayers(Uint8 *keystates)
{
    player->think(curMap,keystates,bulletList);
    bulletList->playerHit(player->getX(),player->getY(),player->getZ(),2);
}
