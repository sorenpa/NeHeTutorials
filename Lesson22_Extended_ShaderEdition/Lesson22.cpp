/*
Intro: This solution is an extension to the NeHe lesson 22 on bump/normal mapping. The NeHe tutorial use an emboss
Technique to do the normal mapping, and further performs the multitexturing in C++ code. The modern way to map
is using programmable shaders and the GLSL language.

Shaders: A shader is a programmable part of the graphics pipeline. With a shader it is thus possible to alter
the way the graphics hardware handles the different parts of the pipeline. Since we are controlling the hardware
the usage of shaders can speed up the performance of code and ofcourse some things is easier to do. In this extended
lesson we are going to program 2 shaders. The Vertex shader and the Fragment shader.

Vertex Shader: A vertex shader is as the name dictates a program that can alter a vertex. The vertex shader is a
program that acts on each vertex passed to it, from previious steps of the graphics pipeline. The vertex shader can
mainly alter the position of vertices.

Fragment Shader: A fragment shader is performed at a later stage than the Vertex Shader. The fragment shader is a small
program that acts on fragments. A fragment is in essence a pixel, but it can hold additional data than just the color, and
thus it is given a different name. So a fragment shader is run once per pixel.

*/

#include <windows.h>	// Standard Header For Most Programs
#include <string.h>     // Header File For The String Library
#include <stdio.h>		//Standard IO header
#include <GL/glew.h>    //header for GLEW (GL extensions loading wrangler library)
#include <GL/glut.h>    // The GL Utility Toolkit (Glut) Header
#include <GL/SOIL.h>	//Image loading library header. Library also referenced by linker
#include "ShaderUtilities.h"
#include "Cube.h"      //Header file for a cube object

#define NUM_TEXTURES 8
#define NUM_NORMAL_MAPS 8

bool b_extensionsSupported=true;  //Is the extensions used bu this tutorial supported
GLint maxTexelUnits=1;  		// Number Of Texel-Pipelines. This Is At Least 1.

GLuint vertexShader; //Storage of the vertex shader ID
GLuint fragmentShader; //Storage of the fragment shader ID
GLuint shaderProgram; //Storage of the shader program ID

GLuint  colorMapIndex=0;                               // Index of the texture used
GLuint  normalMapIndex=0;                               // Stores the index of the normal map used
GLuint  colorMap[NUM_TEXTURES];                             // Storage For 3 Textures
GLuint  normalMap[NUM_NORMAL_MAPS];                                // Our Bumpmappings

GLfloat LightAmbient[]  = { 0.2f, 0.2f, 0.2f};                  // Ambient Light Is 20% White
GLfloat LightDiffuse[]  = { 0.9f, 0.9f, 1.0f};                  // Diffuse Light Is White
GLfloat LightSpecular[]  = { 1.0f, 1.0f, 1.0f};  
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f};                  // Position Is Somewhat In Front Of Screen
GLfloat Gray[]      = { 0.5f, 0.5f, 0.5f, 1.0f};

GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat zrot = 0.0f;
GLfloat zoom = 5.0f;

Cube cube = Cube(2,2,2);


//Checks OpenGL version and extensions run time
bool CheckSystemCompatibility()
{
    //Check that multitexture extenstion is available run time.
    if (glewIsSupported("GL_VERSION_2_0") && 
        GLEW_ARB_vertex_shader && //Check for vertex shader support
        GLEW_ARB_fragment_shader) //Check for fragment shader support
    {
        return true;
    }

    return 0;
}

