/*
Masking:
1: Masking is basically blending. You simply use a special blend function together with a mask
2: The mask is then blending and textured to provide the effect
3: This tutorial also show how to move textures for some cool effects. This is simply done by
   changing the texture mapping coords over time, AND use wrapping textures so they repeat. 

*/

#include <windows.h>   // Standard Header For Most Programs
#include <gl/gl.h>     // The GL Header File
#include <gl/glut.h>   // The GL Utility Toolkit (Glut) Header
#include <GL/SOIL.h> //Image loading library header. Library also referenced by linker

#define NUM_TEXTURES 5

//General,misc,util
int loop = 0;
bool masking = false;
bool scene = false;

//Generel transformation variables
GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat zrot = 0.0f;
GLfloat zoom = -2.0f;

//Textures. 
GLuint texture[NUM_TEXTURES];
GLint filtering = 0;
GLfloat rolling = 0.0f;


//Lightning
GLboolean hasLightening = true;
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };              // Ambient Light Values
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };              // Diffuse Light Values
GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };             // Light Position

int LoadGLTextures()                                    // Load Bitmaps And Convert To Textures
{
    
		//Load the first texture
		texture[0] = SOIL_load_OGL_texture
        (
        "Data/logo.bmp",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );

		//Load the second texture
		texture[1] = SOIL_load_OGL_texture
        (
        "Data/mask1.bmp",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );

		//Load the third texture
		texture[2] = SOIL_load_OGL_texture
        (
        "Data/image1.bmp",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );

		//Load the fourth texture
		texture[3] = SOIL_load_OGL_texture
        (
        "Data/mask2.bmp",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
	
		//Load the fifth texture
		texture[4] = SOIL_load_OGL_texture
        (
        "Data/image2.bmp",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
 
    if(texture[0] == 0||texture[1] == 0||texture[2] == 0||texture[3] == 0||texture[4] == 0)
        return false;

    //Typical Texture Generation/filtering Using Data From The Bitmap
	for(loop = 0; loop < NUM_TEXTURES;loop++)
	{
		glBindTexture(GL_TEXTURE_2D, texture[loop]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
	}

	

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
	//glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	//glEnable ( GL_COLOR_MATERIAL );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Lightning GL_LIGHT1 setup (ambient,diffuse, position)
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);             // Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);             // Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);            // Position The Light

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
}

void display (void)   // Create The Display Function
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	
	glLoadIdentity();									// Reset The Current Modelview Matrix
	
	glTranslatef(0.0f,0.0f,zoom);						// Move Right 3 Units
	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);
	glRotatef(zrot,0.0f,0.0f,1.0f);

	


	glBindTexture(GL_TEXTURE_2D, texture[0]);               // Select Our Texture
	/*Note that we use the "rolling" variable here in the texture mapping coordinates. This means that the mapping
	  of the texture is variable and thus the texture will move according to the change in variable value! The 
	  coordinates are normalized (goes from 0.0->1.0 and center is 0.5,0.5). A mapping higher that 1.0 simply wraps
	  the texture around the surface edge and the texture repeats*/
	glBegin(GL_QUADS);									// Draw A Quad
		//Front
		glTexCoord2f(0.0f, -rolling+0.0f); glVertex3f(-1.1f, -1.1f,  0.0f);    // Bottom Left
		glTexCoord2f(3.0f, -rolling+0.0f); glVertex3f( 1.1f, -1.1f,  0.0f);    // Bottom Right
		glTexCoord2f(3.0f, -rolling+3.0f); glVertex3f( 1.1f,  1.1f,  0.0f);    // Top Right
		glTexCoord2f(0.0f, -rolling+3.0f); glVertex3f(-1.1f,  1.1f,  0.0f);    // Top Left
	glEnd();

	glEnable(GL_BLEND);                         // Enable Blending
	glDisable(GL_DEPTH_TEST);                       // Disable Depth Testing

	if(masking) //if masking is enabled
	{
		glBlendFunc(GL_DST_COLOR,GL_ZERO);              // Blend Screen Color With Zero (Black)
	}
	if (scene)                              // Are We Drawing The Second Scene?
	{
		glTranslatef(0.0f,0.0f,-1.0f);                  // Translate Into The Screen One Unit
		glRotatef(rolling*360,0.0f,0.0f,1.0f);             // Rotate On The Z Axis 360 Degrees
		
		if(masking)
		{
		glBindTexture(GL_TEXTURE_2D, texture[3]);       // Select The Second Mask Texture
			glBegin(GL_QUADS);                  // Start Drawing A Textured Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.1f, -1.1f,  0.0f);  // Bottom Left
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.1f, -1.1f,  0.0f);  // Bottom Right
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.1f,  1.1f,  0.0f);  // Top Right
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.1f,  1.1f,  0.0f);  // Top Left
		glEnd();    
		}

		glBlendFunc(GL_ONE, GL_ONE);                    // Copy Image 2 Color To The Screen
		glBindTexture(GL_TEXTURE_2D, texture[4]);           // Select The Second Image Texture
		glBegin(GL_QUADS);                      // Start Drawing A Textured Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.1f, -1.1f,  0.0f);  // Bottom Left
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.1f, -1.1f,  0.0f);  // Bottom Right
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.1f,  1.1f,  0.0f);  // Top Right
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.1f,  1.1f,  0.0f);  // Top Left
		glEnd();                            // Done Drawing The Quad
		
	}
	else //if !scene then draw the other scene
	{
		if (masking)                            // Is Masking On?
		{
			glBindTexture(GL_TEXTURE_2D, texture[1]);       // Select The First Mask Texture
			glBegin(GL_QUADS);                  // Start Drawing A Textured Quad
				glTexCoord2f(rolling+0.0f, 0.0f); glVertex3f(-1.1f, -1.1f,  0.0f); // Bottom Left
				glTexCoord2f(rolling+4.0f, 0.0f); glVertex3f( 1.1f, -1.1f,  0.0f); // Bottom Right
				glTexCoord2f(rolling+4.0f, 4.0f); glVertex3f( 1.1f,  1.1f,  0.0f); // Top Right
				glTexCoord2f(rolling+0.0f, 4.0f); glVertex3f(-1.1f,  1.1f,  0.0f); // Top Left
			glEnd();                        // Done Drawing The Quad
		}

		glBlendFunc(GL_ONE, GL_ONE);                    // Copy Image 1 Color To The Screen
		glBindTexture(GL_TEXTURE_2D, texture[2]);           // Select The First Image Texture
		glBegin(GL_QUADS);                      // Start Drawing A Textured Quad
			glTexCoord2f(rolling+0.0f, 0.0f); glVertex3f(-1.1f, -1.1f,  0.0f); // Bottom Left
			glTexCoord2f(rolling+4.0f, 0.0f); glVertex3f( 1.1f, -1.1f,  0.0f); // Bottom Right
			glTexCoord2f(rolling+4.0f, 4.0f); glVertex3f( 1.1f,  1.1f,  0.0f); // Top Right
			glTexCoord2f(rolling+0.0f, 4.0f); glVertex3f(-1.1f,  1.1f,  0.0f); // Top Left
		glEnd();                            // Done Drawing The Quad
	}

	glEnable(GL_DEPTH_TEST);                        // Enable Depth Testing
	glDisable(GL_BLEND);                            // Disable Blending
	
	rolling+=0.0001f;                               // Increase Our Texture Roll Variable
    if (rolling>4.0f)                               // Is Roll Greater Than One
    {
        rolling-=4.0f;                         // Subtract 1 From Roll
    }

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
	case 103: //g
		masking = !masking;
		break;
	case 104: //h
		scene = !scene;
		break;
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
	case 114: //r
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

