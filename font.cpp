#include "font.h"

void font::loadTexture(const char* name,GLuint &texture)
{
    SDL_Surface *TextureImage;
    TextureImage=IMG_Load(name);
    if (TextureImage!=NULL)
    {
	    glGenTextures( 1, &texture);
	    glBindTexture( GL_TEXTURE_2D, texture);
	    glTexImage2D( GL_TEXTURE_2D, 0, 3,
                    TextureImage->w,
                    TextureImage->h,
                    0, GL_BGRA,
                    GL_UNSIGNED_BYTE,
                    TextureImage->pixels );

	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
    SDL_FreeSurface( TextureImage);
}
void font::BuildFont(const char *file)          // Build Our Font Display List
{
    int loop1;
    float cx, cy,cx2,cy2;
    loadTexture(file,texID);

    base=glGenLists(256);                          // Creating 256 Display Lists
    glBindTexture(GL_TEXTURE_2D, texID); // Select Our Font Texture
    for (loop1=0; loop1<256; loop1++)              // Loop Through All 256 Lists
    {
        cx=(float)(loop1%16)/16.0f;                  // X Position Of Current Character
        cy=(float)(loop1/16)/16.0f;                  // Y Position Of Current Character
        cx2=cx+0.065;
        cy2=cy+0.065;

        glNewList(base+loop1,GL_COMPILE);            // Start Building A List
        glBegin(GL_QUADS);                           // Use A Quad For Each Character
            glTexCoord2f(cx,cy);glVertex2i(0,0);                           // Vertex Coord (Top Left)
            glTexCoord2f(cx2,cy);glVertex2i(16,0);                          // Vertex Coord (Top Right)
            glTexCoord2f(cx2,cy2);glVertex2i(16,16);                         // Vertex Coord (Bottom Right)
            glTexCoord2f(cx,cy2);glVertex2d(0,16);                          // Vertex Coord (Bottom Left)
        glEnd();                                     // Done Building Our Quad (Character)
        glEnd();                                     // Done Building Our Quad (Character)
        glTranslated(12,0,0);                        // Move To The Right Of The Character
        glEndList();                                 // Done Building The Display List
    }                                              // Loop Until All 256 Are Built
}

void font::PrintText(GLint x, GLint y, char type, char *fmt,...)// Where The Printing Happens
{
    char    text[1024];                            // Holds Our String
    int blendOn,scissorOn,textureOn,lightOn;  // Holds the previous GL settings
    int depthOn,matrixMode,screenStats[4],blendSrc,blendDst;
    char typ=type;

    va_list ap;                                    // Pointer To List Of Arguments

    if (fmt == NULL)                               // If There's No Text
    return;                                      // Do Nothing

    va_start(ap, fmt);                             // Parses The String For Variables
    vsprintf(text, fmt, ap);                       // And Converts Symbols To Actual Numbers
    va_end(ap);                                    // Results Are Stored In Text

    if (type>3)                                     // Did User Choose An Invalid Character Set?
    type=3;                                       // If So, Select Set 2 (Italic)

    textureOn = glIsEnabled(GL_TEXTURE_2D); // Were textures enabled?
    depthOn = glIsEnabled(GL_DEPTH_TEST);  // Was GL_DEPTH_TEST enabled?
    lightOn = glIsEnabled(GL_LIGHTING); // Was GL_LIGHTING enabled?
    scissorOn = glIsEnabled(GL_SCISSOR_TEST);  // etc.
    glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
    glGetIntegerv(GL_VIEWPORT, screenStats);
    blendOn= glIsEnabled(GL_BLEND);
    glGetIntegerv(GL_BLEND_SRC, &blendSrc);
    glGetIntegerv(GL_BLEND_DST, &blendDst);

    if (depthOn) glDisable(GL_DEPTH_TEST);  // If they were enabled/disabled
    if (!textureOn) glEnable(GL_TEXTURE_2D); // then enable/disable them
    if (!blendOn) glEnable(GL_BLEND);
    if (!scissorOn) glEnable(GL_SCISSOR_TEST);
    if (lightOn) glDisable(GL_LIGHTING);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the correct blending mode

    glMatrixMode(GL_PROJECTION); // and initalize Ortho mode
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f,screenStats[2],screenStats[3],0.0f,-1.0f,1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,texID);

    if(type>1) typ=typ-2;
    glLoadIdentity();
    glTranslated(x,y,-1);
    glListBase(base-32+(128*typ));
    glScalef(sizex,sizey,1.0f);
    glCallLists(strlen(text),GL_UNSIGNED_BYTE, text); // Write The Text To The Screen
    if(type>1)
    {
        glLoadIdentity();
        glTranslated(x+1,y,0);
        glListBase(base-32+(128*(type-2)));
        glScalef(sizex,sizey,1.0f);
        glCallLists(strlen(text),GL_UNSIGNED_BYTE, text); // Write The Text To The Screen
    }

    glBindTexture(GL_TEXTURE_2D, 0); // Fix everything up
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    if (depthOn) glEnable(GL_DEPTH_TEST);
    if (!textureOn) glDisable(GL_TEXTURE_2D);
    if (!blendOn) glDisable(GL_BLEND);
    if (!scissorOn) glDisable(GL_SCISSOR_TEST);
    if (lightOn) glEnable(GL_LIGHTING);

    glBlendFunc(blendSrc, blendDst);
    glMatrixMode(matrixMode);
}
