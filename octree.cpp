#include "octree.h"
#define MAXTRISINNODE 30
#define MINWIDTH 0.1
#define MAXDEPTH 5
bool drawLine=false;
int numDrawn=0;
int toDraw=1;
octree::octree()
{
    hasNode = false;
    nodes = NULL;
    treeDepth = 0;
}

bool octree::inBox(polygon *triangle)
{
    for(int i=0;i<3;i++)
    {
        float x=(*triangle).verts[i][0], y=(*triangle).verts[i][1], z=(*triangle).verts[i][2];
        if(min[0]<=x && x<=max[0] &&
           min[1]<=y && y<=max[1] &&
           min[2]<=z && z<=max[2])
            return true;
    }
    return false;
}

void octree::build(int depth, Vec3f minCoord, Vec3f maxCoord)
{
    treeDepth = depth;

    min = minCoord;
        max = maxCoord;
        center = minCoord + maxCoord;
        center = center/2;

    if(depth<MAXDEPTH) {
        nodes=new octree[8];
        hasNode = true;

        nodes[0].build( depth + 1, Vec3f(min[0], center[1], center[2]), Vec3f(center[0], max[1], max[2]) );
        nodes[1].build( depth + 1, Vec3f(center[0], center[1], center[2]), Vec3f(max[0], max[1], max[2]) );
        nodes[2].build( depth + 1, Vec3f(center[0], center[1], min[2]), Vec3f(max[0], max[1], center[2]) );
        nodes[3].build( depth + 1, Vec3f(min[0], center[1], min[2]), Vec3f(center[0], max[1], center[2]) );

        nodes[4].build( depth + 1, Vec3f(min[0], min[1], center[2]), Vec3f(center[0], center[1], max[2]) );
        nodes[5].build( depth + 1, Vec3f(center[0], min[1], center[2]), Vec3f(max[0], center[1], max[2]) );
        nodes[6].build( depth + 1, Vec3f(center[0], min[1], min[2]), Vec3f(max[0], center[1], center[2]) );
        nodes[7].build( depth + 1, Vec3f(min[0], min[1], min[2]), Vec3f(center[0], center[1], center[2]) );
    }
    else {
        hasNode=false;
    }
}

void octree::insertPolygon(polygon *triangle, int id) {
    if (inBox(triangle)) {
        tris.push_back(id);
        if(hasNode) {
            for(int i=0; i<8; i++) {
                nodes[i].insertPolygon(triangle, id);
            }
        }
    }
}

void octree::draw(vector<polygon> *triangles, GLuint *textures,frustum *curFrus)
{
    int nodeVisibility=curFrus->boxInFrustum(min, max);

    if (nodeVisibility == FULL) {
        drawNode(triangles,textures);
    }
    else if (nodeVisibility == PARTIAL) {
        if(tris.size() < MAXTRISINNODE) {
            drawNode(triangles,textures);
        }
        else if (hasNode) {
            for(int i=0;i<8;i++) {
                nodes[i].draw(triangles, textures, curFrus);
            }
        }
        else
            drawNode(triangles,textures);
    }
}

void octree::drawNode(vector<polygon> *triangles, GLuint *textures)
{
        for(int i=0;i<tris.size();i++)
        {
            int a=tris[i];
            if ((*triangles)[a].drawn!=toDraw)
            {
                (*triangles)[a].drawn=toDraw;
                glBindTexture(GL_TEXTURE_2D, textures[(*triangles)[a].texID]);
                glNormal3f( (*triangles)[a].norms[0],(*triangles)[a].norms[1], (*triangles)[a].norms[2] );
                glBegin(GL_TRIANGLES);
                    glTexCoord2f((*triangles)[a].texCoord[0][0],(*triangles)[a].texCoord[0][1] ); glVertex3f( (*triangles)[a].verts[0][0],(*triangles)[a].verts[0][1], (*triangles)[a].verts[0][2] );
                    glTexCoord2f((*triangles)[a].texCoord[1][0],(*triangles)[a].texCoord[1][1] ); glVertex3f( (*triangles)[a].verts[1][0],(*triangles)[a].verts[1][1], (*triangles)[a].verts[1][2] );
                    glTexCoord2f((*triangles)[a].texCoord[2][0],(*triangles)[a].texCoord[2][1] ); glVertex3f( (*triangles)[a].verts[2][0],(*triangles)[a].verts[2][1], (*triangles)[a].verts[2][2] );
                glEnd();
                numDrawn++;
            }
        }
        if (drawLine)
        {
            glDisable(GL_TEXTURE_2D);
            glColor3f(1.0f,1.0f,1.0f);
            glBegin(GL_LINES);
                glVertex3f(min[0],min[1],min[2]);
                glVertex3f(min[0],min[1],max[2]);

                glVertex3f(min[0],min[1],max[2]);
                glVertex3f(min[0],max[1],max[2]);

                glVertex3f(min[0],max[1],max[2]);
                glVertex3f(min[0],max[1],min[2]);

                glVertex3f(min[0],max[1],min[2]);
                glVertex3f(min[0],min[1],min[2]);

                glVertex3f(max[0],min[1],min[2]);
                glVertex3f(max[0],min[1],max[2]);

                glVertex3f(max[0],min[1],max[2]);
                glVertex3f(max[0],max[1],max[2]);

                glVertex3f(max[0],max[1],max[2]);
                glVertex3f(max[0],max[1],min[2]);

                glVertex3f(max[0],max[1],min[2]);
                glVertex3f(max[0],min[1],min[2]);

                glVertex3f(min[0],min[1],min[2]);
                glVertex3f(max[0],min[1],min[2]);

                glVertex3f(min[0],max[1],min[2]);
                glVertex3f(max[0],max[1],min[2]);

                glVertex3f(min[0],min[1],max[2]);
                glVertex3f(max[0],min[1],max[2]);

                glVertex3f(min[0],max[1],max[2]);
                glVertex3f(max[0],max[1],max[2]);

            glEnd();
            glEnable(GL_TEXTURE_2D);
        }
}
void octree::resetNumDrawn()
{
    toDraw++;
    numDrawn=0;
}

int octree::getNumDrawn()
{
    return numDrawn;
}

void octree::toggleDrawLine()
{
    drawLine=!drawLine;
}
