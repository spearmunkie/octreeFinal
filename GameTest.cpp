#include "Game.h"
#include <iostream>
//#include<conio.h>
using namespace std;

int main ( int argc, char** argv )
{
    int fps,drawDist;
    char ch;
    Game curGame(1920,1080,32);
    cout<<"What fps should it be clocked at?\n";
    cin>>fps;
    cout<<"What should the draw distance be?\n";
    cin>>drawDist;
    curGame.setDrawDist(drawDist);

    //cout<<"\nUse the arrow keys to move\nSpace bar to shoot, 'o' to enable/diable the octree\nClick to see the bounding boxes when the octree is enabled\nPress 'f' to free the camera\nUse the mouse and 'w','s' keys to move the camera in free mode\n\nPress any key to continue\n";

    //Game curGame(800,600,32);


    curGame.init(true);
    curGame.load("blank");
    curGame.setFpsCAP(fps);
    curGame.mainLoop();
    return 0;
}
