//used to draw al 2D images
#ifndef SPRITE_H
#define SPRITE_H


#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
extern SDL_Surface *screen;
class sprite
{
    public:
        sprite();
        ~sprite();
        bool load(const char *filename, Uint8 r, Uint8 g, Uint8 b);	//color keyed
        bool load(const char *filename);							//non color keyed

        bool draw(int x, int y);//simple draw function, uses sdl_blit


		int getW(){return pic->w;};//width and height of our pictures
		int getH(){return pic->h;};

		SDL_Surface *getSurface(){return pic;};//returns the sruface of our image
    private:
        SDL_Surface* pic;//the actual image surface
        SDL_Rect offset;//the rectangular offsets; used for drawing

};
#endif
