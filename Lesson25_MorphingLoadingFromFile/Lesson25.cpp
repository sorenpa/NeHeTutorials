/*
Intro: This tutorial is about morphing. Morphing is simply just moving the vertices over time from a start to an end point.
       The start and point A for a vertex has a displacement vector from origin to A, and likewise for the end point B for
       the vertex. The displacement vector AB is then the total distance the vertex needs to move.


*/

#include <windows.h>	// Standard Header For Most Programs
#include <string.h>     // Header File For The String Library
#include <stdio.h>		//Standard IO header
#include <GL/glut.h>    // The GL Utility Toolkit (Glut) Header
#include "Object3D.h"
#include "ObjectLoader.h"

GLfloat LightAmbient[]  = { 0.2f, 0.2f, 0.2f};                  // Ambient Light Is 20% White
GLfloat LightDiffuse[]  = { 0.9f, 0.9f, 1.0f};                  // Diffuse Light Is White
GLfloat LightSpecular[]  = { 1.0f, 1.0f, 1.0f};  
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f};                  // Position Is Somewhat In Front Of Screen
GLfloat Gray[]      = { 0.5f, 0.5f, 0.5f, 1.0f};

GLfloat     xrot=0.0f,yrot=0.0f,zrot=0.0f,                             // X, Y & Z Rotation
            xspeed=0.0f,yspeed=0.0f,zspeed=0.0f,                           // X, Y & Z Spin Speed
            xpos=0.0f,ypos=0.0f,zpos=-7.0f;                               // X, Y & Z Position
 
int     key=1;                                  // Used To Make Sure Same Morph Key Is Not Pressed
int     step=0,steps=200;                           // Step Counter And Maximum Number Of Steps
bool    morph=FALSE;                                // Default morph To False (Not Morphing)

int targetObjectIndex;
Object3D  morpher;
Object3D  targetObjects[3];
GLfloat colours[3][3] = { {1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},{1.0f,1.0f,0.0f}};

void initLights(void) {
        glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);              // Load Light-Parameters into GL_LIGHT1
        glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
        glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
        //glEnable(GL_LIGHTING);
        //glEnable(GL_LIGHT1);
}

void initObjects(void)
{
    
    int obj1Size = objload("Data/Sphere.txt",&targetObjects[0]);                     // Load The First Object Into targetObjects[0] From File sphere.txt
    int obj2Size = objload("Data/Torus.txt",&targetObjects[1]);                      // Load The Second Object Into targetObjects[0] From File torus.txt
    int obj3Size = objload("Data/Tube.txt",&targetObjects[2]);                       // Load The Third Object Into targetObjects[0] From File tube.txt

    int maxSize = max(max(obj1Size,obj2Size),obj3Size);

    morpher.InitObject(maxSize);
    morpher.InitMorphSource();
    morpher.SetMorphSteps(1000);
    morpher.RandomizeVertices();
    
    targetObjectIndex = 0;
    morpher.SetColor(colours[0][0],colours[0][1],colours[0][2],1.0f);
    morpher.SetMorphTarget(&targetObjects[0]);
}

void init ( GLvoid )     // Create Some Everyday Functions
{
    //Init objects
    initObjects();
        
    //Init lights
    initLights();

    glEnable(GL_TEXTURE_2D);							// Enable colorMapping!
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
    glColor3f(0.0f,0.5f,0.5f);                          // Base Color
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glEnable ( GL_COLOR_MATERIAL );
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_CULL_FACE);

}


void display (void)   // Create The Display Function
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
    glLoadIdentity();
 
    glTranslatef(xpos,ypos,zpos);                             // Translate To The Current Position
    glRotatef(xrot,1,0,0);                              // Rotate On The X Axis By xrot
    glRotatef(yrot,0,1,0);                              // Rotate On The Y Axis By yrot
    glRotatef(zrot,0,0,1);                              // Rotate On The Z Axis By zrot

    morpher.Draw();

    xrot+=xspeed; yrot+=yspeed; zrot+=zspeed;                   // Increase xrot,yrot & zrot by 
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
        zspeed -= 0.001f;
        break;
    case 119: //w
        xspeed += 0.001f;
        break;
    case 101: //e
        zspeed += 0.001f;
        break;
    case 97: //a
        yspeed -= 0.001f;
        break; 
    case 115: //s
        xspeed -= 0.001f;
        break;
    case 100: //d
        yspeed += 0.001f;
        break;
    case 120: //x
        zpos -= 0.5f;
        break;
    case 32: //space
        zpos += 0.5f;
        break;
    case 102: //f
        
        targetObjectIndex = (targetObjectIndex +1)%3;
        morpher.SetMorphTarget(&targetObjects[targetObjectIndex]);
        morpher.SetColor(colours[targetObjectIndex][0],colours[targetObjectIndex][1],colours[targetObjectIndex][2],1.0f);

        break;
    case 103: //g
        morpher.StopMorphing();
        break;
    case 104: //h
        morpher.StartMorphing();
        break;
    case 106: //j
        break;
    case 114: //r
        morpher.RandomizeVertices();
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

