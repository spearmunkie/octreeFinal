#ifndef CAMERA_H
#define CAMERA_H
#include "vec3f.h"
#include <math.h>
#include <SDL/SDL_opengl.h>

class camera
{
   public:
      void setCamera(Vec3f p,Vec3f v,Vec3f u);
      void setView();
      void moveCamera(float dir);
      void rotCam(float ang, Vec3f axis);
      void mouseRot(int deltaX,int deltaY);

    private:
      Vec3f pos;// Camera position.
      Vec3f view;// Look at position.
      Vec3f up;// Up direction.
};

#endif
