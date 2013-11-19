#ifndef OCTREE_H
#define OCTREE_H

#include "frustum.h"
#include "polygon.h"
#include <vector>
#include <iostream>
using namespace std;

class octree
{
    public:
        octree();
        void build(int depth, Vec3f minCoord, Vec3f maxCoord);
        void draw(vector<polygon> *triangles, GLuint *textures,frustum *curFrus);
        void drawNode(vector<polygon> *triangles, GLuint *textures);
        void insertPolygon(polygon *triangle, int id);
        void toggleDrawLine();
        void resetNumDrawn();
        bool inBox(polygon *triangle);
        int getNumDrawn();
        bool hasNode;
        int numTriangles;
        Vec3f center,min,max;
        vector<int> tris;
        bool built;
        int treeDepth;

        octree *nodes;
};

#endif
