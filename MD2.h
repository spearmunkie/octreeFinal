#ifndef MD2_H
#define MD2_H
//This file is only used in the beta
#include <GL/gl.h>
#include <GL/glu.h>
#include<GL/glext.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include<fstream>
#include<iostream>
#include "vec3f.h"
// magic number "IDP2" or 844121161
#define MD2_IDENT				(('2'<<24) + ('P'<<16) + ('D'<<8) + 'I')

// model version
#define	MD2_VERSION				8
using namespace std;
typedef float Vec3[3];

// MD2 header
struct MD2Header
{
    int ident;                  // magic number: "IDP2"
    int version;                // version: must be 8

    int skinwidth;              // texture width
    int skinheight;             // texture height

    int frameSize;              // size in bytes of a frame

    int numSkins;              // number of skins
    int numVertices;           // number of vertices per frame
    int numST;                 // number of texture coordinates
    int numTris;               // number of triangles
    int numGlcmds;             // number of opengl commands
    int numFrames;             // number of frames

    //these make loading the md2 file much easier
    //if we ever want something, just seek with these values
    int offsetSkins;           // offset skin data
    int offsetST;              // offset texture coordinate data
    int offsetTris;            // offset triangle data
    int offsetFrames;          // offset frame data
    int offsetGlcmds;          // offset OpenGL command data
    int offsetEnd;             // offset end of file
};
//typedef float Vec3f[3];

// Texture name
struct MD2Skin
{
  char name[64];
};

//Texture coords
struct MD2TexCoord
{
  short s;
  short t;
};

// Triangle info
struct MD2Triangle
{
  unsigned short vertex[3];
  unsigned short st[3];
};

// Compressed vertex
struct MD2Vertex
{
  unsigned char v[3];
  unsigned char normalIndex;
};

// Model frame
struct MD2Frame
{
  Vec3 scale;
  Vec3 translate;
  char name[16];
  MD2Vertex *verts;
};

// GL command packet
struct MD2Glcmd
{
  float s;
  float t;
  int index;
};

class MD2Model
{
    public:
        MD2Model();
        ~MD2Model();
        		//Switches to the given animation
		void Animate ();
		//Draws the current state of the animated model.
		void draw();

		void setAnimation(const string name);//set the anmation by finding out where the frame names begin and end
		//Loads an MD2Model from the specified file.  Returns false if there was
		//an error loading it.
		bool ReadMD2Model(const char* fileName);
		void loadTexture(const char* name);//load the texture
		int getEnd();//we don't need this anymore
		bool doneAnim;
    private:
        MD2Header header;//the header contains everything
        string animName;
        MD2Skin *skins;//we don't really need this
        MD2TexCoord *texcoords;//texture coordinates
        MD2Triangle *triangles;//triangle, has 3 verticies per element
        MD2Frame *frames;//frame, contains all the verticies and tex coords for each frame
        int *glcmds;//we aren't using gl commands
        int frame,start,end;//self explanatory
        float interp;//interpolation
        double curent_time;//these two are used for interpolation
        double last_time;


        GLuint texture;//the actual texture of our md2
};
#endif
