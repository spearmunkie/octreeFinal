#include "camera.h"

void camera::setCamera(Vec3f p,Vec3f v,Vec3f u)
{
    pos=p;
    view=v;
    up=u;
}

void camera::setView()
{
    gluLookAt(pos[0],pos[1],pos[2],
              view[0],view[1],view[2],
              up[0],up[1],up[2]);
}

void camera::moveCamera(float dir)
{
    Vec3f lookDir;
    lookDir=view-pos;
    lookDir=lookDir.normalize();

    pos+=lookDir*dir;
    view+=lookDir*dir;
}

void camera::rotCam(float ang, Vec3f axis)
{
    Vec3f newLookDir,lookDir;
    float Sin=(float)sin(ang), Cos=(float)cos(ang);
    lookDir=view-pos;
    lookDir=lookDir.normalize();

    newLookDir[0]= (Cos + (1.0 - Cos) * axis[0]) * lookDir[0];
    newLookDir[0]+= ((1 - Cos) * axis[0] * axis[1] - axis[2] * Sin)* lookDir[1];
    newLookDir[0]+= ((1 - Cos) * axis[0] * axis[2] + axis[1] * Sin) * lookDir[2];

    newLookDir[1] = ((1 - Cos) * axis[0] * axis[1] + axis[2] * Sin) * lookDir[0];
	newLookDir[1] += (Cos + (1 - Cos) * axis[1]) * lookDir[1];
	newLookDir[1] += ((1 - Cos) * axis[1] * axis[2] - axis[0] * Sin) * lookDir[2];

	newLookDir[2] = ((1 - Cos) * axis[0] * axis[2] - axis[1] * Sin) * lookDir[0];
	newLookDir[2] += ((1 - Cos) * axis[1] * axis[2] + axis[0] * Sin) * lookDir[1];
	newLookDir[2] += (Cos + (1 - Cos) * axis[2]) * lookDir[2];

	newLookDir=newLookDir.normalize();

	view=pos+newLookDir;
}

void camera::mouseRot(int deltaX,int deltaY)
{
    if ((deltaX==0)&&deltaY==0) return;
    float dir,rot;
    dir=(float)deltaX/1500;
    rot=(float)deltaY/1500;
    Vec3f Adir,axis;
    Adir=view-pos;
    axis=Adir.cross(up);
    axis=axis.normalize();
    rotCam(rot,axis);
    rotCam(dir, Vec3f(0,1,0));
}

