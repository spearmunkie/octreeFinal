#include "Game.h"
#include <math.h>
SDL_Surface *screen;
const double DEG_TO_RAD=57.29578;
Game::Game(int w,int h,int bpp)
{
    newTime=0;
    oldTime=0;
    SCREEN_WIDTH=w;
    SCREEN_HEIGHT=h;
    SCREEN_BPP=bpp;
    nReg=0;
    oReg=0;
    fpsCAP=30;
    drawDist=100;
}

void Game::init(bool isNot2D)
{
    screen=NULL;
    is3D=isNot2D;
    level=new map(isNot2D);
    curBullets=new bulletClass(isNot2D);
    useOct=true;
    if (is3D)
    {
        int videoFlags;
        videoFlags  = SDL_OPENGL;
        videoFlags |= SDL_GL_DOUBLEBUFFER;
        videoFlags |= SDL_HWPALETTE;
        videoFlags |= SDL_FULLSCREEN;
        screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
                    videoFlags );
        glShadeModel (GL_SMOOTH);

        glEnable (GL_DEPTH_TEST);
        glEnable( GL_TEXTURE_2D );

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-40,40,-40,40,-40,40);
        player=new human;
        player->setIs3D(isNot2D);
        player->load(1,1,0,"config/humanInfo.txt");
        resize(SCREEN_WIDTH,SCREEN_HEIGHT);
        freeCam=false;
        SDL_ShowCursor(SDL_DISABLE);

    }
    else
    {
        int videoFlags;
        videoFlags = SDL_SWSURFACE;
        videoFlags |= SDL_FULLSCREEN;
        if(SDL_Init( SDL_INIT_EVERYTHING ) == -1)
            exit(1);

        screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, videoFlags );

        if( screen == NULL )
            exit(1);

        SDL_WM_SetCaption( "Editor", NULL );
        player=new human;
        player->setIs3D(isNot2D);
    }
    fps=0;
    ff=0;
}

void Game::resize(int width, int height)
{
    if (is3D)
    {
        GLfloat ratio;
        if ( height == 0 )
            height = 1;
        ratio = ( GLfloat )width / ( GLfloat )height;
        glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );

        glMatrixMode( GL_PROJECTION );
        glLoadIdentity( );

        gluPerspective(60.0, ratio, 0.01f, drawDist );
        glMatrixMode( GL_MODELVIEW );

        glLoadIdentity();

        glEnable( GL_TEXTURE_2D );
        glShadeModel (GL_SMOOTH);

        glEnable (GL_DEPTH_TEST);
    }
}

void Game::load(const char* levelFile)
{
    ifstream fin(levelFile);
    string fontName,mapName;
    fin>>fontName>>mapName;
    fontName="images/font.tga";
    mapName="config/test.txt";

    level->load(mapName.c_str());
    if (is3D)
    {
        polyMngr.loadMap(level);
        polyMngr.buildOctree();
        polyMngr.buildTextures();
        player->generateTextures();
        loadP("images/circle.bmp","images/circlealpha.bmp");
        text.BuildFont(fontName.c_str());
    }
}

void Game::draw()
{
    if (is3D)
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        float vx=15*sin((player->getAngle()-90)/DEG_TO_RAD);
        float vy=15*cos((player->getAngle()-90)/DEG_TO_RAD);
        Vec3f camPos (player->getX()/16*-8-vx+4,player->getZ()*16+3,player->getY()/16*-8+vy);
        if (!freeCam)
            cam.setCamera(Vec3f(player->getX()/16*-8-vx+4,player->getZ()*16+3,player->getY()/16*-8+vy),
                          Vec3f(player->getX()/16*-8+4,player->getZ()*16,player->getY()/16*-8),
                          Vec3f(0,1,0));
        cam.setView();
        if(useOct)
        {
            curFrustum.UpdateFrustum();
            polyMngr.drawOctree(&curFrustum);
        }
        else
            polyMngr.drawBruteForce();
        player->draw();
        curBullets->draw(camPos);
        text.PrintText(0,0,1,"FPS: %d",fps);
        text.PrintText(0,16,0,"Num Drawn: %d",polyMngr.getNumdrawn());
        text.PrintText(0,32.5,0,"Num bullets: %d",curBullets->getNumBullets());
        if (freeCam)
            text.PrintText(0,48,0,"Cam Mode:Free");
        else
            text.PrintText(0,48,0,"Cam Mode:CharFixed");
        if(useOct)
            text.PrintText(0,64,0,"Octree in use");

        SDL_GL_SwapBuffers();
    }
    else
    {
        level->draw(player->getZ());
        player->draw();
        SDL_Flip( screen );
    }
}

void Game::mainLoop()
{
    bool done=false;
    keystates = SDL_GetKeyState(0);

    while (!done)
    {
        nReg=SDL_GetTicks();
        if(nReg-oReg>1000/fpsCAP)
        {
            while ( SDL_PollEvent( &event ) )
            {
                switch( event.type )
                {
                    case SDL_MOUSEBUTTONDOWN:
                        polyMngr.toggleOctLineDraw();
                        break;
                    case SDL_MOUSEMOTION:
                        mX=event.motion.x;
                        mY=event.motion.y;
                        break;
                    case SDL_KEYDOWN:
                        switch( event.key.keysym.sym )
                        {
                            case SDLK_f:
                                freeCam=!freeCam;
                                break;
                            case SDLK_o:
                                useOct=!useOct;
                                break;
                            default:
                                break;
                        }
                    default:
                        break;
                }
            }
            if (freeCam)
            {
                if (keystates[SDLK_w])
                    cam.moveCamera(0.5);
                if (keystates[SDLK_s])
                    cam.moveCamera(-0.5);
                cam.mouseRot(SCREEN_WIDTH/2-mX,SCREEN_HEIGHT/2-mY);
            }
            SDL_WarpMouse(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
            if (keystates[SDLK_ESCAPE])
                done=true;

            curBullets->run(level);
            player->think(level,keystates,curBullets);
            draw();
            oReg=nReg;
            ff++;
        }
        newTime=SDL_GetTicks();
        if (newTime-oldTime>1000){fps=ff;ff=0;oldTime=newTime;}
    }
}
