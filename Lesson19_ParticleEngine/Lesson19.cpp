/*
Particle Engine:
This particle engine is made by displaying multiple particle strips moving in random directions,
with random lifespans and travel distance. The strips are texture mapped and smoothed to make it 
look nice

*/

#include <windows.h>   // Standard Header For Most Programs
#include <gl/gl.h>     // The GL Header File
#include <gl/glut.h>   // The GL Utility Toolkit (Glut) Header
#include <stdio.h>
#include <GL/SOIL.h> //Image loading library header. Library also referenced by linker

#define MAX_NUM_PARTICLES 1000

bool raindBowMode = true;
GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat zrot = 0.0f;
GLfloat zoom = -5.0f;

//Textures. 
GLuint texture[1];
GLint filtering = 0;

//Lightning
GLboolean hasLightening = false;
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };              // Ambient Light Values
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };              // Diffuse Light Values
GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };             // Light Position

//Particle specific
float   slowdown=2.0f;          // Slow Down Particles
float   xspeed;             // Base X Speed (To Allow Keyboard Direction Of Tail)
float   yspeed;             // Base Y Speed (To Allow Keyboard Direction Of Tail)

GLuint  loop;               // Misc Loop Variable
GLuint  col;                // Current Color Selection
GLuint  delay;              // Rainbow Effect Delay

typedef struct                      // Create A Structure For Particle
{
    bool    active;                 // Active (Yes/No)
    float   life;                   // Particle Life
    float   fade;                   // Fade Speed
	float   r;                  // Red Value
	float   g;                  // Green Value
	float   b;                  // Blue Value
	float   x;                  // X Position
	float   y;                  // Y Position
	float   z;                  // Z Position
	float   xi;                 // X Direction
	float   yi;                 // Y Direction
	float   zi;                 // Z Direction
	float   xg;                 // X Gravity
	float   yg;                 // Y Gravity
	float   zg;                 // Z Gravity
}
particles;                      // Particles Structure

particles particle[MAX_NUM_PARTICLES];          // Particle Array (Room For Particle Info)

static GLfloat colors[12][3]=               // Rainbow Of Colors
{
    {1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
    {0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
    {0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
};


int LoadGLTextures()                                    // Load Bitmaps And Convert To Textures
{
    /* load an image file directly as a new OpenGL texture */
	for(int i = 0; i < 3;i++)
	{
		texture[i] = SOIL_load_OGL_texture
        (
        "Data/Particle.bmp",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
	}
 
    if(texture[0] == 0)
        return false;

 
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    return true;                                        // Return Success
	
}

void initParticles(void)
{
	for(loop=0; loop<MAX_NUM_PARTICLES;loop++)
	{
		particle[loop].active=true;                 // Make All The Particles Active
	    particle[loop].life=1.0f;                   // Give All The Particles Full Life
		particle[loop].fade=float(rand()%100)/1000.0f+0.003f;       // Random Fade Speed
		particle[loop].r=colors[loop*(12/MAX_NUM_PARTICLES)][0];        // Select Red Rainbow Color
		particle[loop].g=colors[loop*(12/MAX_NUM_PARTICLES)][1];        // Select Red Rainbow Color
		particle[loop].b=colors[loop*(12/MAX_NUM_PARTICLES)][2];        // Select Red Rainbow Color
		particle[loop].xi=float((rand()%50)-26.0f)*10.0f;       // Random Speed On X Axis
		particle[loop].yi=float((rand()%50)-25.0f)*10.0f;       // Random Speed On Y Axis
		particle[loop].zi=float((rand()%50)-25.0f)*10.0f;       // Random Speed On Z Axis
		particle[loop].xg=0.0f;                     // Set Horizontal Pull To Zero
        particle[loop].yg=-0.8f;                    // Set Vertical Pull Downward
        particle[loop].zg=0.0f;                     // Set Pull On Z Axis To Zero

	}
}

void init ( GLvoid )     // Create Some Everyday Functions
{
	//Load the textures with SOIL
	LoadGLTextures();

	//Initialize particles
	initParticles();	

	glEnable(GL_TEXTURE_2D);							// Enable Texturemapping!
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glDisable(GL_DEPTH_TEST);                       // Disables Depth Testing
	glEnable(GL_BLEND);                         // Enable Blending
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);                   // Type Of Blending To Perform
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glEnable ( GL_COLOR_MATERIAL );						// Enable texture coloring
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);           // Really Nice Perspective Calculations
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);                 // Really Nice Point Smoothing
	
	//Lightning GL_LIGHT1 setup (ambient,diffuse, position)
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);             // Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);             // Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);            // Position The Light

	glEnable(GL_LIGHTING);
}

