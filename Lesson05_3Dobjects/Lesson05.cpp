/*
3D objects:
1: This tutorial mostly concerns adding vertices to form objects in 3d-space.
2: Objects should always be drawn clockwise or counterclockwise, else holes will occur.
*/

#include <windows.h>   // Standard Header For Most Programs
#include <gl/gl.h>     // The GL Header File
#include <gl/glut.h>   // The GL Utility Toolkit (Glut) Header

GLfloat quadxrot = 0.0f;
GLfloat quadyrot = 0.0f;
GLfloat quadzrot = 0.0f;

GLfloat trixrot = 0.0f;
GLfloat triyrot = 0.0f;
GLfloat trizrot = 0.0f;

void init ( GLvoid )     // Create Some Everyday Functions
{

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glEnable ( GL_COLOR_MATERIAL );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void display (void)   // Create The Display Function
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	
	glLoadIdentity();									// Reset The Current Modelview Matrix
	
	glTranslatef(-1.5f,0.0f,-5.0f);						// Move Right 3 Units
	glRotatef(trixrot,1.0f,0.0f,0.0f);
	glRotatef(triyrot,0.0f,1.0f,0.0f);
	glRotatef(trizrot,0.0f,0.0f,1.0f);

	glBegin(GL_TRIANGLES);                  // Start Drawing The Pyramid
		//
		glColor3f(1.0f,0.0f,0.0f);          // Red
		glVertex3f( 0.0f, 1.0f, 0.0f);          // Top Of Triangle (Front)
		glColor3f(0.0f,1.0f,0.0f);          // Green
		glVertex3f(-1.0f,-1.0f, 1.0f);          // Left Of Triangle (Front)
		glColor3f(0.0f,0.0f,1.0f);          // Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);          // Right Of Triangle (Front)
		//
		glColor3f(1.0f,0.0f,0.0f);          // Red
		glVertex3f( 0.0f, 1.0f, 0.0f);          // Top Of Triangle (Right)
		glColor3f(0.0f,0.0f,1.0f);          // Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);          // Left Of Triangle (Right)
		glColor3f(0.0f,1.0f,0.0f);          // Green
		glVertex3f( 1.0f,-1.0f, -1.0f);         // Right Of Triangle (Right)
		//
		glColor3f(1.0f,0.0f,0.0f);          // Red
		glVertex3f( 0.0f, 1.0f, 0.0f);          // Top Of Triangle (Back)
		glColor3f(0.0f,1.0f,0.0f);          // Green
		glVertex3f( 1.0f,-1.0f, -1.0f);         // Left Of Triangle (Back)
		glColor3f(0.0f,0.0f,1.0f);          // Blue
		glVertex3f(-1.0f,-1.0f, -1.0f);         // Right Of Triangle (Back)
		//
		glColor3f(1.0f,0.0f,0.0f);          // Red
		glVertex3f( 0.0f, 1.0f, 0.0f);          // Top Of Triangle (Left)
		glColor3f(0.0f,0.0f,1.0f);          // Blue
		glVertex3f(-1.0f,-1.0f,-1.0f);          // Left Of Triangle (Left)
		glColor3f(0.0f,1.0f,0.0f);          // Green
		glVertex3f(-1.0f,-1.0f, 1.0f);          // Right Of Triangle (Left)
		//
		glColor3f(5.0f,5.0f,0.0f);          // Some Yellow
		glVertex3f(-1.0f,-1.0f, 1.0f);          // Top Of Triangle (Bottom1)
		glVertex3f(-1.0f,-1.0f,-1.0f);          // Left Of Triangle (Bottom1)
		glVertex3f( 1.0f,-1.0f,-1.0f);          // Right Of Triangle (Bottom1)
		//
		glVertex3f( 1.0f,-1.0f,-1.0f);          // Top Of Triangle (Bottom2)
		glVertex3f( 1.0f,-1.0f, 1.0f);          // Left Of Triangle (Bottom2)
		glVertex3f(-1.0f,-1.0f, 1.0f);          // Right Of Triangle (Bottom2)
	glEnd();                        // Done Drawing The Pyramid

	glLoadIdentity();									// Reset The Current Modelview Matrix
	
	glTranslatef(1.5f,0.0f,-7.0f);						// Move Right 3 Units
	glRotatef(quadxrot,1.0f,0.0f,0.0f);
	glRotatef(quadyrot,0.0f,1.0f,0.0f);
	glRotatef(quadzrot,0.0f,0.0f,1.0f);

	glBegin(GL_QUADS);									// Draw A Quad
		//Top
		glColor3f(1.0f,0.0f,0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f,-1.0f);          // Top Right Of The Quad (Top)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-1.0f);          // Top Left Of The Quad (Top)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);          // Bottom Left Of The Quad (Top)
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 1.0f);          // Bottom Right Of The Quad (Top)
		//Bottom
		glColor3f(0.0f,1.0f,0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,-1.0f, 1.0f);          // Top Right Of The Quad (Bottom)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,-1.0f, 1.0f);          // Top Left Of The Quad (Bottom)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);          // Bottom Left Of The Quad (Bottom)
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f,-1.0f);          // Bottom Right Of The Quad (Bottom)
		//Front
		glColor3f(0.0f,0.0f,1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f);          // Top Right Of The Quad (Front)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);          // Top Left Of The Quad (Front)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 1.0f);          // Bottom Left Of The Quad (Front)
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 1.0f);          // Bottom Right Of The Quad (Front)
		//Back
		glColor3f(5.0f,5.0f,0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,-1.0f,-1.0f);          // Bottom Left Of The Quad (Back)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,-1.0f,-1.0f);          // Bottom Right Of The Quad (Back)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f,-1.0f);          // Top Right Of The Quad (Back)
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f,-1.0f);          // Top Left Of The Quad (Back)
		//Left
		glColor3f(0.0f,0.5f,0.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);          // Top Right Of The Quad (Left)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-1.0f);          // Top Left Of The Quad (Left)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);          // Bottom Left Of The Quad (Left)
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 1.0f);          // Bottom Right Of The Quad (Left)
		//Right
		glColor3f(1.0f,0.2f,0.3f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f,-1.0f);          // Top Right Of The Quad (Right)
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f);          // Top Left Of The Quad (Right)
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 1.0f);          // Bottom Left Of The Quad (Right)
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f,-1.0f);          // Bottom Right Of The Quad (Right)
	glEnd();

	trixrot -= 0.03f;
	triyrot += 0.05f;
	trizrot += 0.02f;

	quadxrot += 0.02f;
	quadyrot -= 0.03f;
	quadzrot -= 0.05f;

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

  //NOTE: The call to the init function has been moved to this place. in lesson05
  //The result is that the drawn 3d objects does not hav holes.
  init();
  glutDisplayFunc(display);  // Matching Earlier Functions To Their Counterparts
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(arrow_keys);

  //NOTE: glutIdleFunc this tells openGL what function to call when "Idle".
  //Without this function the scene would not be redrawn!
  //FUNCTIONALITY: glutIdleFunc sets the global idle callback to be 
  //func so a GLUT program can perform background processing tasks or 
  //continuous animation when window system events are not being 
  //received. If enabled, the idle callback is continuously called 
  //when events are not being received.
  glutIdleFunc(display);
  glutMainLoop();          // Initialize The Main Loop

  return 0;
}

