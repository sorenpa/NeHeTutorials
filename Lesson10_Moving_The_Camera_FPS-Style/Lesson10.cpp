/*
Blending:
1: Upuntil now we have used a function with 3 inputs to colour our objects. but
   OpenGL provides a function glColor4f that has a 4th input parameter (alpha).
   This alpha value specifies the "opacity" of the object, where 0.0 is totally 
   transparent and 1.0 is totally solid.

2: Most special effects in OpenGL rely on some type of blending. Blending 
   is used to combine the color of a given pixel that is about to be drawn with 
   the pixel that is already on the screen. How the colors are combined is based 
   on the alpha value of the colors, and/or the blending function that is being 
   used. Alpha is a 4th color component usually specified at the end

3: Math time!:
   
   (Rs Sr + Rd Dr, Gs Sg + Gd Dg, Bs Sb + Bd Db, As Sa + Ad Da)

	OpenGL will calculate the result of blending two pixels based on the above equation. 
	The s and d subscripts specify the source and destination pixels. The S and D components 
	are the blend factors. These values indicate how you would like to blend the pixels. 
	The most common values for S and D are (As, As, As, As) (AKA source alpha) for S and 
	(1, 1, 1, 1) - (As, As, As, As) (AKA one minus src alpha) for D. This will yield a 
	blending equation that looks like this:

	(Rs As + Rd (1 - As), Gs As + Gd (1 - As), Bs As + Bd (1 - As), As As + Ad (1 - As))

	This equation will yield transparent/translucent style effects.

4: We enable blending just like everything else. Then we set the equation, and turn off 
   depth buffer writing when drawing transparent objects, since we still want objects behind 
   the translucent shapes to be drawn. This isn't the proper way to blend, but most the time 
   in simple projects it will work fine. Rui Martins Adds: The correct way is to draw all the 
   transparent (with alpha < 1.0) polys after you have drawn the entire scene, and to draw them 
   in reverse depth order (farthest first). This is due to the fact that blending two polygons 
   (1 and 2) in different order gives different results, i.e. (assuming poly 1 is nearest to 
   the viewer, the correct way would be to draw poly 2 first and then poly 1

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
        "Data/NeHe.bmp",
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

	//Bind texture 4 (NeHe.bmp)
	glBindTexture(GL_TEXTURE_2D, texture[3]);               // Select Our Texture
	//Set the color and alpha value
	glColor4f(1.0f,1.0f,1.0f,0.5f);         // Full Brightness, 50% Alpha
	//Draw the cube
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

