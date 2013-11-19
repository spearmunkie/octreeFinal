#ifndef PLANE_H
#define PLANE_H

#include "vec3f.h"
#include <math.h>
#define BEHIND 0
#define IN 1
#define ABOVE 2
struct plane
{
    Vec3f abc;
    float d;
    void normaize()
    {
        float m = sqrt(abc[0] * abc[0] + abc[1] * abc[1] + abc[2] * abc[2]);
        d=d/m;
        abc=abc.normalize();
    }
    int SideOfPlane(Vec3f point)
    {
        Vec3f tmp=abc;
        float val=tmp.dot(point)+d;

        if (val>0) return ABOVE;
        if (val<0) return BEHIND;
        else return IN;
    }
};
#endif
