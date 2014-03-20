/*
Display Lists:
A display list is simply a data structure that is able to store objects. You can
prebuild the object and load it directly into memory with textures and everything.
This speeds up the drawing/display of the cube (or multiple cubes).

1: glGenLists(2) creates room for the two lists, and returns a pointer to the first list. 
   'box' will hold the location of the first list. Whenever we call box the first list will 
   be drawn.

2: glNewList([list],GL_COMPILE) is similar to programming. If you write a program, and load 
   it into your compiler, you have to compile it every time you want to run it. If it's already 
   compiled into an .EXE file, all you have to do is click on the .exe to run it. No compiling 
   needed. Once GL has compiled the display list, it's ready to go, no more compiling required. 
   This is where we get the speed boost from using display lists.

3: glEnable(GL_COLOR_MATERIAL); enables coloring of textures.

*/

#include <windows.h>   // Standard Header For Most Programs
#include <gl/gl.h>     // The GL Header File
#include <gl/glut.h>   // The GL Utility Toolkit (Glut) Header
#include <math.h>      // math lib

#include <GL/SOIL.h> //Image loading library header. Library also referenced by linker


//Movement
GLfloat walkspeed = 0.5f;
GLfloat turnspeed = 2.0f;
GLfloat xPos = 0.0f;
GLfloat yPos = 0.0f;
GLfloat zPos = 0.0f;
GLfloat yRot = 0.0f;
GLfloat xRot = 0.0f;
GLfloat heading = 0.0f;
const GLfloat piOver180 = 0.0174532925f;

//Textures. 
GLuint texture[4];
GLint filtering = 0;

//Lightning
GLboolean hasLightening = true;
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };              // Ambient Light Values
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };              // Diffuse Light Values
GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };             // Light Position

//Variables for the cubes in the display list
GLuint box; //storrage for the box
GLuint top; //storrage for the top of the box
static GLfloat boxcol[5][3]=                                // Array For Box Colors
{
    // Bright:  Red, Orange, Yellow, Green, Blue
    {1.0f,0.0f,0.0f},{1.0f,0.5f,0.0f},{1.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,1.0f,1.0f}
};
 
static GLfloat topcol[5][3]=                                // Array For Top Colors
{
    // Dark:  Red, Orange, Yellow, Green, Blue
    {.5f,0.0f,0.0f},{0.5f,0.25f,0.0f},{0.5f,0.5f,0.0f},{0.0f,0.5f,0.0f},{0.0f,0.5f,0.5f}
};

void BuildDisplayList(void)
{
	//Create memspace for 2 lists. Box has a pointer to the first lsit
	box=glGenLists(2);                              // Building Two Lists

	//Begin a new compiled list, stored in "box".
	glNewList(box,GL_COMPILE);                          // New Compiled box Display List
		//Draw the cube
		glBegin(GL_QUADS);									// Draw A Quad
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
		glEnd(); //End draw the cube
	glEndList(); //End the difinition of the list.

	//Make top point to the second list.
	top = box+1;

	glNewList(top,GL_COMPILE);
		glBegin(GL_QUADS);
			//Top
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f,-1.0f);          // Top Right Of The Quad (Top)
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-1.0f);          // Top Left Of The Quad (Top)
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);          // Bottom Left Of The Quad (Top)
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 1.0f);          // Bottom Right Of The Quad (Top)
		glEnd();
	glEndList();
}

int LoadGLTextures()                                    // Load Bitmaps And Convert To Textures
{
    /* load an image file directly as a new OpenGL texture */
	for(int i = 0; i < 3;i++)
	{
		texture[i] = SOIL_load_OGL_texture
        (
        "Data/Crate.tga",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
	}
 
	texture[3] = SOIL_load_OGL_texture
        (
        "Data/Cube.bmp",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );

    if(texture[0] == 0||texture[1] == 0||texture[2] == 0)
        return false;

    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
 
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

    return true;                                        // Return Success
	
}

void init ( GLvoid )     // Create Some Everyday Functions
{
	//Load the textures with SOIL
	LoadGLTextures();

	//Call our function that builds the 2 displaylists
	BuildDisplayList();
	
	glEnable(GL_TEXTURE_2D);							// Enable Texturemapping!
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glEnable ( GL_COLOR_MATERIAL );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Lightning GL_LIGHT1 setup (ambient,diffuse, position)
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);             // Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);             // Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);            // Position The Light
	
	glEnable(GL_LIGHT1);

	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);                            // Enable Material Coloring
}

