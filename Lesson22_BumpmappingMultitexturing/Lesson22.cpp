/*
Work in progress
*/

#include <windows.h>	// Standard Header For Most Programs
#include <string.h>     // Header File For The String Library
#include <stdio.h>		//Standard IO header
#include <math.h>		//Math header
#include "mathlib.h"    //Our own math lib
#include <GL/glew.h>    //header for GLEW (GL extensions loading wrangler library)
#include <GL/glut.h>    // The GL Utility Toolkit (Glut) Header
#include <GL/SOIL.h>	//Image loading library header. Library also referenced by linker

#define NUM_TEXTURES 5
#define NUM_NORMAL_MAPS 7

bool extensionsSupported=true;  //Is the extensions used bu this tutorial supported
GLint maxTexelUnits=1;												// Number Of Texel-Pipelines. This Is At Least 1.

GLfloat maxEmboss = 0.01f;

bool emboss = true;
bool    bumps=true;													// Do Bumpmapping?
GLuint  textureIndex=0;                               // Index of the texture used
GLuint  normalMapIndex=0;                               // Stores the index of the normal map used
GLuint  texturemap[NUM_TEXTURES];                             // Storage For 3 Textures
GLuint  normalmap[NUM_NORMAL_MAPS];                                // Our Bumpmappings

GLfloat LightAmbient[]  = { 0.2f, 0.2f, 0.2f};                  // Ambient Light Is 20% White
GLfloat LightDiffuse[]  = { 1.0f, 1.0f, 1.0f};                  // Diffuse Light Is White
GLfloat LightPosition[] = { -4.0f, 1.0f, 2.0f};                  // Position Is Somewhat In Front Of Screen
GLfloat Gray[]      = { 0.5f, 0.5f, 0.5f, 1.0f};

GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat zrot = 0.0f;
GLfloat zoom = -5.0f;

// Data Contains The Faces Of The Cube In Format 2xTexCoord, 3xVertex.
// Note That The Tesselation Of The Cube Is Only Absolute Minimum.
GLfloat data[]= {
    // FRONT FACE
    0.0f, 0.0f,     -1.0f, -1.0f, +1.0f,
    1.0f, 0.0f,     +1.0f, -1.0f, +1.0f,
    1.0f, 1.0f,     +1.0f, +1.0f, +1.0f,
    0.0f, 1.0f,     -1.0f, +1.0f, +1.0f,
    // BACK FACE
    1.0f, 0.0f,     -1.0f, -1.0f, -1.0f,
    1.0f, 1.0f,     -1.0f, +1.0f, -1.0f,
    0.0f, 1.0f,     +1.0f, +1.0f, -1.0f,
    0.0f, 0.0f,     +1.0f, -1.0f, -1.0f,
    // Top Face
    0.0f, 1.0f,     -1.0f, +1.0f, -1.0f,
    0.0f, 0.0f,     -1.0f, +1.0f, +1.0f,
    1.0f, 0.0f,     +1.0f, +1.0f, +1.0f,
    1.0f, 1.0f,     +1.0f, +1.0f, -1.0f,
    // Bottom Face
    1.0f, 1.0f,     -1.0f, -1.0f, -1.0f,
    0.0f, 1.0f,     +1.0f, -1.0f, -1.0f,
    0.0f, 0.0f,     +1.0f, -1.0f, +1.0f,
    1.0f, 0.0f,     -1.0f, -1.0f, +1.0f,
    // Right Face
    1.0f, 0.0f,     +1.0f, -1.0f, -1.0f,
    1.0f, 1.0f,     +1.0f, +1.0f, -1.0f,
    0.0f, 1.0f,     +1.0f, +1.0f, +1.0f,
    0.0f, 0.0f,     +1.0f, -1.0f, +1.0f,
    // Left Face
    0.0f, 0.0f,     -1.0f, -1.0f, -1.0f,
    1.0f, 0.0f,     -1.0f, -1.0f, +1.0f,
    1.0f, 1.0f,     -1.0f, +1.0f, +1.0f,
    0.0f, 1.0f,     -1.0f, +1.0f, -1.0f
};

