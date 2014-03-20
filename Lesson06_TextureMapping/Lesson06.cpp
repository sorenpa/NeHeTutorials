/*
Textures are mappend to an object with the follwoing steps:
1: Enable texture mapping (glEnable("GL_TEXTURE_2D");)
2: First load the texture. In this lesson an external library (SOIL) is used for loading the texture.
3: Bind/select the texture you want to work with (like selecting a color)
4: Setup the texture parameters (filtering etc)
5: Map the texture to the object of choice (glTextCoord2f(x,y)) 
*/

#include <windows.h>   // Standard Header For Most Programs
#include <gl/gl.h>     // The GL Header File
#include <gl/glut.h>   // The GL Utility Toolkit (Glut) Header

#include <GL/SOIL.h> //Image loading library header. Library also referenced by linker

GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat zrot = 0.0f;

//Textures. 
GLuint texture[1];






int LoadGLTextures()                                    // Load Bitmaps And Convert To Textures
{
    
		texture[0] = SOIL_load_OGL_texture
        (
        "Data/NeHe.bmp",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
	
 
    if(texture[0] == 0)
        return false;

    // Typical Texture Generation Using Data From The Bitmap
 	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    return true;                                        // Return Success
	
}

void init ( GLvoid )     // Create Some Everyday Functions
{
	//Load the textures with SOIL
	LoadGLTextures();
	
	glEnable(GL_TEXTURE_2D);							// Enable Texturemapping!
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
	
	glTranslatef(0.0f,0.0f,-5.0f);						// Move Right 3 Units
	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);
	glRotatef(zrot,0.0f,0.0f,1.0f);

	

	/*************************************************************
	 * NOTE:
	 * glTexCoord2f maps the texture to a vertex. When mapping onto a
	 * Quad, you basically should map top right (0,0) of the texture to
	 * the top right of the quad! Top left to top left and so on. Failing
	 * this will result in mirrored/flipped texture mapping or simply no
	 * texture mapping at all.
	 **************************************************************/
	glBindTexture(GL_TEXTURE_2D, texture[0]);               // Select Our Texture
	glBegin(GL_QUADS);									// Draw A Quad
		//Top
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f,-1.0f);          // Top Right Of The Quad (Top)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-1.0f);          // Top Left Of The Quad (Top)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);          // Bottom Left Of The Quad (Top)
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 1.0f);          // Bottom Right Of The Quad (Top)
		//Bottom
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,-1.0f, 1.0f);          // Top Right Of The Quad (Bottom)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,-1.0f, 1.0f);          // Top Left Of The Quad (Bottom)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);          // Bottom Left Of The Quad (Bottom)
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f,-1.0f);          // Bottom Right Of The Quad (Bottom)
		//Front
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f);          // Top Right Of The Quad (Front)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);          // Top Left Of The Quad (Front)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 1.0f);          // Bottom Left Of The Quad (Front)
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 1.0f);          // Bottom Right Of The Quad (Front)
		//Back
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,-1.0f,-1.0f);          // Bottom Left Of The Quad (Back)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,-1.0f,-1.0f);          // Bottom Right Of The Quad (Back)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f,-1.0f);          // Top Right Of The Quad (Back)
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f,-1.0f);          // Top Left Of The Quad (Back)
		//Left
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);          // Top Right Of The Quad (Left)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-1.0f);          // Top Left Of The Quad (Left)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);          // Bottom Left Of The Quad (Left)
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 1.0f);          // Bottom Right Of The Quad (Left)
		//Right
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f,-1.0f);          // Top Right Of The Quad (Right)
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f);          // Top Left Of The Quad (Right)
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 1.0f);          // Bottom Left Of The Quad (Right)
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f,-1.0f);          // Bottom Right Of The Quad (Right)
	glEnd();

	xrot += 0.02f;
	yrot -= 0.03f;
	zrot -= 0.05f;

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

