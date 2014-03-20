/*
Waving textures:
	1: glPolyGonMode - These simply specify that we want back facing polygons to 
	   be filled completely and that we want front facing polygons to be outlined 
	   only. Mostly personal preference at this point. Has to do with the orientation 
	   of the polygon or the direction of the vertices.
	2: To create the flag texture we simply build the "flag" as a 2D plane with multiple 
	   vertices alligned as a grid. We then apply a ´sine wave function to create "ripples".

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
GLuint texture[3];
GLint filtering = 0;

//Lightning
GLboolean hasLightening = true;
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };              // Ambient Light Values
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };              // Diffuse Light Values
GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };             // Light Position

//Flag data
float points[ 45 ][ 45 ][3];                    // The Array For The Points On The Grid Of Our "Wave"
int wiggle_count = 0;                       // Counter Used To Control How Fast Flag Waves
GLfloat hold;     

int LoadGLTextures()                                    // Load Bitmaps And Convert To Textures
{
    /* load an image file directly as a new OpenGL texture */
	for(int i = 0; i < 3;i++)
	{
		texture[i] = SOIL_load_OGL_texture
        (
        "Data/Tim.bmp",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
	}

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

	//Set the polygon mode to draw both front and back faces
	glPolygonMode( GL_BACK, GL_FILL );          // Back Face Is Filled In
	glPolygonMode( GL_FRONT, GL_LINE );         // Front Face Is Drawn With Lines


	//Lightning GL_LIGHT1 setup (ambient,diffuse, position)
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);             // Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);             // Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);            // Position The Light
	
	glEnable(GL_LIGHT1);

	glEnable(GL_LIGHTING);

	//Init the flag array
	// Loop Through The X Plane
	for(int x=0; x<45; x++)
	{
		// Loop Through The Y Plane
		for(int y=0; y<45; y++)
		{
			// Apply The Wave To Our Mesh
	        points[x][y][0]=float((x/5.0f)-4.5f);
		    points[x][y][1]=float((y/5.0f)-4.5f);
			points[x][y][2]=float(sin((((x/5.0f)*40.0f)/360.0f)*3.141592654*2.0f));
	    }
	}

}

void display (void)   // Create The Display Function
{
	int x,y;
	GLfloat float_x,float_y,float_xb,float_yb;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer

  	glLoadIdentity();	// Reset The Current Modelview Matrix
	
	//translate world relating to camera position.
	glRotatef(360.0f-yRot,0.0f,1.0f,0.0f);
	glTranslatef(-xPos,yPos,-zPos);
	glTranslatef(0.0f,0.0f,-12.0f);
	glRotatef(-45.0f,1.0f,0.0f,0.0f);
	//Translate the original object position
	

	glBindTexture(GL_TEXTURE_2D, texture[0]);       // Select Our Texture

	glBegin(GL_QUADS);                  // Start Drawing Our Quads
	for( x = 0; x < 44; x++ )                // Loop Through The X Plane (44 Points)
	{
		for( y = 0; y < 44; y++ )            // Loop Through The Y Plane (44 Points)
		{

			float_x = float(x)/44.0f;       // Create A Floating Point X Value
			float_y = float(y)/44.0f;       // Create A Floating Point Y Value
			float_xb = float(x+1)/44.0f;        // Create A Floating Point Y Value+0.0227f
			float_yb = float(y+1)/44.0f;        // Create A Floating Point Y Value+0.0227f

			glTexCoord2f( float_x, float_y);    // First Texture Coordinate (Bottom Left)
			glVertex3f( points[x][y][0], points[x][y][1], points[x][y][2] );
         
			glTexCoord2f( float_x, float_yb );  // Second Texture Coordinate (Top Left)
			glVertex3f( points[x][y+1][0], points[x][y+1][1], points[x][y+1][2] );
         
			glTexCoord2f( float_xb, float_yb ); // Third Texture Coordinate (Top Right)
			glVertex3f( points[x+1][y+1][0], points[x+1][y+1][1], points[x+1][y+1][2] );
         
			glTexCoord2f( float_xb, float_y );  // Fourth Texture Coordinate (Bottom Right)
			glVertex3f( points[x+1][y][0], points[x+1][y][1], points[x+1][y][2] );
		}
	}
	glEnd();                        // Done Drawing Our Quads

	if( wiggle_count == 2 )                 // Used To Slow Down The Wave (Every 2nd Frame Only)
	{

		for( y = 0; y < 45; y++ )            // Loop Through The Y Plane
		{
			hold=points[0][y][2];           // Store Current Value One Left Side Of Wave
			for( x = 0; x < 44; x++)     // Loop Through The X Plane
			{
				// Current Wave Value Equals Value To The Right
				points[x][y][2] = points[x+1][y][2];
			}
			points[44][y][2]=hold;          // Last Value Becomes The Far Left Stored Value
		}
		wiggle_count = 0;               // Set Counter Back To Zero
	}
	
	wiggle_count++;                     // Increase The Counter

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