int CheckExtensionAvailability()
{
    //Check that multitexture extenstion is available run time.
    if (GLEW_ARB_multitexture)
    {
        return true;
    }

    return false;
}

int LoadGLTextures()                                    // Load Bitmaps And Convert To Textures
{
    

    texturemap[0] = SOIL_load_OGL_texture
    (
       "Data/Texture1.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texturemap[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    
    texturemap[1] = SOIL_load_OGL_texture
    (
       "Data/Texture2.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texturemap[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);


    texturemap[2] = SOIL_load_OGL_texture
    (
       "Data/Texture3.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texturemap[2]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
 
    texturemap[3] = SOIL_load_OGL_texture
    (
       "Data/Texture4.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

        // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texturemap[3]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
 
    texturemap[4] = SOIL_load_OGL_texture
    (
       "Data/Texture5.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texturemap[4]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

 
    normalmap[0] = SOIL_load_OGL_texture
    (
       "Data/Normal1.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, normalmap[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    normalmap[1] = SOIL_load_OGL_texture
    (
       "Data/Normal2.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, normalmap[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    normalmap[2] = SOIL_load_OGL_texture
    (
       "Data/Normal3.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, normalmap[2]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);


    normalmap[3] = SOIL_load_OGL_texture
    (
       "Data/Normal4.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, normalmap[3]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    normalmap[4] = SOIL_load_OGL_texture
    (
       "Data/Normal5.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, normalmap[4]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    normalmap[5] = SOIL_load_OGL_texture
    (
       "Data/Normal6.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, normalmap[5]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    normalmap[6] = SOIL_load_OGL_texture
    (
       "Data/Normal7.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, normalmap[6]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    
    for(int i = 0;i<NUM_TEXTURES;i++)
    {
        if(normalmap[i] == 0)
        {
            return false;
        }
    }

        for(int i = 0;i<NUM_TEXTURES;i++)
    {
        if(texturemap[i] == 0)
        {
            return false;
        }
    }
    
    
    return true;                                        // Return Success
    
}


void initLights(void) {
        glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);              // Load Light-Parameters into GL_LIGHT1
        glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
        glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
        glEnable(GL_LIGHT1);
}

// Calculates v=vM, M Is 4x4 In Column-Major, v Is 4dim. Row (i.e. "Transposed")
void VMatMult(GLfloat *M, GLfloat *v) {
    GLfloat res[3];
    res[0]=M[ 0]*v[0]+M[ 1]*v[1]+M[ 2]*v[2]+M[ 3]*v[3];
    res[1]=M[ 4]*v[0]+M[ 5]*v[1]+M[ 6]*v[2]+M[ 7]*v[3];
    res[2]=M[ 8]*v[0]+M[ 9]*v[1]+M[10]*v[2]+M[11]*v[3];
    v[0]=res[0];
    v[1]=res[1];
    v[2]=res[2];
    v[3]=M[15];                             // Homogenous Coordinate
}



void doCube (void) {
    int i;
    glBegin(GL_QUADS);
        // Front Face
        glNormal3f( 0.0f, 0.0f, +1.0f);
        for (i=0; i<4; i++) {
            glTexCoord2f(data[5*i],data[5*i+1]);
            glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
        }
        // Back Face
        glNormal3f( 0.0f, 0.0f,-1.0f);
        for (i=4; i<8; i++) {
            glTexCoord2f(data[5*i],data[5*i+1]);
            glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
        }
        // Top Face
        glNormal3f( 0.0f, 1.0f, 0.0f);
        for (i=8; i<12; i++) {
            glTexCoord2f(data[5*i],data[5*i+1]);
            glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
        }
        // Bottom Face
        glNormal3f( 0.0f,-1.0f, 0.0f);
        for (i=12; i<16; i++) {
            glTexCoord2f(data[5*i],data[5*i+1]);
            glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
        }
        // Right Face
        glNormal3f( 1.0f, 0.0f, 0.0f);
        for (i=16; i<20; i++) {
            glTexCoord2f(data[5*i],data[5*i+1]);
            glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
        }
        // Left Face
        glNormal3f(-1.0f, 0.0f, 0.0f);
        for (i=20; i<24; i++) {
            glTexCoord2f(data[5*i],data[5*i+1]);
            glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
        }
    glEnd();
}



void init ( GLvoid )     // Create Some Everyday Functions
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }

    //Check extension availability runtime
    if(extensionsSupported = CheckExtensionAvailability())
    {
        bumps = true; 
    }
    

    //Load the textures with SOIL
    LoadGLTextures();
    
    //Init lights
    initLights();

    glEnable(GL_TEXTURE_2D);							// Enable Texturemapping!
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    //glEnable ( GL_COLOR_MATERIAL );
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

}

// Sets Up The Texture-Offsets
// n : Normal On Surface. Must Be Of Length 1
// c : Current Vertex On Surface
// l : Lightposition
// s : Direction Of s-Texture-Coordinate In Object Space (Must Be Normalized!)
// t : Direction Of t-Texture-Coordinate In Object Space (Must Be Normalized!)
void SetUpBumps(GLfloat *n, GLfloat *c, GLfloat *l, GLfloat *s, GLfloat *t) {
    GLfloat v[3];                               // Vector From Current Position To Light
    GLfloat lenQ;                               // Used To Normalize
    // Calculate v From Current Vertex c To Lightposition And Normalize v
    v[0]=l[0]-c[0];
    v[1]=l[1]-c[1];
    v[2]=l[2]-c[2];
    lenQ=(GLfloat) sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
    v[0]/=lenQ;
    v[1]/=lenQ;
    v[2]/=lenQ;
    // Project v Such That We Get Two Values Along Each Texture-Coordinate Axis
    c[0]=(s[0]*v[0]+s[1]*v[1]+s[2]*v[2])*maxEmboss; // c[0] = Dot product of S.V
    c[1]=(t[0]*v[0]+t[1]*v[1]+t[2]*v[2])*maxEmboss; // c[1] = Dot product of T.V
}


void display (void)   // Create The Display Function
{
    GLfloat c[4]={0.0f,0.0f,0.0f,1.0f};                 // Holds Current Vertex
    GLfloat n[4]={0.0f,0.0f,0.0f,1.0f};                 // Normalized Normal Of Current Surface
    GLfloat s[4]={0.0f,0.0f,0.0f,1.0f};                 // s-Texture Coordinate Direction, Normalized
    GLfloat t[4]={0.0f,0.0f,0.0f,1.0f};                 // t-Texture Coordinate Direction, Normalized
    GLfloat l[4];                               // Holds Our Lightposition To Be Transformed Into Object Space
    GLfloat MVMinv[16];                           // Holds The Inverted Modelview Matrix To Do So
    int i;
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
 
    // Build Inverse Modelview Matrix First. This Substitutes One Push/Pop With One glLoadIdentity();
    // Simply Build It By Doing All Transformations Negated And In Reverse Order
    glLoadIdentity();
    glRotatef(-yrot,0.0f,1.0f,0.0f);
    glRotatef(-xrot,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,0.0f,-zoom);
    glGetFloatv(GL_MODELVIEW_MATRIX,MVMinv);
    glLoadIdentity();
 
    glTranslatef(0.0f,0.0f,zoom);
    glRotatef(xrot,1.0f,0.0f,0.0f);
    glRotatef(yrot,0.0f,1.0f,0.0f);
 
    // Transform The Lightposition Into Object Coordinates:
    l[0]=LightPosition[0];
    l[1]=LightPosition[1];
    l[2]=LightPosition[2];
    l[3]=1.0f;                              // Homogenous Coordinate
    VMatMult(MVMinv,l); //Multiply the light vector by the inverse model view matrix.

    // TEXTURE-UNIT #0
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturemap[textureIndex]);
    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
    glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);
 
    if(bumps)
    {
        // TEXTURE-UNIT #1
        glActiveTextureARB(GL_TEXTURE1_ARB);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, normalmap[normalMapIndex]);
        glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
        glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD);
    }
 
    // General Switches
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    
    glBegin(GL_QUADS);
        // Front Face
        n[0]=0.0f;
        n[1]=0.0f;
        n[2]=1.0f;
        s[0]=1.0f;
        s[1]=0.0f;
        s[2]=0.0f;
        t[0]=0.0f;
        t[1]=1.0f;
        t[2]=0.0f;
        for (i=0; i<4; i++) 
        {
            c[0]=data[5*i+2];
            c[1]=data[5*i+3];
            c[2]=data[5*i+4];
            SetUpBumps(n,c,l,s,t);
            glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
            glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
        }
        // Back Face
        n[0]=0.0f;
        n[1]=0.0f;
        n[2]=-1.0f;
        s[0]=-1.0f;
        s[1]=0.0f;
        s[2]=0.0f;
        t[0]=0.0f;
        t[1]=1.0f;
        t[2]=0.0f;
        for (i=4; i<8; i++) 
        {
            c[0]=data[5*i+2];
            c[1]=data[5*i+3];
            c[2]=data[5*i+4];
            SetUpBumps(n,c,l,s,t);
            glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
            glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
        }
        // Top Face
        n[0]=0.0f;
        n[1]=1.0f;
        n[2]=0.0f;
        s[0]=1.0f;
        s[1]=0.0f;
        s[2]=0.0f;
        t[0]=0.0f;
        t[1]=0.0f;
        t[2]=-1.0f;
        for (i=8; i<12; i++) 
        {
            c[0]=data[5*i+2];
            c[1]=data[5*i+3];
            c[2]=data[5*i+4];
            SetUpBumps(n,c,l,s,t);
            glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
            glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
        }
        // Bottom Face
        n[0]=0.0f;
        n[1]=-1.0f;
        n[2]=0.0f;
        s[0]=-1.0f;
        s[1]=0.0f;
        s[2]=0.0f;
        t[0]=0.0f;
        t[1]=0.0f;
        t[2]=-1.0f;
        for (i=12; i<16; i++) 
        {
            c[0]=data[5*i+2];
            c[1]=data[5*i+3];
            c[2]=data[5*i+4];
            SetUpBumps(n,c,l,s,t);
            glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
            glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
        }
        // Right Face
        n[0]=1.0f;
        n[1]=0.0f;
        n[2]=0.0f;
        s[0]=0.0f;
        s[1]=0.0f;
        s[2]=-1.0f;
        t[0]=0.0f;
        t[1]=1.0f;
        t[2]=0.0f;
        for (i=16; i<20; i++) 
        {
            c[0]=data[5*i+2];
            c[1]=data[5*i+3];
            c[2]=data[5*i+4];
            SetUpBumps(n,c,l,s,t);
            glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
            glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
        }
        // Left Face
        n[0]=-1.0f;
        n[1]=0.0f;
        n[2]=0.0f;
        s[0]=0.0f;
        s[1]=0.0f;
        s[2]=1.0f;
        t[0]=0.0f;
        t[1]=1.0f;
        t[2]=0.0f;

        for (i=20; i<24; i++) 
        {
            c[0]=data[5*i+2];
            c[1]=data[5*i+3];
            c[2]=data[5*i+4];
            SetUpBumps(n,c,l,s,t);
            glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
            glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
        }
    glEnd();

    glActiveTextureARB(GL_TEXTURE1_ARB);
    glDisable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    
    if (!emboss) 
    {
        glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D,texturemap[textureIndex]);
        glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);
        glEnable(GL_BLEND);
        glEnable(GL_LIGHTING);
        doCube();
    }

    if (xrot>360.0f) xrot-=360.0f;
    if (xrot<0.0f) xrot+=360.0f;
    if (yrot>360.0f) yrot-=360.0f;
    if (yrot<0.0f) yrot+=360.0f;

    glutSwapBuffers ( );
    // Swap The Buffers To Not Be Left With A Clear Screen
}

void reshape ( int w, int h )   // Create The Reshape Function (the viewport)
{
  glViewport     ( 0, 0, w, h );
  glMatrixMode   ( GL_PROJECTION );  // Select The Projection Matrix
  glLoadIdentity ( );                // Reset The Projection Matrix
  if ( h==0 )  // Calculate The Aspect Ratio Of The Window
     gluPerspective ( 80, ( float ) w, 1.0, 5000.0 );
  else
     gluPerspective ( 80, ( float ) w / ( float ) h, 1.0, 5000.0 );
  glMatrixMode   ( GL_MODELVIEW );  // Select The Model View Matrix
  glLoadIdentity ( );    // Reset The Model View Matrix


}

void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{
    switch ( key ) {
    case 27:        // When Escape Is Pressed...
      exit ( 0 );   // Exit The Program
      break;        // Ready For Next Case
    case 113: //q
        zrot -= 2.5f;
        break;
    case 119: //w
        xrot += 2.5f;
        break;
    case 101: //e
        zrot += 2.5f;
        break;
    case 97: //a
        yrot -= 2.5f;
        break; 
    case 115: //s
        xrot -= 2.5f;
        break;
    case 100: //d
        yrot += 2.5f;
        break;
    case 120: //x
        zoom -= 0.5f;
        break;
    case 32: //space
        zoom += 0.5f;
        break;
    case 102: //f
        textureIndex = (textureIndex + 1)%NUM_TEXTURES; //Cycle through textures
        normalMapIndex = textureIndex;
        break;
    case 103: //g
        bumps=!bumps;
        break;
    case 104: //h
        if(maxEmboss < 1.0f)
        {
            maxEmboss = maxEmboss + 0.01f;
        }
        break;
    case 106: //j
        if(maxEmboss > 0.0f)
        {
            maxEmboss = maxEmboss - 0.01f;
        }
        break;
    case 114: //r
        normalMapIndex = (normalMapIndex + 1)%NUM_NORMAL_MAPS; //Cycle through normal maps
        break;
    case 116: //t
        emboss=!emboss;
        break;  
    default:        // Now Wrap It Up
      break;
  }
}

void arrow_keys ( int a_keys, int x, int y )  // Create Special Function (required for arrow keys)
{
  switch ( a_keys ) {
    case GLUT_KEY_UP:     // When Up Arrow Is Pressed...
      glutFullScreen ( ); // Go Into Full Screen Mode
      break;
    case GLUT_KEY_DOWN:               // When Down Arrow Is Pressed...
      glutReshapeWindow ( 500, 500 ); // Go Into A 500 By 500 Window
      break;
    default:
      break;
  }
}

int main(int argc, char** argv)   // Create Main Function For Bringing It All Together
{
  glutInit(&argc, argv); // Erm Just Write It =)
  
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Display Mode
  glutInitWindowSize(500, 500); // If glutFullScreen wasn't called this is the window size
  glutCreateWindow("NeHe's OpenGL Framework"); // Window Title (argv[0] for current directory as title)
  glutFullScreen();          // Put Into Full Screen
  
  init();

  glutDisplayFunc(display);  // Matching Earlier Functions To Their Counterparts
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(arrow_keys);

  glutIdleFunc(display);
  glutMainLoop();          // Initialize The Main Loop
  
  return 0;
}

