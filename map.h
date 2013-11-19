#ifndef MAP_H
#define MAP_H

#include<GL/gl.h>
#include "sprite.h"
#include "vec3f.h"
#include<vector>
#include<fstream>
#include<iostream>
#include<string>
using namespace std;

struct tileID
{
    int polyID;
    int property;
	int material;
};

class map
{
    public:
        map(bool is3d);
        void draw(int z);

        bool load(const char* filename);
        bool save(const char* filename);

        int getProperty(int x,int y,int z) const;
        int getMaterial(int x,int y,int z) const;
        int getObj(int x,int y,int z) const;

        void setProperty(int x,int y,int z,int toSet);
        void setMaterial(int x,int y,int z,int toSet);
        void setObj(int x,int y, int z, int toSet);


        void blankMap(int x,int y,int z);

        int getMaxX()const;
        int getMaxY()const;
        int getMaxZ()const;

        string getCubeInfo()const {return cubeInfo;}

        void setDx(int x);
        void setDy(int y);
        int getDx() {return dx;}
        int getDy() {return dy;}

        void setTileSize(int size) { tileSize=size;}

        void toggleMatShow();
    protected:
        vector<sprite> Tile;
        vector<sprite> Material;

        void getInfo(const char* tmp);
        void loadTiles(const char* tmp);
        string cubeInfo;
        string picInfo;

        tileID *Map;
        int loc(int x,int y,int z) const;
        int totalX,totalY,totalZ;
        int tileSize,cubeSize;

        int dx,dy;
        bool drawMat;
        bool is3D;
};
#endif

