#include "playerBase.h"
#include <math.h>
const double DEG_TO_RAD=57.29578;
playerBase::playerBase()
{
    x=0*16+7.5;
    y=0*16+7.5;
    z=0;
    angle=270;
    vx=0;
    vy=0;
    health=100;
    is3D=false;
    isShooting=false;
    canShoot=true;
}
playerBase::playerBase(float startX, float startY,float startZ,const char* charInfo)
{
    isShooting=false;
    is3D=false;
    load(startX,startY,startZ,charInfo);
}

struct RGB
{
    int r;
    int g;
    int b;
};

void pset(int x,int y,const RGB& color)
{
    Uint32 colorSDL = SDL_MapRGB(screen->format, color.r, color.g, color.b);
    Uint32* bufp;
    bufp = (Uint32*)screen->pixels + y * screen->pitch / 4 + x;
    *bufp = colorSDL;
}

void drawLine(int x1, int y1, int x2, int y2,const RGB &color)
{
    //if we are offscreen
    if (x1<0) x1 = 0;
    if (x1> screen->w - 1) x1= screen->w - 1;

    if (x2<0) x2 = 0;
    if (x2> screen->w - 1) x2= screen->w - 1;

    if (y1<0) y1 = 0;
    if (y1> screen->h - 1) y1= screen->h - 1;

    if (y2<0) y2 = 0;
    if (y2> screen->h - 1) y2= screen->h - 1;

    int deltax = fabs(x2 - x1); //The difference between the x's
    int deltay = fabs(y2 - y1); //The difference between the y's
    int x = x1; //Start x off at the first pixel
    int y = y1; //Start y off at the first pixel
    int xinc1, xinc2, yinc1, yinc2, den, num, numadd, numpixels;
    if(x2 >= x1) //The x-values are increasing
    {
        xinc1 = 1;
        xinc2 = 1;
    }
    else //The x-values are decreasing
    {
        xinc1 = -1;
        xinc2 = -1;
    }
    if(y2 >= y1) //The y-values are increasing
    {
        yinc1 = 1;
        yinc2 = 1;
    }
    else //The y-values are decreasing
    {
        yinc1 = -1;
        yinc2 = -1;
    }
    if (deltax >= deltay) //There is at least one x-value for every y-value
    {
        xinc1 = 0; //Don't change the x when numerator >= denominator
        yinc2 = 0; //Don't change the y for every iteration
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax; //There are more x-values than y-values
    }
    else //There is at least one y-value for every x-value
    {
        xinc2 = 0; //Don't change the x for every iteration
        yinc1 = 0; //Don't change the y when numerator >= denominator
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay; //There are more y-values than x-values
    }

    for (int curpixel = 0; curpixel <= numpixels; curpixel++)
    {
        pset((x) % screen->w, (y) % screen->h, color);  //Draw the current pixel
        num += numadd;  //Increase the numerator by the top of the fraction
        if (num >= den) //Check if numerator >= denominator
        {
            num -= den; //Calculate the new numerator value
            x += xinc1; //Change the x as appropriate
            y += yinc1; //Change the y as appropriate
        }
        x += xinc2; //Change the x as appropriate
        y += yinc2; //Change the y as appropriate
  }
}

void playerBase::draw()
{
    if (is3D)
    {
        tris.Animate();
        weapon.Animate();
        glPushMatrix();

        float scaleFactor=0.1;
        glTranslatef ((float)x/16*-8+4, (float)z*16-5.3, (float)y/16*-8);
        glScalef(scaleFactor,scaleFactor,scaleFactor);
        glRotatef (-angle-180, 0.0, 1.0, 0.0);
        glRotatef (-90.0f, 1.0, 0.0, 0.0);

        tris.draw();
        weapon.draw();
        glPopMatrix();
    }
    else
    {
        pic.draw(x-2,y-2);
        RGB tmp;
        tmp.b=255;
        tmp.g=0;
        tmp.r=255;
        drawLine(x,y,x+10*cos(angle/DEG_TO_RAD),sin(angle/DEG_TO_RAD)*10+y,tmp);
    }
}

