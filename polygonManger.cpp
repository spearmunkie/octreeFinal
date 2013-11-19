#include "polygonManager.h"
#include <GL/glext.h>
void polygonManager::drawBruteForce()
{
    for(int a=0;a<list.size();a++)
    {
        glBindTexture(GL_TEXTURE_2D,textureList[list[a].texID]);
        glBegin( GL_TRIANGLES );
            glNormal3f( list[a].norms[0],list[a].norms[1], list[a].norms[2] );
            glTexCoord2f(list[a].texCoord[0][0],list[a].texCoord[0][1] ); glVertex3f( list[a].verts[0][0],list[a].verts[0][1], list[a].verts[0][2] );
            glTexCoord2f(list[a].texCoord[1][0],list[a].texCoord[1][1] ); glVertex3f( list[a].verts[1][0],list[a].verts[1][1], list[a].verts[1][2] );
            glTexCoord2f(list[a].texCoord[2][0],list[a].texCoord[2][1] ); glVertex3f( list[a].verts[2][0],list[a].verts[2][1], list[a].verts[2][2] );
        glEnd();
    }
    numDrawn=list.size();
}

void polygonManager::loadTexture(const char* name,GLuint &texture)
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
                    0, GL_BGR,
                    GL_UNSIGNED_BYTE,
                    TextureImage->pixels );
	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
    SDL_FreeSurface( TextureImage);

}
void polygonManager::buildTextures()
{
    textureList=new GLuint[texNames.size()];
    for(int x=0;x<texNames.size();x++)
        loadTexture(texNames[x].c_str(),textureList[x]);
}
void polygonManager::buildOctree()
{
    headNode.build(0, Vec3f(xmin, ymin, zmin), Vec3f(xmax, ymax, zmax));

    for( int x=0;x<list.size();x++)
        headNode.insertPolygon(&list[x], x);

    cout<<"done\n";
}

void polygonManager::drawOctree(frustum *curFrustum)
{
    headNode.resetNumDrawn();
    headNode.draw(&list,textureList,curFrustum);
    numDrawn=headNode.getNumDrawn();
}

void polygonManager::loadMap(const map* toLoad)
{
    polygon **cubes;
    string name;
    int *numTris;
    int max;

    ifstream fin (toLoad->getCubeInfo().c_str());
    fin>>max;
    for(int x=0;x<max;x++)
    {
        fin>>name;
        texNames.push_back(name);
    }

    fin>>max;
    cubes= new polygon*[max];
    numTris=new int [max];
    for(int x=0;x<max;x++)
    {
        fin>>numTris[x];
        cubes[x]=new polygon[numTris[x]];
        for(int y=0;y<numTris[x];y++)
        {
            fin>>cubes[x][y].norms[0]
               >>cubes[x][y].norms[1]
               >>cubes[x][y].norms[2];
            for(int a=0;a<3;a++)
            {
                fin>>cubes[x][y].texCoord[a][0];
                fin>>cubes[x][y].texCoord[a][1];

                fin>>cubes[x][y].verts[a][0];
                fin>>cubes[x][y].verts[a][1];
                fin>>cubes[x][y].verts[a][2];
            }

        }
    }
    int deb=0;
    float scale=2;
    for(int x=0;x<toLoad->getMaxX();x++)
        for(int y=0;y<toLoad->getMaxY();y++)
            for(int z=0;z<toLoad->getMaxZ();z++)
            {
                int n=toLoad->getObj(x,y,z),m=toLoad->getMaterial(x,y,z);
                for(int a=0;a<numTris[n];a++)
                {

                    polygon tmp(cubes[n][a].texCoord,m,cubes[n][a].norms,cubes[n][a].verts);
                    Vec3f toScale(8,16,8);
                    Vec3f toAdd(-toScale[0]*x,toScale[1]*z,-toScale[2]*y);
                    Vec3f scaler (4,8,4);
                    tmp.verts[0]*=scaler;
                    tmp.verts[1]*=scaler;
                    tmp.verts[2]*=scaler;

                    tmp.verts[0]+=toAdd;
                    tmp.verts[1]+=toAdd;
                    tmp.verts[2]+=toAdd;
                    list.push_back(tmp);
                    if (tmp.verts[0][0]>xmax)xmax=tmp.verts[0][0];if (tmp.verts[0][0]<xmin)xmin=tmp.verts[0][0];
                    if (tmp.verts[0][1]>ymax)ymax=tmp.verts[0][1];if (tmp.verts[0][1]<ymin)ymin=tmp.verts[0][1];
                    if (tmp.verts[0][2]>zmax)zmax=tmp.verts[0][2];if (tmp.verts[0][2]<zmin)zmin=tmp.verts[0][2];

                    if (tmp.verts[1][0]>xmax)xmax=tmp.verts[1][0];if (tmp.verts[1][0]<xmin)xmin=tmp.verts[1][0];
                    if (tmp.verts[1][1]>ymax)ymax=tmp.verts[1][1];if (tmp.verts[1][1]<ymin)ymin=tmp.verts[1][1];
                    if (tmp.verts[1][2]>zmax)zmax=tmp.verts[1][2];if (tmp.verts[1][2]<zmin)zmin=tmp.verts[1][2];

                    if (tmp.verts[2][0]>xmax)xmax=tmp.verts[2][0];if (tmp.verts[2][0]<xmin)xmin=tmp.verts[2][0];
                    if (tmp.verts[2][1]>ymax)ymax=tmp.verts[2][1];if (tmp.verts[2][1]<ymin)ymin=tmp.verts[2][1];
                    if (tmp.verts[2][2]>zmax)zmax=tmp.verts[2][2];if (tmp.verts[2][2]<zmin)zmin=tmp.verts[2][2];
                }
            }
}
