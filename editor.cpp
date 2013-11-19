#include "map.h"
#include "sprite.h"
#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP     32

SDL_Surface *screen;
void SDLinit()
{
    int videoFlags;
    videoFlags = SDL_SWSURFACE;
    //videoFlags |= SDL_FULLSCREEN;
    if(SDL_Init( SDL_INIT_EVERYTHING ) == -1)
        exit(1);

    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, videoFlags );

    if( screen == NULL )
        exit(1);

    SDL_WM_SetCaption( "Editor", NULL );
}

void screenUpdate()
{
    if( SDL_Flip( screen ) == -1 )
        SDL_Quit();
}

void drawRect(int x1, int y1, int x2, int y2)
{
    SDL_Rect rec;
    rec.x = x1;
    rec.y = y1;
    rec.w = x2 - x1 + 1;
    rec.h = y2 - y1 + 1;
    Uint32 colorSDL = SDL_MapRGB(screen->format, 0, 0, 0);
    SDL_FillRect(screen, &rec, colorSDL);
}

struct mouseDef{int x,y; bool b;};

int main (int argc, char *argv[])
{
    char answer;
    map curEdit(false),mPallet(false),oPallet(false);
    cout<<"Would you like to load a map?[y/n]"<<endl;
    cin>>answer;

    while(answer!='y' &&answer!='n')
        cin>>answer;

    if (answer=='y')
    {
        string fileName;
        cout<<"Please enter the map name\n";
        cin>>fileName;
        SDLinit();
        curEdit.load(fileName.c_str());
    }
    else
    {
        int x,y,z;
        cout<<"Please enter the dimensions of the world. (must be at least 25 by 25 by 1\n";
        cin>>x>>y>>z;
        SDLinit();
        if (x<25) x=25;
        if(y<25) y=25;
        if (z<1)z=1;
        curEdit.blankMap(x,y,z);
    }
    curEdit.setDx(5*16);
    curEdit.setDy(3*16);
    curEdit.toggleMatShow();

    mPallet.load("config/mPallet.txt");
    mPallet.setDx(32*16);
    mPallet.setDy(5*16);
    mPallet.setTileSize(20);

    oPallet.load("config/oPallet.txt");
    oPallet.setDx(32*16);
    oPallet.setDy(10*16);
    oPallet.setTileSize(20);

    int curZ=0,curM=0,curT=0,curP=0,tmp;
    int scrollX=curEdit.getDx(),scrollY=curEdit.getDy(),locX=curEdit.getDx(),locY=curEdit.getDy();
    bool done=false;

    Uint8 *keystates;
    SDL_Event event;
    mouseDef m;
    m.b=false;
    keystates = SDL_GetKeyState(0);
    while (!done)
    {
        while ( SDL_PollEvent( &event ))
        {
            switch( event.type )
            {
                case SDL_MOUSEMOTION:
                    m.y=event.motion.y;
                    m.x=event.motion.x;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.state==SDL_BUTTON_LEFT)
                        m.b=true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.state!=SDL_BUTTON_LEFT)
                        m.b=false;
                    break;
                case SDL_KEYDOWN:
                    switch( event.key.keysym.sym )
                    {
                        case SDLK_KP_MINUS:
                            curZ--;
                            if (curZ<0) curZ=0;
                        break;
                        case SDLK_KP_PLUS:
                            curZ++;
                            if (curZ>curEdit.getMaxZ()-1) curZ=curEdit.getMaxZ()-1;
                        break;
                        default:
                        break;
                    }
                default:
                    break;
            }
        }

        if (keystates[SDLK_ESCAPE])//exit if escape was pressed
            done=true;

        if (m.x>=locX&&m.x<=locX+16*3&&m.y>=locY&&m.y<=locY+25*16)
            scrollX+=4;
        if (m.x>=locX+22*16&&m.x<=locX+25*16&&m.y>=locY&&m.y<=locY+25*16)
            scrollX-=4;
        if (m.y>=locY&&m.y<=locY+16*3&&m.x>=locX&&m.x<=locX+25*16)
            scrollY+=4;
        if (m.y>=locY+22*16&&m.y<=locY+25*16&&m.y>=locX&&m.x<=locX+25*16)
            scrollY-=4;

        if (scrollY>locY)
            scrollY=locY;
        if (scrollY+curEdit.getMaxY()*16<locY+25*16)
            scrollY=locY+25*16-curEdit.getMaxY()*16;
        if (scrollX>locX)
            scrollX=locX;
        if (scrollX+curEdit.getMaxX()*16<locX+25*16)
            scrollX=locX+25*16-curEdit.getMaxX()*16;
        if (m.b)
        {
            tmp=oPallet.getObj((m.x-oPallet.getDx())/20,(m.y-oPallet.getDy())/20,0);
            curT= (tmp==-1)? curT:tmp;
            tmp=oPallet.getProperty((m.x-oPallet.getDx())/20,(m.y-oPallet.getDy())/20,0);
            curP= (tmp==-1)? curP:tmp;
            tmp=mPallet.getMaterial((m.x-mPallet.getDx())/20,(m.y-mPallet.getDy())/20,0);
            curM= (tmp==-1)? curM:tmp;

            if (m.x>=locX&&m.x<=locX+16*25&&m.y>=locY&&m.y<=locY+16*25)
            {
                curEdit.setProperty( (m.x-curEdit.getDx())/16,(m.y-curEdit.getDy())/16,curZ,curP);
                curEdit.setMaterial( (m.x-curEdit.getDx())/16,(m.y-curEdit.getDy())/16,curZ,curM);
                curEdit.setObj     ( (m.x-curEdit.getDx())/16,(m.y-curEdit.getDy())/16,curZ,curT);
            }
        }
        curEdit.setDx(scrollX);
        curEdit.setDy(scrollY);
        curEdit.draw(curZ);
        drawRect(0,0,SCREEN_WIDTH,locY-1);
        drawRect(0,0,locX-1,SCREEN_HEIGHT);
        drawRect(0,locY+25*16,SCREEN_WIDTH,SCREEN_HEIGHT);
        drawRect(locX+25*16,0,SCREEN_WIDTH,SCREEN_HEIGHT);
        mPallet.draw(0);
        oPallet.draw(0);
        screenUpdate();
    }
    SDL_WM_IconifyWindow();
    cout<<"Would you like to save this map[y/n]?\n";
    do
    {
        cin>>answer;
    }
    while (answer!='y'&&answer!='n');

    if (answer=='y')
    {
        string fileName;
        cout<<"Please enter the name you would like to save as.\n";
        cin>>fileName;
        curEdit.save(fileName.c_str());
    }
    return 0;
}
