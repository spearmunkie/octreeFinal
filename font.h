#ifndef FONT_H
#define FONT_H

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "vec3f.h"
class font
{
    public:
        font() {sizex=1.0f; sizey=1.0f;};
        void BuildFont(const char *file);
        void PrintText(int x, int y, char type, char *fmt, ...);
        void SetSize(float x, float y);
        void setColour(Vec3f col) {colour=col;}
    private:
        void loadTexture(const char* name,GLuint &texture);
        int base;
        GLuint texID;
        float sizex,sizey;
        Vec3f colour;

};
#endif
