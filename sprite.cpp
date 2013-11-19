#include "sprite.h"
#include <iostream>
using namespace std;
bool sprite::load(const char* filename)
{
    SDL_Surface* loadedImage=NULL;
    SDL_Surface* optimumImage=NULL;

    loadedImage=IMG_Load(filename);

    if (loadedImage!=NULL)
    {
        optimumImage = SDL_DisplayFormat( loadedImage );
        SDL_FreeSurface(loadedImage);
    }
    else
        return false;
    if (optimumImage==NULL)
        return false;
    pic=optimumImage;
    return true;
}

bool sprite::load(const char *filename, Uint8 r, Uint8 g, Uint8 b)
{
    SDL_Surface* loadedImage=NULL;
    SDL_Surface* optimumImage=NULL;

    loadedImage=IMG_Load(filename);

    if (loadedImage!=NULL)
    {
        optimumImage = SDL_DisplayFormat( loadedImage );
        SDL_FreeSurface(loadedImage);
    }
    else
        return false;

    if (optimumImage!=NULL)
    {
        Uint32 colorkey = SDL_MapRGB( optimumImage->format, r, g, b );
        SDL_SetColorKey( optimumImage, SDL_SRCCOLORKEY, colorkey );
    }
    else
        return false;

    pic=optimumImage;
    return true;
}

bool sprite::draw(int x, int y)
{
    offset.x=x;
    offset.y=y;

    return (SDL_BlitSurface(pic,NULL,screen,&offset)<0);
}

sprite::sprite()
{
	offset.x = 0;
	offset.y = 0;
	offset.w = 0;
	offset.h = 0;
	pic = NULL;
}
sprite::~sprite()
{
	if(pic)
	{
		SDL_FreeSurface(pic);
		pic = NULL;
	}
}