void display (void)   // Create The Display Function
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer

  	glLoadIdentity();	// Reset The Current Modelview Matrix
	
	//translate world relating to camera position.
	glRotatef(360.0f-yRot,0.0f,1.0f,0.0f);
	glTranslatef(-xPos,yPos,-zPos);

	//Translate the original object position
	glTranslatef(0.0f,0.0f,-5.0f);

	//Bind texture 4 (Cube.bmp)
	glBindTexture(GL_TEXTURE_2D, texture[3]);               // Select Our Texture
	//Set the color and alpha value
	glColor4f(1.0f,0.0f,0.0f,0.5f);         // Full Brightness, 50% Alpha
	glCallList(box);
	glCallList(top);

	glTranslatef(-2.0f,0.0f,2.0f);
	glColor4f(0.0f,0.0f,1.0f,0.5f);         // Full Brightness, 50% Alpha
	glCallList(box);
	glCallList(top);

	glTranslatef(0.0f,0.0f,-2.0f);
	glColor4f(0.0f,1.0f,1.0f,0.5f);         // Full Brightness, 50% Alpha
	glCallList(box);
	glCallList(top);

	glTranslatef(0.0f,2.0f,0.0f);
	glColor4f(0.0f,1.0f,0.0f,0.5f);         // Full Brightness, 50% Alpha
	glCallList(box);
	glCallList(top);

	//Reset the matrix
	glLoadIdentity();
	glColor4f(1.0f,1.0f,1.0f,0.5f);         // Full Brightness, 50% Alpha
	//translate world relating to camera position.
	glRotatef(360.0f-yRot,0.0f,1.0f,0.0f);
	glTranslatef(-xPos,yPos,-zPos);
	//Draw the "world"
	glBindTexture(GL_TEXTURE_2D, texture[filtering]);               // Select Our Texture
	glBegin(GL_QUADS);									// Draw A Quad
		//floor
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 20.0f, -1.0f,-20.0f);          // Top Right Of The Quad (Top)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-20.0f, -1.0f,-20.0f);          // Top Left Of The Quad (Top)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-20.0f, -1.0f, 20.0f);          // Bottom Left Of The Quad (Top)
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 20.0f, -1.0f, 20.0f);          // Bottom Right Of The Quad (Top)
		//Cieling
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 20.0f, 10.0f,-20.0f);          // Top Right Of The Quad (Top)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-20.0f, 10.0f,-20.0f);          // Top Left Of The Quad (Top)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-20.0f, 10.0f, 20.0f);          // Bottom Left Of The Quad (Top)
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 20.0f, 10.0f, 20.0f);          // Bottom Right Of The Quad (Top)
		//SideWall11
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-20.0f, -1.0f,-5.0f);          // Top Right Of The Quad (Top)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-20.0f, 10.0f,-5.0f);          // Top Left Of The Quad (Top)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-20.0f, 10.0f,-20.0f);          // Bottom Left Of The Quad (Top)
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-20.0f, -1.0f,-20.0f);          // Bottom Right Of The Quad (Top)
		//SideWall12
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-20.0f, -1.0f,-20.0f);          // Top Right Of The Quad (Top)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-20.0f, 10.0f,-20.0f);          // Top Left Of The Quad (Top)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0f, 10.0f,-20.0f);          // Bottom Left Of The Quad (Top)
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0f, -1.0f,-20.0f);          // Bottom Right Of The Quad (Top)
		//SideWall21
		glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0f, -1.0f,-5.0f);          // Top Right Of The Quad (Top)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(20.0f, 10.0f,-5.0f);          // Top Left Of The Quad (Top)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(20.0f, 10.0f,-20.0f);          // Bottom Left Of The Quad (Top)
		glTexCoord2f(1.0f, 0.0f); glVertex3f(20.0f, -1.0f,-20.0f);          // Bottom Right Of The Quad (Top)
		//SideWall22
		glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0f, -1.0f,-20.0f);          // Top Right Of The Quad (Top)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(20.0f, 10.0f,-20.0f);          // Top Left Of The Quad (Top)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0f, 10.0f,-20.0f);          // Bottom Left Of The Quad (Top)
		glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0f, -1.0f,-20.0f);          // Bottom Right Of The Quad (Top)
		//SideWall31
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-20.0f, -1.0f,5.0f);          // Top Right Of The Quad (Top)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-20.0f, 10.0f,5.0f);          // Top Left Of The Quad (Top)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-20.0f, 10.0f,20.0f);          // Bottom Left Of The Quad (Top)
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-20.0f, -1.0f,20.0f);          // Bottom Right Of The Quad (Top)
		//SideWall32
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-20.0f, -1.0f,20.0f);          // Top Right Of The Quad (Top)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-20.0f, 10.0f,20.0f);          // Top Left Of The Quad (Top)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0f, 10.0f,20.0f);          // Bottom Left Of The Quad (Top)
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0f, -1.0f,20.0f);          // Bottom Right Of The Quad (Top)
		//SideWall41
		glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0f, -1.0f,5.0f);          // Top Right Of The Quad (Top)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(20.0f, 10.0f,5.0f);          // Top Left Of The Quad (Top)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(20.0f, 10.0f,20.0f);          // Bottom Left Of The Quad (Top)
		glTexCoord2f(1.0f, 0.0f); glVertex3f(20.0f, -1.0f,20.0f);          // Bottom Right Of The Quad (Top)
		//SideWall42
		glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0f, -1.0f,20.0f);          // Top Right Of The Quad (Top)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(20.0f, 10.0f,20.0f);          // Top Left Of The Quad (Top)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0f, 10.0f,20.0f);          // Bottom Left Of The Quad (Top)
		glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0f, -1.0f,20.0f);          // Bottom Right Of The Quad (Top)
	glEnd();

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
	
	case 119: //w
		xPos -= sin(heading*piOver180)*walkspeed;
		zPos -= cos(heading*piOver180)*walkspeed;
		break;
	case 97: //a
		heading += turnspeed;
		yRot = heading;
		break; 
	case 115: //s
		xPos += sin(yRot*piOver180)*walkspeed;
		zPos += cos(yRot*piOver180)*walkspeed;
		break;
	case 100: //d
		heading -= turnspeed;
		yRot = heading;
		break;
    case 120: //x
	
		break;
	case 32: //space
	
		break;
	case 102: //r
		filtering = (filtering + 1)%3;
		break;
	case 114: //f
		if(hasLightening)
		{
			glDisable(GL_LIGHT1);
			hasLightening = false;
		}
		else
		{
			glEnable(GL_LIGHT1);
			hasLightening = true;
		}
		
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

