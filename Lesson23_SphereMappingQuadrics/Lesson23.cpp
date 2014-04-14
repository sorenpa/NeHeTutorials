/*
Quadrics:
1: define the quadric storage variable -> GLUquadricObj *quadratic;
2: Initialize/create memory space -> quadratic=glNewQuadratic();
3: Set up the quadratic -> gluQuadricNormals(quadratic, GLU_SMOOTH);   --Create Smooth Normals 
                        -> gluQuadricTexture(quadratic, GL_TRUE);      --Create Texture Coords
                        .
4: Draw the quadratic	-> gluCylinder(quadratic,1.0f,1.0f,3.0f,32,32);  --Draw a Cylinder
                        -> gluDisk(quadratic,0.5f,1.5f,32,32); --Draw a Disc
                        -> gluSphere(quadratic,0.5f,1.5f,32,32); --Draw a Sphere
                        .
*/

#include <windows.h>   // Standard Header For Most Programs
#include <gl/gl.h>     // The GL Header File
#include <gl/glut.h>   // The GL Utility Toolkit (Glut) Header

#include <GL/SOIL.h> //Image loading library header. Library also referenced by linker

GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat zrot = 0.0f;
GLfloat zoom = -5.0f;

//control variables
int part1;                      // Start Of Disc    ( NEW )
int part2;                      // End Of Disc      ( NEW )
int p1=0;                       // Increase 1       ( NEW )
int p2=1;                       // Increase 2       ( NEW )
GLfloat z=-5.0f;                    // Depth Into The Screen
int objectToDraw = 0;
 
GLUquadricObj *quadratic;               // Storage For Our Quadratic Objects ( NEW )

//Textures. 
GLuint texture[3];
GLint filtering = 0;

//Lightning
GLboolean hasLightening = false;
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
        "Data/Wall.bmp",
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

    //Lightning GL_LIGHT1 setup (ambient,diffuse, position)
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);             // Setup The Ambient Light
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);             // Setup The Diffuse Light
    glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);            // Position The Light

    glEnable(GL_LIGHTING);

    //Quadric init
    quadratic=gluNewQuadric();          // Create A Pointer To The Quadric Object ( NEW )
    gluQuadricNormals(quadratic, GLU_SMOOTH);   // Create Smooth Normals ( NEW )
    gluQuadricTexture(quadratic, GL_TRUE);      // Create Texture Coords ( NEW )
}

void glDrawCube(void)
{
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
}

void display (void)   // Create The Display Function
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
    
    glLoadIdentity();									// Reset The Current Modelview Matrix
    
    glTranslatef(0.0f,0.0f,zoom);						// Move Right 3 Units
    glRotatef(xrot,1.0f,0.0f,0.0f);
    glRotatef(yrot,0.0f,1.0f,0.0f);
    glRotatef(zrot,0.0f,0.0f,1.0f);

    
    switch(objectToDraw)
    {
        case 0:
            glBindTexture(GL_TEXTURE_2D, texture[filtering]);               // Select Our Texture
            glDrawCube();
            break;
        case 1:
            glTranslatef(0.0f,0.0f,-1.5f);          // Center The Cylinder
            gluCylinder(quadratic,1.0f,1.0f,3.0f,32,32);    // Draw Our Cylinder
            break;
        case 2:
            gluDisk(quadratic,0.5f,1.5f,32,32);     // Draw A Disc (CD Shape)
            break;
        case 3:
            gluSphere(quadratic,1.3f,32,32);        // Draw A Sphere
            break;
        case 4:
            glTranslatef(0.0f,0.0f,-1.5f);          // Center The Cone
            gluCylinder(quadratic,1.0f,0.0f,3.0f,32,32);    // A Cone With A Bottom Radius Of .5 And A Height Of 2
            break;
        case 5:                         // Drawing Object 6
            part1+=p1;                  // Increase Start Angle
            part2+=p2;                  // Increase Sweep Angle
 
            if(part1>359)                    // 360 Degrees
            {
                p1=0;                   // Stop Increasing Start Angle
                part1=0;                // Set Start Angle To Zero
                p2=1;                   // Start Increasing Sweep Angle
                part2=0;                // Start Sweep Angle At Zero
            }
            if(part2>359)                    // 360 Degrees
            {
                p1=1;                   // Start Increasing Start Angle
                p2=0;                   // Stop Increasing Sweep Angle
            }
            gluPartialDisk(quadratic,0.5f,1.5f,32,32,part1,part2-part1);    // A Disk Like The One Before
        break;                      // Done
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
        objectToDraw = (objectToDraw + 1)%5;
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