float playerBase::getX()
{
    return x;
}

float playerBase::getY()
{
    return y;
}

float playerBase::getZ()
{
    return z;
}

float playerBase::getAngle()
{
    return angle;
}

float playerBase::getHealth()
{
    return health;
}

void playerBase::setIs3D(bool isNot2D)
{
    is3D=isNot2D;
}

void playerBase::load(float startX, float startY,float startZ,const char* charInfo)
{
    x=startX*16+7.5;
    y=startY*16+7.5;
    z=startZ;
    angle=270;
    health=100;
    ifstream fin(charInfo);
    string picName,trisName,weaponName;

    if(is3D)
    {
        fin>>picName
            >>trisName>>trisTex
            >>weaponName>>weaponTex;

        tris.ReadMD2Model(trisName.c_str());
        weapon.ReadMD2Model(weaponName.c_str());
    }
    else
    {
        fin>>picName;
        pic.load(picName.c_str());
    }
    fin.close();
}

void playerBase::generateTextures()
{
    tris.loadTexture(trisTex.c_str());
    weapon.loadTexture(weaponTex.c_str());
}

bool playerBase::isColliding(float vx, float vy,const map* curMap)
{
    float box=2;
    int count=0,count2=0;
    for(int a=-1;a<=1;a++)
        for(int b=-1;b<=1;b++)
            if (curMap->getProperty((int)(x+box*a+vx)/16,(int)(y+box*b+vy)/16,(int)z)==1)
                return true;
            else if(curMap->getProperty((int)(x+box*a+vx)/16,(int)(y+box*b+vy)/16,(int)z)==2)
            {
                count2++;
            }
            else if(curMap->getProperty((int)(x+box*a+vx)/16,(int)(y+box*b+vy)/16,(int)z)==3)
            {
                count++;
            }
    if (count==9)
        z-=0.1;
    if (count2>0&&count2<9)
    {
        if (count2>=3)
        {
            if((curMap->getProperty((int)(x+box*-1+vx)/16,(int)(y+box*-1+vy)/16,(int)z)==2)&&(curMap->getProperty((int)(x+box*1+vx)/16,(int)(y+box*-1+vy)/16,(int)z)==2))
            {
                int b=-1,a=-1;
                float tmp=(int)(y+box*b+vy)/16;
                float tmp2=1-((y+box*b+vy)/16-tmp);
                if (tmp2>0.9)
                    z=(int)z+1;
                else
                    z=(int)z+(float)tmp2;
                return false;
            }
            else
            {
                float t=(int)z;
                float t2=z-t;
                if (t2>0.5)
                    if((curMap->getProperty((int)(x+box*-1+vx)/16,(int)(y+box*1+vy)/16,(int)z)==2)&&(curMap->getProperty((int)(x+box*1+vx)/16,(int)(y+box*1+vy)/16,(int)z)==2))
                    {
                        float tmp=(int)(y+box*1+vy)/16;
                        float tmp2=1-((y+box*1+vy)/16-tmp);
                        if (tmp2>0.9)
                            z=(int)z+1;
                        else
                            z=(int)z+(float)tmp2;
                        return false;
                    }

            }
        }
        return true;
    }
    if (count2==9)
    {
        int b=1,a=-1;
        float tmp=(int)(y+box*b+vy)/16;
        float tmp2=1-((y+box*b+vy)/16-tmp);
        if (tmp2>0.9)
            z=(int)z+1;
        else
            z=(int)z+(float)tmp2;
    }
    return false;
}

void playerBase::collide(const map* curMap)
{
    if (isColliding( vx,0,curMap))
        vx=0;
    if (isColliding( 0,vy,curMap))
        vy=0;
    x+=vx;
    y+=vy;
}
