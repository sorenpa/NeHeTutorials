
#include <windows.h>		// Header File For Windows
#include <GL\GL.h>			// Header File For The OpenGL32 Library
#include <GL\GLU.h>			// Header File For The GLu32 Library
#include <GL\glut.h>		// Header File For The Glut Library


void Draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
		glVertex3f(0.25, 0.25, 0.0);
		glVertex3f(0.75, 0.75, 0.0);
	glEnd();
	glFlush();
}

void Initialize() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int iArgc, char** cppArgv) {
	 glutInit( &iArgc, cppArgv ); // Erm Just Write It =)
  Initialize();
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE ); // Display Mode
  glutInitWindowSize( 500, 500 ); // If glutFullScreen wasn't called this is the window size
  glutCreateWindow( "NeHe's OpenGL Framework" ); // Window Title (argv[0] for current directory as title)
  //glutFullScreen( );          // Put Into Full Screen
  glutDisplayFunc(Draw);  // Matching Earlier Functions To Their Counterparts

  glutMainLoop();          // Initialize The Main Loop
}