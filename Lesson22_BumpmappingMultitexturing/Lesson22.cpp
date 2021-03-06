/*
Nromalmapping (aka. Bumpmapping):
1:The basic idea behind a normal map, is to make a flat surface look more realistic by telling
the OpenGL how light should be reflected by that surface. Without a normal map the light for
a surface is calculated from the normal vector perpendicular to the surface, and a vector from
the vertices of that surface to the light source(s). 

2: A normal map is a "texture" that stores normal vectors per pixel of the surface. The vectors are
store as RGB values in an image, where R=x component, G=y component and B=Z component of the normal.
Since the normal always points away from the surface, and the normal is expressed in the "tangent-space"
(a vector space where the x,y plane is the surface it selt), then the z component (B) is dominant.
Thus normal maps are bluish in colour when viewing at them in a 2D image processing program.

3:As previously mentioned, the normals in the normal map is expressed in the tangent space. This hints
thar we have a basis for this vector space (ofcourse we do). What it also suggest is that normal
mapping involves multiple coordinate systems. In fact the below code involves World,Model and Tangent
spaces, and in order to change between them we use the inverse of the model view matrix, to change from
world to object space, and the inverse TBN-matrix (Tangent, Tinormal, Normal which are the basis vectors for 
the tangent space - changing per vertex), to change from model to surface coordinates.

4: The modern way of applying a normal map to the scene is by using shaders, which we will look at in
an extension to NeHe lesson22.
*/

#include <windows.h>	// Standard Header For Most Programs
#include <string.h>     // Header File For The String Library
#include <stdio.h>		//Standard IO header
#include <GL/glew.h>    //header for GLEW (GL extensions loading wrangler library)
#include <GL/glut.h>    // The GL Utility Toolkit (Glut) Header
#include <GL/SOIL.h>	//Image loading library header. Library also referenced by linker
#include "mathlib.h"    //Header file for a custom math library
#include "Cube.h";      //Header file for a cube object

#define NUM_TEXTURES 7
#define NUM_NORMAL_MAPS 7

bool extensionsSupported=true;  //Is the extensions used bu this tutorial supported
GLint maxTexelUnits=1;  		// Number Of Texel-Pipelines. This Is At Least 1.

bool IsLightEnabled = true;
bool    IsBumpEnabled=true;													// Do Bumpmapping?
GLuint  textureMapIndex=0;                               // Index of the texture used
GLuint  normalMapIndex=0;                               // Stores the index of the normal map used
GLuint  textureMap[NUM_TEXTURES];                             // Storage For 3 Textures
GLuint  normalMap[NUM_NORMAL_MAPS];                                // Our Bumpmappings

GLfloat LightAmbient[]  = { 0.2f, 0.2f, 0.2f};                  // Ambient Light Is 20% White
GLfloat LightDiffuse[]  = { 1.0f, 1.0f, 1.0f};                  // Diffuse Light Is White
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f};                  // Position Is Somewhat In Front Of Screen
GLfloat Gray[]      = { 0.5f, 0.5f, 0.5f, 1.0f};

GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat zrot = 0.0f;
GLfloat zoom = 5.0f;

Cube cube = Cube(2,2,2);
Cube cube2 = Cube(3,3,3);



bool CheckSystemCompatibility()
{
    //Check that multitexture extenstion is available run time.
    if (GLEW_ARB_multitexture && 
        GLEW_ARB_texture_cube_map && 
        GLEW_ARB_texture_env_combine &&
        GLEW_ARB_texture_env_dot3)
    {
        return true;
    }

    return false;
}

