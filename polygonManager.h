#ifndef POLYGONMANAGER_H
#define POLYGONMANAGER_H

#include "polygon.h"
#include "map.h"
#include "octree.h"
#include <vector>
#include <string>
#include <fstream>
using namespace std;
struct obj
{
    int numTris;
    Vec3f norms;
    float texCoords[3][2];
    Vec3f verts[3];
};

class polygonManager
{
    public:
        polygonManager() {xmin=0; ymin=0; zmin=0; xmax=0; ymax=0; zmax=0;}
        void drawBruteForce();
        void drawOctree(frustum *curFrustum);
        void buildOctree();
        void toggleOctLineDraw(){headNode.toggleDrawLine();}
        void buildTextures();
        void loadMap(const map* toLoad);
        int getNumdrawn(){return numDrawn;}
    private:
         vector<polygon> list;
         GLuint* textureList;
         vector<string> texNames;
         void loadTexture(const char* name,GLuint &texture);
         float xmin, ymin, zmin, xmax, ymax, zmax;
         octree headNode;
         int numDrawn;

};
#endif/*
*/
