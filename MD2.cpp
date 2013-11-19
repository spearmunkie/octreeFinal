#include "MD2.h"
#include <iostream>
//#include<conio.h>
using namespace std;
// Table of precalculated normals
Vec3 normals[162] = {
#include "anorms.h"
};

void MD2Model::loadTexture(const char* name)
{
    doneAnim=false;
    SDL_Surface *TextureImage;
    TextureImage=IMG_Load(name);
    animName="";
    if (TextureImage!=NULL)
    {
	    glGenTextures( 1, &texture);
	    glBindTexture( GL_TEXTURE_2D, texture);

	    glTexImage2D( GL_TEXTURE_2D, 0, 3,
                    TextureImage->w,
                    TextureImage->h,
                    0, GL_BGR,
                    GL_UNSIGNED_BYTE,
                    TextureImage->pixels );

	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
    cout<<"tex: "<<name<<endl;
    SDL_FreeSurface( TextureImage);
}

MD2Model::MD2Model()
{
    skins=NULL;
    texcoords=NULL;
    triangles=NULL;
    frames=NULL;
    glcmds=NULL;
    interp=0.0;
    start=0;
    end=0;
}
MD2Model::~MD2Model()
{
    delete [] skins;
    delete [] texcoords;
    delete [] triangles;
    delete [] frames;
    delete [] glcmds;
}
bool MD2Model::ReadMD2Model(const char* fileName)
{

    ifstream file(fileName,ios::binary);
    if (file.fail())
        return false;
    // read header file
	file.read( (char *)&header, sizeof( MD2Header ) );
	if( (header.ident != MD2_IDENT) && (header.version != MD2_VERSION) )
	{
		// this is not a MD2 model
		file.close();
		return false;
	}
    //allocate all memory
	skins = new MD2Skin[header.numSkins];
	texcoords = new MD2TexCoord[header.numST];
	triangles = new MD2Triangle[header.numTris];
	frames = new MD2Frame[header.numFrames];
	glcmds = new int[header.numGlcmds];
	/////////////////////////////////////////////
	//			reading file data

	// read skin data...
	file.seekg(header.offsetSkins, ios::beg);
	file.read( (char*) skins, header.numSkins*sizeof(MD2Skin));

	// read texture coordinate data...
	file.seekg(header.offsetST, ios::beg);
	file.read( (char*) texcoords, header.numST*sizeof(MD2TexCoord));

	// read triangle data...
	file.seekg(header.offsetTris, ios::beg);
	file.read( (char*) triangles, header.numTris*sizeof(MD2Triangle));

	// read gl commands data...
	file.seekg(header.offsetGlcmds, ios::beg);
	file.read( (char*) glcmds, header.numGlcmds*sizeof(int));

	//read frames
	file.seekg(header.offsetFrames, ios::beg);
	for (int x = 0; x < header.numFrames; x++)
	{
        frames[x].verts=new MD2Vertex[header.numVertices];
	    file.read( (char*) frames[x].scale,1*sizeof(Vec3));
	    file.read( (char*) frames[x].translate,1*sizeof(Vec3));
        file.read( (char*) frames[x].name,16*sizeof(char));
	    file.read( (char*) frames[x].verts,header.numVertices*sizeof(MD2Vertex));
	}
	return true;
}

void MD2Model::Animate ()
{
    last_time = curent_time;
    curent_time = (double)SDL_GetTicks() / 1000.0;

    interp += 10 * (curent_time - last_time);
    if ((frame < start) || (frame > end))
        frame = start;

    if (interp >= 1.0f)
    {
        /* Move to next frame */
        interp = 0.0f;
        frame++;
        if (frame >= end)
        {
            doneAnim=true;
            frame = start;
        }
    }
}

void MD2Model::draw()
{
   // text();
    Vec3 curr, next,norm,vertex;
    float *nCurr, *nNext;
    GLfloat s,t;
    MD2Frame *frame1, *frame2;
    MD2Vertex *vert1, *vert2;
    MD2Glcmd *packet;

    // Check if n is in a valid range
    if ((frame < 0) || (frame > header.numFrames - 1))
        return;
    // Draw the model
    glBindTexture (GL_TEXTURE_2D, texture);
    glBegin (GL_TRIANGLES);
    for(int x=0;x<header.numTris;++x)
       for(int i=0;i<3;++i)
       {
           frame1=&frames[frame];
           frame2=&frames[((frame+1)>=end)?start:(frame+1)];
           vert1=&frame1->verts[triangles[x].vertex[i]];
           vert2=&frame2->verts[triangles[x].vertex[i]];
           nCurr=normals[vert1->normalIndex];
           nNext=normals[vert2->normalIndex];

           norm[0]= nCurr[0] + interp * (nNext[0] - nCurr[0]);
           norm[1]= nCurr[1] + interp * (nNext[1] - nCurr[1]);
           norm[2]= nCurr[2] + interp * (nNext[2] - nCurr[2]);

           curr[0]=(frame1->scale[0] * vert1->v[0])+frame1->translate[0];
           curr[1]=(frame1->scale[1] * vert1->v[1])+frame1->translate[1];
           curr[2]=(frame1->scale[2] * vert1->v[2])+frame1->translate[2];

           next[0]=(frame2->scale[0] * vert2->v[0])+frame2->translate[0];
           next[1]=(frame2->scale[1] * vert2->v[1])+frame2->translate[1];
           next[2]=(frame2->scale[2] * vert2->v[2])+frame2->translate[2];

           vertex[0]=curr[0] + interp*(next[0]-curr[0]);
           vertex[1]=curr[1] + interp*(next[1]-curr[1]);
           vertex[2]=curr[2] + interp*(next[2]-curr[2]);

           s = (GLfloat)texcoords[triangles[x].st[i]].s / header.skinwidth;
           t = (GLfloat)texcoords[triangles[x].st[i]].t / header.skinheight;

           glTexCoord2f (s, t);
           glNormal3fv (norm);
           glVertex3fv (vertex);
       }
    glEnd();

}
int MD2Model::getEnd()
{
    return header.numFrames;
}

void MD2Model::setAnimation(const string name)
{
    if(name!=animName)
    {
        animName=name;
        frame=start;
        doneAnim=false;
    }
    bool found = false;
    for(int i = 0; i < header.numFrames; i++)
    {
        MD2Frame* frame = frames + i;
        if (strlen(frame->name) > strlen(name.c_str()) &&
            strncmp(frame->name, name.c_str(), strlen(name.c_str())) == 0 &&
            !isalpha(frame->name[strlen(name.c_str())]))
            {
            if (!found) {
                found = true;
                start = i;
            }
            else {
                end = i;
            }
        }
        else if (found) {
            break;
        }
    }
}
