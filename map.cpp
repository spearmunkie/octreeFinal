#include "map.h"
map::map(bool is3d)
{
    Map=NULL;
    tileSize=16;
    cubeSize=2;
    dx=0;
    dy=0;
    drawMat=false;
    is3D=is3d;
}

void map::draw(int z)
{
    for(int y=0;y<totalY;y++)
        for(int x=0;x<totalX;x++)
        {
            Tile[Map[loc(x,y,z)].polyID].draw(x*tileSize+dx,y*tileSize+dy);
            if (drawMat)
                Material[Map[loc(x,y,z)].material].draw(x*tileSize+dx,y*tileSize+dy);
        }
}

bool map::load(const char* fileName)
{
    ifstream fin(fileName);
    string tmp;
    fin>>totalX>>totalY>>totalZ;
    fin>>picInfo;
    fin>>cubeInfo;
    Map=new tileID[totalX*totalY*totalZ];
    for(int z=0;z<totalZ;z++)
        for(int y=0;y<totalY;y++)
            for(int x=0;x<totalX;x++)
                fin>>Map[loc(x,y,z)].polyID>>Map[loc(x,y,z)].property>>Map[loc(x,y,z)].material;

    if (!is3D)
        loadTiles(picInfo.c_str());
}
bool map::save(const char* fileName)
{
    ofstream fout(fileName);
    string tmp;
    fout<<totalX<<" "<<totalY<<" "<<totalZ<<endl;
    fout<<picInfo<<endl;
    fout<<cubeInfo<<endl;
    for(int z=0;z<totalZ;z++)
        for(int y=0;y<totalY;y++)
            for(int x=0;x<totalX;x++)
                fout<<Map[loc(x,y,z)].polyID<<" "<<Map[loc(x,y,z)].property<<" "<<Map[loc(x,y,z)].material<<endl;

}

int map::getProperty(int x,int y,int z) const
{
    if ((x<0||x>totalX-1)||(y<0||y>totalY-1)||(z<0||z>totalZ-1))
        return -1;
    return Map[ loc( x, y, z) ].property;
}

int map::getMaterial(int x,int y,int z)const
{
    if ((x<0||x>totalX-1)||(y<0||y>totalY-1)||(z<0||z>totalZ-1))
        return -1;
    return Map[ loc( x, y, z) ].material;
}

int map::getObj(int x,int y,int z)const
{
    if ((x<0||x>totalX-1)||(y<0||y>totalY-1)||(z<0||z>totalZ-1))
        return -1;
    return Map[ loc( x, y, z) ].polyID;
}

void map::setProperty(int x,int y,int z,int toSet)
{
    if ((x<0||x>totalX-1)||(y<0||y>totalY-1)||(z<0||z>totalZ-1))
        return;

    Map[loc(x,y,z)].property=toSet;
}

void map::setMaterial(int x,int y,int z,int toSet)
{
    if ((x<0||x>totalX-1)||(y<0||y>totalY-1)||(z<0||z>totalZ-1))
        return;
    if (toSet<0||toSet>Material.size())
        return;
    Map[loc(x,y,z)].material=toSet;
}

void map::setObj(int x,int y, int z, int toSet)
{
    if ((x<0||x>totalX-1)||(y<0||y>totalY-1)||(z<0||z>totalZ-1))
        return;
    Map[loc(x,y,z)].polyID=toSet;
}

void map::blankMap(int x,int y,int z)
{
    totalX=x;
    totalY=y;
    totalZ=z;
    Map =new tileID[x*y*z];
    for(int a=0;a<x*y*z;a++)
    {
        Map[a].polyID=0;
        Map[a].property=0;
        Map[a].material=0;
    }
    getInfo("config/defaultInit.txt");
    if (!is3D)
        loadTiles(picInfo.c_str());
}

int map::getMaxX()const
{
    return totalX;
}

int map::getMaxY()const
{
    return totalY;
}

int map::getMaxZ()const
{
    return totalZ;
}

void map::setDx(int x)
{
    dx=x;
}
void map::setDy(int y)
{
    dy=y;
}

void map::toggleMatShow()
{
    drawMat=!drawMat;
}

void map::getInfo(const char* tmp)
{
    ifstream fin(tmp);
    fin>>picInfo>>cubeInfo;
}

void map::loadTiles(const char* tmp)
{
    ifstream fin(tmp);
    int max;
    string name;
    fin>>max;
    Material.resize(max);
    for(int x=0;x<max;x++)
    {
        fin>>name;
        Material[x].load(name.c_str());
    }

    fin>>max;
    Tile.resize(max);
    for(int x=0;x<max;x++)
    {
        fin>>name;
        Tile[x].load(name.c_str());
    }
}

int map::loc(int x,int y,int z) const
{
    if ( x<0 || x>totalX )
        return 0;
    if ( y<0 || y>totalY )
        return 0;
    if ( z<0 || z>totalZ )
        return 0;
    return (y*totalX+ x +z*totalX*totalY);
}
