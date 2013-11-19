#ifndef POLYGON_H
#define POLYGON_H

#include "vec3f.h"

struct polygon
{
    polygon()
    {
        drawn=0;
    }
    polygon(float t[3][2],int texture,Vec3f n,Vec3f v[3])
    {
        texID=texture;
        for(int x=0;x<3;x++)
        {
            norms[x]=n[x];
            verts[x]=v[x];
            texCoord[x][0]=t[x][0];
            texCoord[x][1]=t[x][1];
        }
    }

    Vec3f verts[3];
    Vec3f norms;
    float texCoord[3][2];
    int texID;
    int drawn;
};
#endif
