#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "plane.h"
#include "vec3f.h"

#define OUT 0
#define FULL 1
#define PARTIAL 2
class frustum
{
    public:
        int boxInFrustum(Vec3f min, Vec3f max);
        void UpdateFrustum();
        frustum();

    private:
        plane fPlanes[6];
};

#endif