int LoadGLTextures()                                    // Load Bitmaps And Convert To Textures
{
    

    textureMap[0] = SOIL_load_OGL_texture
    (
       "Data/Texture1.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, textureMap[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    textureMap[1] = SOIL_load_OGL_texture
    (
       "Data/Texture2.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, textureMap[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    textureMap[2] = SOIL_load_OGL_texture
    (
       "Data/Texture3.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, textureMap[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    textureMap[3] = SOIL_load_OGL_texture
    (
       "Data/Texture4.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

        // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, textureMap[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
    textureMap[4] = SOIL_load_OGL_texture
    (
       "Data/Texture5.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, textureMap[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    textureMap[5] = SOIL_load_OGL_texture
    (
       "Data/Texture6.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, textureMap[5]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    textureMap[6] = SOIL_load_OGL_texture
    (
       "Data/Texture7.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, textureMap[6]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    normalMap[0] = SOIL_load_OGL_texture
    (
       "Data/Normal1.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, normalMap[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    normalMap[1] = SOIL_load_OGL_texture
    (
       "Data/Normal2.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, normalMap[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    normalMap[2] = SOIL_load_OGL_texture
    (
       "Data/Normal3.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, normalMap[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    normalMap[3] = SOIL_load_OGL_texture
    (
       "Data/Normal4.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, normalMap[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    normalMap[4] = SOIL_load_OGL_texture
    (
       "Data/Normal5.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, normalMap[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    normalMap[5] = SOIL_load_OGL_texture
    (
       "Data/Normal6.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, normalMap[5]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    normalMap[6] = SOIL_load_OGL_texture
    (
       "Data/Normal7.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, normalMap[6]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   
    for(int i = 0;i<NUM_TEXTURES;i++)
    {
        if(normalMap[i] == 0)
        {
            return false;
        }
    }

        for(int i = 0;i<NUM_TEXTURES;i++)
    {
        if(textureMap[i] == 0)
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
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT1);
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
    if(extensionsSupported = CheckSystemCompatibility())
    {
        IsBumpEnabled = true; 
    }

    //Load the textures with SOIL
    LoadGLTextures();
    
    //Init lights
    initLights();

    glEnable(GL_TEXTURE_2D);							// Enable textureMapping!
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
    glColor3f(0.5f,0.5f,0.5f);                          // Base Color
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    //glEnable ( GL_COLOR_MATERIAL );
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_CULL_FACE);
 
    //initialize cubes
    cube.m_drawColorMap = true;
    cube.m_drawNormalMap = true;
    cube2.m_drawColorMap = true;
    cube2.m_drawNormalMap = true;
    cube2.m_rotation.x = 25.0f;
    cube2.m_rotation.y = 10.0f;
    cube2.m_rotation.z = 0.0f;
}


void display (void)   // Create The Display Function
{

    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
    glLoadIdentity();
    


    //Setup rotation and light for the cube.
    cube.m_posistion = Vector3f(0.0f,0.0f,-zoom);
    cube.m_colorMap = textureMap[textureMapIndex];
    cube.m_normalMap = normalMap[normalMapIndex];
    
    //Could probably be fetched from the cube object draw function by calling a GL function
    cube.m_worldSpaceLightPosition = Vector4f(LightPosition[0], LightPosition[1], LightPosition[2], 1.0f);
    cube.Draw();

    glLoadIdentity();									// Reset The Current Modelview Matrix

    //Setup rotation and light for the cube.
    cube2.m_posistion = Vector3f(-3.0f,2.0f,-7.0f);
    cube2.m_colorMap = textureMap[textureMapIndex];
    cube2.m_normalMap = normalMap[normalMapIndex];
    
    //Could probably be fetched from the cube object draw function by calling a GL function
    cube2.m_worldSpaceLightPosition = Vector4f(LightPosition[0], LightPosition[1], LightPosition[2], 1.0f);
    cube2.Draw();

    glLoadIdentity();									// Reset The Current Modelview Matrix
    
    
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
        cube.m_rotationSpeed.z -= 0.001f;
        cube2.m_rotationSpeed.z += 0.001f;
        break;
    case 119: //w
        cube.m_rotationSpeed.x += 0.001f;
        cube2.m_rotationSpeed.x -= 0.001f;
        break;
    case 101: //e
        cube.m_rotationSpeed.z += 0.001f;
        cube2.m_rotationSpeed.z -= 0.001f;
        break;
    case 97: //a
        cube.m_rotationSpeed.y -= 0.001f;
        cube2.m_rotationSpeed.y += 0.001f;
        break; 
    case 115: //s
        cube.m_rotationSpeed.x -= 0.001f;
        cube2.m_rotationSpeed.x += 0.001f;
        break;
    case 100: //d
        cube.m_rotationSpeed.y += 0.001f;
        cube2.m_rotationSpeed.y -= 0.001f;
        break;
    case 120: //x
        zoom -= 0.5f;
        break;
    case 32: //space
        zoom += 0.5f;
        break;
    case 102: //f
        textureMapIndex = (textureMapIndex + 1)%NUM_TEXTURES; //Cycle through textures
        normalMapIndex = textureMapIndex;
        break;
    case 103: //g
        cube.m_drawNormalMap = !cube.m_drawNormalMap;
        cube2.m_drawNormalMap = !cube2.m_drawNormalMap;
        break;
    case 104: //h
        //Stop rotation
        cube.m_rotationSpeed.x = 0.0f;
        cube.m_rotationSpeed.y = 0.0f;
        cube.m_rotationSpeed.z = 0.0f;

        //Reset rotations
        cube.m_rotation.x = 0.0f;
        cube.m_rotation.y = 0.0f;
        cube.m_rotation.z = 0.0f;

        //Stop rotation
        cube2.m_rotationSpeed.x = 0.0f;
        cube2.m_rotationSpeed.y = 0.0f;
        cube2.m_rotationSpeed.z = 0.0f;

        //Reset rotations
        cube2.m_rotation.x = 0.0f;
        cube2.m_rotation.y = 0.0f;
        cube2.m_rotation.z = 0.0f;
        break;
    case 106: //j
        break;
    case 114: //r
        normalMapIndex = (normalMapIndex + 1)%NUM_NORMAL_MAPS; //Cycle through normal maps
        break;
    case 116: //t
        cube.m_drawColorMap = !cube.m_drawColorMap;
        cube2.m_drawColorMap = !cube2.m_drawColorMap;
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

