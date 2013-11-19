#include <SDL.h>
#include <SDL/SDL_opengl.h>
#include "map.h"
#include "font.h"
#include "polygonManager.h"
#include "Game.h"
#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT  768
#define SCREEN_BPP     32

map tmp(true);
polygonManager mngr;

frustum cur;
font text;
SDL_Surface* screen=NULL;
void init()
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
    //glEnable (GL_LIGHTING);
    //glEnable (GL_LIGHT0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho(-40,40,-40,40,-40,40);
}

int resizeWindow( int width, int height )
{
    //when we resize
    GLfloat ratio;
    if ( height == 0 )
        height = 1;
    //set the viewPort
    ratio = ( GLfloat )width / ( GLfloat )height;
    glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    gluPerspective(60.0, ratio, 0.01f, 75.0f );
    glMatrixMode( GL_MODELVIEW );

    glLoadIdentity();

    glEnable( GL_TEXTURE_2D );
    //glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
    glShadeModel (GL_SMOOTH);

    glEnable (GL_DEPTH_TEST);
    //glEnable (GL_LIGHTING);
    //glEnable (GL_LIGHT0);
    text.BuildFont("images/font.tga");
    //text.SetSize(16,16);
    return 1;
}
/**/

void draw()
{
    static float oldTime=0.0;
    static float newTime=1001.0;
    int fps;
    static int ff;
    static float ang=25.0;
    ang+=-0.05;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(ang*1, 0.0f, 1.0f,0.0f);
    float px=17*16+7.5,py=(17-ang)*16+7.5,pz=ang;
    glTranslatef( (float)-ang/5+16, (float)pz, (float)-ang/10-16 );//move the camera
    cur.UpdateFrustum();
    mngr.drawOctree(&cur);
    //mngr.drawBruteForce();
    ff++;
    newTime=SDL_GetTicks();
    if (newTime-oldTime>1000) {fps=ff; ff=0;oldTime=newTime;}
    text.PrintText(5,5,0,"FPS: %d",fps);
    text.PrintText(5,25,0,"Num Drawn: %d",mngr.getNumdrawn());
    //cout<<"fps= "<<fps<<endl;
    SDL_GL_SwapBuffers();
}

int main ( int argc, char** argv )
{
    bool done=false;
    SDL_Event event;
    init();
    resizeWindow(SCREEN_WIDTH,SCREEN_HEIGHT);
    tmp.load("config/test.txt");
    mngr.loadMap(&tmp);
    mngr.buildTextures();
    mngr.buildOctree();

    while (!done)
    {
        while ( SDL_PollEvent( &event ) )
		{
		    switch( event.type )
		    {
                case SDL_KEYDOWN:
                    done = true;
                case SDL_MOUSEBUTTONDOWN:
                    mngr.toggleOctLineDraw();
		    }
		}
		draw();
    }
    SDL_FreeSurface(screen);
    return 0;
}