void display (void)   // Create The Display Function
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear Screen And Depth Buffer
    glLoadIdentity();                           // Reset The ModelView Matrix



	
	glTranslatef(0.0f,0.0f,zoom);						// Move Right 3 Units
	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);
	glRotatef(zrot,0.0f,0.0f,1.0f);

	for (loop=0;loop<MAX_NUM_PARTICLES;loop++)                   // Loop Through All The Particles
	{

		if (particle[loop].active)                  // If The Particle Is Active
		{

			float x=particle[loop].x;               // Grab Our Particle X Position
			float y=particle[loop].y;               // Grab Our Particle Y Position
			float z=particle[loop].z;              // Particle Z Pos + Zoom

			// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
			glColor4f(particle[loop].r,particle[loop].g,particle[loop].b,particle[loop].life);
			glBindTexture(GL_TEXTURE_2D,texture[0]);                // Select Our Texture
			glBegin(GL_TRIANGLE_STRIP);             // Build Quad From A Triangle Strip	
				glTexCoord2d(1,1); glVertex3f(x+0.5f,y+0.5f,z); // Top Right
				glTexCoord2d(0,1); glVertex3f(x-0.5f,y+0.5f,z); // Top Left
				glTexCoord2d(1,0); glVertex3f(x+0.5f,y-0.5f,z); // Bottom Right
				glTexCoord2d(0,0); glVertex3f(x-0.5f,y-0.5f,z); // Bottom Left
			glEnd();                        // Done Building Triangle Strip

			particle[loop].x+=particle[loop].xi/(slowdown*1000);    // Move On The X Axis By X Speed
			particle[loop].y+=particle[loop].yi/(slowdown*1000);    // Move On The Y Axis By Y Speed
			particle[loop].z+=particle[loop].zi/(slowdown*1000);    // Move On The Z Axis By Z Speed

			particle[loop].xi+=particle[loop].xg;           // Take Pull On X Axis Into Account
			particle[loop].yi+=particle[loop].yg;           // Take Pull On Y Axis Into Account
			particle[loop].zi+=particle[loop].zg;           // Take Pull On Z Axis Into Account

			particle[loop].life-=particle[loop].fade;       // Reduce Particles Life By 'Fade'

			if (particle[loop].life<0.0f)                    // If Particle Is Burned Out
			{
				particle[loop].life=1.0f;               // Give It New Life
				particle[loop].fade=float(rand()%100)/1000.0f+0.003f;   // Random Fade Value
				particle[loop].x=0.0f;                  // Center On X Axis
				particle[loop].y=0.0f;                  // Center On Y Axis
				particle[loop].z=0.0f;                  // Center On Z Axis
				particle[loop].xi=xspeed+float((rand()%60)-32.0f);  // X Axis Speed And Direction
				particle[loop].yi=yspeed+float((rand()%60)-30.0f);  // Y Axis Speed And Direction
				particle[loop].zi=float((rand()%60)-30.0f);     // Z Axis Speed And Direction
				particle[loop].r=colors[col][0];            // Select Red From Color Table
				particle[loop].g=colors[col][1];            // Select Green From Color Table
				particle[loop].b=colors[col][2];            // Select Blue From Color Table
			}
		}
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
	switch ( key ) 
	{
	
    case 27:        // When Escape Is Pressed...
      exit ( 0 );   // Exit The Program
      break;        // Ready For Next Case
	case 103: //g
		particle[loop].x=0.0f;                  // Center On X Axis
        particle[loop].y=0.0f;                  // Center On Y Axis
        particle[loop].z=0.0f;                  // Center On Z Axis
        particle[loop].xi=float((rand()%50)-26.0f)*10.0f;   // Random Speed On X Axis
        particle[loop].yi=float((rand()%50)-25.0f)*10.0f;   // Random Speed On Y Axis
        particle[loop].zi=float((rand()%50)-25.0f)*10.0f;   // Random Speed On Z Axis
		break;
	case 104: //h
		delay=0;						// Reset The Rainbow Color Cycling Delay
		col += (col+1)%3;							// Change The Particle Color
		break;
	case 106: //h
		slowdown += 0.1f;
		break;
	case 107: //h
		slowdown -= 0.1f;
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