int LoadGLTextures()                                    // Load Bitmaps And Convert To Textures
{
    colorMap[0] = SOIL_load_OGL_texture
    (
       "Data/Texture1.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, colorMap[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    colorMap[1] = SOIL_load_OGL_texture
    (
       "Data/Texture2.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, colorMap[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    colorMap[2] = SOIL_load_OGL_texture
    (
       "Data/Texture3.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, colorMap[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    colorMap[3] = SOIL_load_OGL_texture
    (
       "Data/Texture4.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

        // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, colorMap[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
    colorMap[4] = SOIL_load_OGL_texture
    (
       "Data/Texture5.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, colorMap[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    colorMap[5] = SOIL_load_OGL_texture
    (
       "Data/Texture6.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, colorMap[5]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    colorMap[6] = SOIL_load_OGL_texture
    (
       "Data/Texture7.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, colorMap[6]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    colorMap[7] = SOIL_load_OGL_texture
    (
       "Data/Texture8.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, colorMap[7]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    normalMap[0] = SOIL_load_OGL_texture
    (
       "Data/Normal1.png",
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

    normalMap[7] = SOIL_load_OGL_texture
    (
       "Data/Normal8.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, normalMap[7]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    for(int i = 0;i<NUM_TEXTURES;i++)
    {
        if(colorMap[i] == 0)
        {
            return false;
        }
    }

    for(int i = 0;i<NUM_TEXTURES;i++)
    {
        if(normalMap[i] == 0)
        {

            return false;
        }
    }
    
    return true;                                        // Return Success
    
}

void initShaders()
{
    
    GenerateShader("Shaders/NormalMap.vert",GL_VERTEX_SHADER, vertexShader);
    GenerateShader("Shaders/NormalMap.frag",GL_FRAGMENT_SHADER, fragmentShader);
    GenerateShaderProgram(vertexShader,fragmentShader,shaderProgram);
}

void initLights(void) {
        glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);              // Load Light-Parameters into GL_LIGHT1
        glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
        glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT1);
}


void init ( GLvoid )     // Create Some Everyday Functions
{
    printf("Initializing glew");
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(1);
    }
    else
    {
        printf(" - Success\n");
    }


    //Check extension availability runtime
    printf("Checking OpenGL extensions");
    if(CheckSystemCompatibility())
    {
        printf(" - Success\n");
    }
    else
    {
        printf("The runtime system eithe does not supprt OpenGL 2.0 or required extensions not supported.\n");
        exit(1);
    }
    
    //Load the textures with SOIL
    printf("Loading textures");
    if(LoadGLTextures())
    {
        printf(" - Success\n");
    }
    else
    {
        printf(" - Failed\n");
    }
    
    initShaders();

    //Init lights
    initLights();

    glEnable(GL_TEXTURE_2D);							// Enable colorMapping!
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
    glColor3f(0.5f,0.5f,0.5f);                          // Base Color
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    //glEnable ( GL_COLOR_MATERIAL );
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_CULL_FACE);

}


void display (void)   // Create The Display Function
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
    glLoadIdentity();

    cube.m_objectPosistion[2] = -zoom;
    cube.SetColorMap(colorMap[colorMapIndex]);
    cube.SetNormalMap(normalMap[normalMapIndex]);
    cube.SetShader(shaderProgram);
    cube.Draw();
 
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
        cube.m_rotationSpeed[2] -= 0.001f;
        break;
    case 119: //w
        cube.m_rotationSpeed[0] += 0.001f;
        break;
    case 101: //e
        cube.m_rotationSpeed[2] += 0.001f;
        break;
    case 97: //a
        cube.m_rotationSpeed[1] -= 0.001f;
        break; 
    case 115: //s
        cube.m_rotationSpeed[0] -= 0.001f;

        break;
    case 100: //d
        cube.m_rotationSpeed[1] += 0.001f;

        break;
    case 120: //x
        zoom -= 0.5f;
        break;
    case 32: //space
        zoom += 0.5f;
        break;
    case 102: //f
        colorMapIndex = (colorMapIndex + 1)%NUM_TEXTURES; //Cycle through textures
        normalMapIndex = colorMapIndex;
        break;
    case 103: //g


        break;
    case 104: //h
        //Stop rotation
        cube.m_rotationSpeed[0] = 0.0f;
        cube.m_rotationSpeed[1] = 0.0f;
        cube.m_rotationSpeed[2] = 0.0f;

        //Reset rotations
        cube.m_rotation[0] = 0.0f;
        cube.m_rotation[1] = 0.0f;
        cube.m_rotation[2] = 0.0f;
        break;
    case 106: //j
        break;
    case 114: //r
        break;
    case 116: //t
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
  //glutFullScreen();          // Put Into Full Screen
  
  init();

  glutDisplayFunc(display);  // Matching Earlier Functions To Their Counterparts
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(arrow_keys);

  glutIdleFunc(display);
  glutMainLoop();          // Initialize The Main Loop
  
  return 0;
}

