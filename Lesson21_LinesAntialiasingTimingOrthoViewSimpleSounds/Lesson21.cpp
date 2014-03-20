/*
This is a simple game made from lines, ortho view and a timer. Key learning points
1: glBegin(GL_LINES); -- Draws lines
2: glLineWidth(2.0f); -- Sets width of lines
3: Ortho view is set in the reshap function by calling: glOrtho(0.0f,w,h,0.0f,-1.0f,1.0f);
4: A timer is set up in the InitTimer, and the time is fetched by GetTime(). Both custom made functions in this tutorial.
5: The glutMainLoop and glutIdleFunc is used correctly for the first time in this tutorial:
	5a: The glutMainLoop() starts the "main loop" which is an infinite loop that calls the "idle function".
	5b: the gluIdleFunc([*ToFunction]) sets the "idle function".
	5c: In this tutorial the idle function does the following:
		-> call display(); that draws the scene
		-> call glutSwapBuffers(); that swaps the buffer (double buffering)
		-> calculates gamelogic and time between frames.
		-> returns (and is then called again by the "main loop"

*/

#include <windows.h>   // Standard Header For Most Programs
#include <gl/gl.h>     // The GL Header File
#include <gl/glut.h>   // The GL Utility Toolkit (Glut) Header
#include <stdio.h>
#include <GL/SOIL.h> //Image loading library header. Library also referenced by linker

GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat zrot = 0.0f;
GLfloat zoom = -5.0f;

//Textures. 
GLuint texture[2];

//Game variables
bool        vline[11][10];                          // Keeps Track Of Verticle Lines
bool        hline[10][11];                          // Keeps Track Of Horizontal Lines
bool        filled;                             // Done Filling In The Grid?
bool        gameover;                           // Is The Game Over?
bool        anti=TRUE;                          // Antialiasing?
bool        active=TRUE;                            // Window Active Flag Set To TRUE By Default
bool        fullscreen=TRUE;                        // Fullscreen Flag Set To Fullscreen Mode By 
int     loop1;                              // Generic Loop1
int     loop2;                              // Generic Loop2
int     delay;                              // Enemy Delay
int     adjust=3;                           // Speed Adjustment For Really Slow Video Cards
int     lives=2;                            // Player Lives
int     level=5;                            // Internal Game Level
int     level2=level;                           // Displayed Game Level
int     stage=1;                            // Game Stage

struct      object                              // Create A Structure For Our Player
{
	int fx, fy;                             // Fine Movement Position
	int x, y;                               // Current Player Position
	float   spin;                               // Spin Direction
};

struct  object  player;                             // Player Information
struct  object  enemy[9];                           // Enemy Information
struct  object  hourglass;                          // Hourglass Information

struct                                      // Create A Structure For The Timer Information
{
	__int64       frequency;                          // Timer Frequency
	float         resolution;                         // Timer Resolution
	unsigned long mm_timer_start;                         // Multimedia Timer Start Value
	unsigned long mm_timer_elapsed;                       // Multimedia Timer Elapsed Time
	bool      performance_timer;                      // Using The Performance Timer?
	__int64       performance_timer_start;                    // Performance Timer Start Value
	__int64       performance_timer_elapsed;                  // Performance Timer Elapsed Time
} timer;  

int     steps[6]={ 1, 2, 4, 5, 10, 20 };                // Stepping Values For Slow Video Adjustment

GLuint      base;                               // Base Display List For The Font

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);               // Declaration For WndProc

void TimerInit(void)                                // Initialize Our Timer (Get It Ready)
{
	memset(&timer, 0, sizeof(timer));                   // Clear Our Timer Structure

	// Check To See If A Performance Counter Is Available
	// If One Is Available The Timer Frequency Will Be Updated
	if (!QueryPerformanceFrequency((LARGE_INTEGER *) &timer.frequency))
	{
		// No Performace Counter Available
		timer.performance_timer = FALSE;                // Set Performance Timer To FALSE
		timer.mm_timer_start    = timeGetTime();            // Use timeGetTime() To Get Current Time
		timer.resolution    = 1.0f/1000.0f;             // Set Our Timer Resolution To .001f
		timer.frequency     = 1000;                 // Set Our Timer Frequency To 1000
		timer.mm_timer_elapsed  = timer.mm_timer_start;         // Set The Elapsed Time To The Current Time
	}

	else
	{
		// Performance Counter Is Available, Use It Instead Of The Multimedia Timer
		// Get The Current Time And Store It In performance_timer_start
		QueryPerformanceCounter((LARGE_INTEGER *) &timer.performance_timer_start);
		timer.performance_timer     = TRUE;             // Set Performance Timer To TRUE
		// Calculate The Timer Resolution Using The Timer Frequency
		timer.resolution        = (float) (((double)1.0f)/((double)timer.frequency));
		// Set The Elapsed Time To The Current Time
		timer.performance_timer_elapsed = timer.performance_timer_start;
	}
}

float TimerGetTime()                                // Get Time In Milliseconds
{
	__int64 time;                               // time Will Hold A 64 Bit Integer

	if (timer.performance_timer)                        // Are We Using The Performance Timer?
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &time);       // Grab The Current Performance Time
		// Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
		return ( (float) ( time - timer.performance_timer_start) * timer.resolution)*1000.0f;
	}
	else
	{
		// Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
		return( (float) ( timeGetTime() - timer.mm_timer_start) * timer.resolution)*1000.0f;
	}
}

void ResetObjects(void)                             // Reset Player And Enemies
{
	player.x=0;                             // Reset Player X Position To Far Left Of The Screen
	player.y=0;                             // Reset Player Y Position To The Top Of The Screen
	player.fx=0;                                // Set Fine X Position To Match
	player.fy=0;  
	for (loop1=0; loop1<(stage*level); loop1++)              // Loop Through All The Enemies
	{
		enemy[loop1].x=5+rand()%6;                  // Select A Random X Position
		enemy[loop1].y=rand()%11;                   // Select A Random Y Position
		enemy[loop1].fx=enemy[loop1].x*60;              // Set Fine X To Match
		enemy[loop1].fy=enemy[loop1].y*40;              // Set Fine Y To Match
	}
}

GLvoid BuildFont(GLvoid)                            // Build Our Font Display List
{
	base=glGenLists(256);                           // Creating 256 Display Lists
	glBindTexture(GL_TEXTURE_2D, texture[0]);               // Select Our Font Texture
	for (loop1=0; loop1<256; loop1++)                    // Loop Through All 256 Lists
	{
		float cx=float(loop1%16)/16.0f;                 // X Position Of Current Character
		float cy=float(loop1/16)/16.0f;                 // Y Position Of Current Character

		glNewList(base+loop1,GL_COMPILE);               // Start Building A List
		glBegin(GL_QUADS);                  // Use A Quad For Each Character
		glTexCoord2f(cx,1.0f-cy-0.0625f);       // Texture Coord (Bottom Left)
		glVertex2d(0,16);               // Vertex Coord (Bottom Left)
		glTexCoord2f(cx+0.0625f,1.0f-cy-0.0625f);   // Texture Coord (Bottom Right)
		glVertex2i(16,16);              // Vertex Coord (Bottom Right)
		glTexCoord2f(cx+0.0625f,1.0f-cy);       // Texture Coord (Top Right)
		glVertex2i(16,0);               // Vertex Coord (Top Right)
		glTexCoord2f(cx,1.0f-cy);           // Texture Coord (Top Left)
		glVertex2i(0,0);                // Vertex Coord (Top Left)
		glEnd();                        // Done Building Our Quad (Character)
		glTranslated(15,0,0);                   // Move To The Right Of The Character
		glEndList();                            // Done Building The Display List
	}                                   // Loop Until All 256 Are Built
}

GLvoid KillFont(GLvoid)                             // Delete The Font From Memory
{
	glDeleteLists(base,256);                        // Delete All 256 Display Lists
}

GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...)         // Where The Printing Happens
{
	char        text[256];                      // Holds Our String
	va_list     ap;                         // Pointer To List Of Arguments

	if (fmt == NULL)                            // If There's No Text
		return;                             // Do Nothing

	va_start(ap, fmt);                          // Parses The String For Variables
	vsprintf(text, fmt, ap);                        // And Converts Symbols To Actual Numbers
	va_end(ap);                             // Results Are Stored In Text

	if (set>1)                               // Did User Choose An Invalid Character Set?
	{
		set=1;                              // If So, Select Set 1 (Italic)
	}
	glEnable(GL_TEXTURE_2D);                        // Enable Texture Mapping
	glLoadIdentity();                           // Reset The Modelview Matrix
	glTranslated(x,y,0);                            // Position The Text (0,0 - Bottom Left)
	glListBase(base-32+(128*set));                      // Choose The Font Set (0 or 1)

	if (set==0)                             // If Set 0 Is Being Used Enlarge Font
	{
		glScalef(1.5f,2.0f,1.0f);                   // Enlarge Font Width And Height
	}

	glCallLists(strlen(text),GL_UNSIGNED_BYTE, text);           // Write The Text To The Screen
	glDisable(GL_TEXTURE_2D);                       // Disable Texture Mapping
}

int LoadGLTextures()                                    // Load Bitmaps And Convert To Textures
{
	/* load an image file directly as a new OpenGL texture */
	texture[0] = SOIL_load_OGL_texture
		(
		"Data/Font.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
		);

	texture[1] = SOIL_load_OGL_texture
		(
		"Data/Image.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
		);


	if(texture[0] == 0||texture[1] == 0)
		return false;

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	return true;                                        // Return Success

}

void init ( GLvoid )     // Create Some Everyday Functions
{
	if (!LoadGLTextures())                          // Jump To Texture Loading Routine
	{
		return;                           // If Texture Didn't Load Return
	}

	TimerInit();								// Timer Initialization

	BuildFont();                                // Build The Font

	glShadeModel(GL_SMOOTH);                        // Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                   // Black Background
	glClearDepth(1.0f);                         // Depth Buffer Setup
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);                 // Set Line Antialiasing
	glEnable(GL_BLEND);                         // Enable Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);          // Type Of Blending To Use

}

void display (void)   // Create The Display Function
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear Screen And Depth Buffer
	glBindTexture(GL_TEXTURE_2D, texture[0]);               // Select Our Font Texture
	glColor3f(1.0f,0.5f,1.0f);                      // Set Color To Purple
	glPrint(207,24,0,"GRID CRAZY");                     // Write GRID CRAZY On The Screen
	glColor3f(1.0f,1.0f,0.0f);                      // Set Color To Yellow
	glPrint(20,20,1,"Level:%2i",level2);                    // Write Actual Level Stats
	glPrint(20,40,1,"Stage:%2i",stage);                 // Write Stage Stats
	glPrint(20,480,1,"X:%2i",player.x);                 // Write Stage Stats
	glPrint(100,480,1,"Y:%2i",player.y);                 // Write Stage Stats
	

	if (gameover)                               // Is The Game Over?
	{
		glColor3ub(rand()%255,rand()%255,rand()%255);           // Pick A Random Color
		glPrint(472,20,1,"GAME OVER");                  // Write GAME OVER To The Screen
		glPrint(456,40,1,"PRESS SPACE");                // Write PRESS SPACE To The Screen
	}

	for (loop1=0; loop1<lives-1; loop1++)                    // Loop Through Lives Minus Current Life
	{
		glLoadIdentity();                       // Reset The View
		glTranslatef(490+(loop1*40.0f),40.0f,0.0f);         // Move To The Right Of Our Title Text
		glRotatef(-player.spin,0.0f,0.0f,1.0f);             // Rotate Counter Clockwise
		glColor3f(0.0f,1.0f,0.0f);                  // Set Player Color To Light Green
		glBegin(GL_LINES);                      // Start Drawing Our Player Using Lines
		glVertex2d(-5,-5);                  // Top Left Of Player
		glVertex2d( 5, 5);                  // Bottom Right Of Player
		glVertex2d( 5,-5);                  // Top Right Of Player
		glVertex2d(-5, 5);                  // Bottom Left Of Player
		glEnd();                            // Done Drawing The Player
		glRotatef(-player.spin*0.5f,0.0f,0.0f,1.0f);            // Rotate Counter Clockwise
		glColor3f(0.0f,0.75f,0.0f);                 // Set Player Color To Dark Green
		glBegin(GL_LINES);                      // Start Drawing Our Player Using Lines
		glVertex2d(-7, 0);                  // Left Center Of Player
		glVertex2d( 7, 0);                  // Right Center Of Player
		glVertex2d( 0,-7);                  // Top Center Of Player
		glVertex2d( 0, 7);                  // Bottom Center Of Player
		glEnd();                            // Done Drawing The Player
	}

	filled=TRUE;                                // Set Filled To True Before Testing
	glLineWidth(2.0f);                          // Set Line Width For Cells To 2.0f
	glDisable(GL_LINE_SMOOTH);                      // Disable Antialiasing
	glLoadIdentity();                           // Reset The Current Modelview Matrix
	for (loop1=0; loop1<11; loop1++)                 // Loop From Left To Right
	{
		for (loop2=0; loop2<11; loop2++)             // Loop From Top To Bottom
		{
			glColor3f(0.0f,0.5f,1.0f);              // Set Line Color To Blue
			if (hline[loop1][loop2])                // Has The Horizontal Line Been Traced
			{
				glColor3f(1.0f,1.0f,1.0f);          // If So, Set Line Color To White
			}
			if (loop1<10)                        // Dont Draw To Far Right
			{
				if (!hline[loop1][loop2])           // If A Horizontal Line Isn't Filled
				{
					filled=FALSE;               // filled Becomes False
				}
				glBegin(GL_LINES);              // Start Drawing Horizontal Cell Borders
				glVertex2d(20+(loop1*60),70+(loop2*40));    // Left Side Of Horizontal Line
				glVertex2d(80+(loop1*60),70+(loop2*40));    // Right Side Of Horizontal Line
				glEnd();                    // Done Drawing Horizontal Cell Borders
			}
			glColor3f(0.0f,0.5f,1.0f);              // Set Line Color To Blue
			if (vline[loop1][loop2])                // Has The Horizontal Line Been Traced
			{
				glColor3f(1.0f,1.0f,1.0f);          // If So, Set Line Color To White
			}
			if (loop2<10)                        // Dont Draw To Far Down
			{
				if (!vline[loop1][loop2])           // If A Verticle Line Isn't Filled
				{
					filled=FALSE;               // filled Becomes False
				}
				glBegin(GL_LINES);              // Start Drawing Verticle Cell Borders
				glVertex2d(20+(loop1*60),70+(loop2*40));    // Left Side Of Horizontal Line
				glVertex2d(20+(loop1*60),110+(loop2*40));   // Right Side Of Horizontal Line
				glEnd();                    // Done Drawing Verticle Cell Borders
			}

			glEnable(GL_TEXTURE_2D);                // Enable Texture Mapping
			glColor3f(1.0f,1.0f,1.0f);              // Bright White Color
			glBindTexture(GL_TEXTURE_2D, texture[1]);       // Select The Tile Image
			if ((loop1<10) && (loop2<10))             // If In Bounds, Fill In Traced Boxes
			{
				// Are All Sides Of The Box Traced?
				if (hline[loop1][loop2] && hline[loop1][loop2+1] && vline[loop1][loop2] && vline[loop1+1][loop2])
				{
					glBegin(GL_QUADS);          // Draw A Textured Quad
					glTexCoord2f(float(loop1/10.0f)+0.1f,1.0f-(float(loop2/10.0f)));
					glVertex2d(20+(loop1*60)+59,(70+loop2*40+1));   // Top Right
					glTexCoord2f(float(loop1/10.0f),1.0f-(float(loop2/10.0f)));
					glVertex2d(20+(loop1*60)+1,(70+loop2*40+1));    // Top Left
					glTexCoord2f(float(loop1/10.0f),1.0f-(float(loop2/10.0f)+0.1f));
					glVertex2d(20+(loop1*60)+1,(70+loop2*40)+39);   // Bottom Left
					glTexCoord2f(float(loop1/10.0f)+0.1f,1.0f-(float(loop2/10.0f)+0.1f));
					glVertex2d(20+(loop1*60)+59,(70+loop2*40)+39);  // Bottom Right
					glEnd();                // Done Texturing The Box
				}
			}
			glDisable(GL_TEXTURE_2D);               // Disable Texture Mapping
		}
	}
	glLineWidth(1.0f);                          // Set The Line Width To 1.0f
	if (anti)                               // Is Anti TRUE?
	{
		glEnable(GL_LINE_SMOOTH);                   // If So, Enable Antialiasing
	}

	if (hourglass.fx==1)                            // If fx=1 Draw The Hourglass
	{
		glLoadIdentity();                       // Reset The Modelview Matrix
		glTranslatef(20.0f+(hourglass.x*60),70.0f+(hourglass.y*40),0.0f);   // Move To The Fine Hourglass Position
		glRotatef(hourglass.spin,0.0f,0.0f,1.0f);           // Rotate Clockwise
		glColor3ub(rand()%255,rand()%255,rand()%255);           // Set Hourglass Color To Random Color
		glBegin(GL_LINES);                      // Start Drawing Our Hourglass Using Lines
		glVertex2d(-5,-5);                  // Top Left Of Hourglass
		glVertex2d( 5, 5);                  // Bottom Right Of Hourglass
		glVertex2d( 5,-5);                  // Top Right Of Hourglass
		glVertex2d(-5, 5);                  // Bottom Left Of Hourglass
		glVertex2d(-5, 5);                  // Bottom Left Of Hourglass
		glVertex2d( 5, 5);                  // Bottom Right Of Hourglass
		glVertex2d(-5,-5);                  // Top Left Of Hourglass
		glVertex2d( 5,-5);                  // Top Right Of Hourglass
		glEnd();                            // Done Drawing The Hourglass
	}
	glLoadIdentity();                           // Reset The Modelview Matrix
	glTranslatef(player.fx+20.0f,player.fy+70.0f,0.0f);         // Move To The Fine Player Position
	glRotatef(player.spin,0.0f,0.0f,1.0f);                  // Rotate Clockwise
	glColor3f(0.0f,1.0f,0.0f);                      // Set Player Color To Light Green
	glBegin(GL_LINES);                          // Start Drawing Our Player Using Lines
	glVertex2d(-5,-5);                      // Top Left Of Player
	glVertex2d( 5, 5);                      // Bottom Right Of Player
	glVertex2d( 5,-5);                      // Top Right Of Player
	glVertex2d(-5, 5);                      // Bottom Left Of Player
	glEnd();                                // Done Drawing The Player

	glRotatef(player.spin*0.5f,0.0f,0.0f,1.0f);             // Rotate Clockwise
	glColor3f(0.0f,0.75f,0.0f);                     // Set Player Color To Dark Green
	glBegin(GL_LINES);                          // Start Drawing Our Player Using Lines
	glVertex2d(-7, 0);                      // Left Center Of Player
	glVertex2d( 7, 0);                      // Right Center Of Player
	glVertex2d( 0,-7);                      // Top Center Of Player
	glVertex2d( 0, 7);                      // Bottom Center Of Player
	glEnd();                                // Done Drawing The Player


	for (loop1=0; loop1<(stage*level); loop1++)              // Loop To Draw Enemies
	{
		glLoadIdentity();                       // Reset The Modelview Matrix
		glTranslatef(enemy[loop1].fx+20.0f,enemy[loop1].fy+70.0f,0.0f);
		glColor3f(1.0f,0.5f,0.5f);                  // Make Enemy Body Pink
		glBegin(GL_LINES);                      // Start Drawing Enemy
		glVertex2d( 0,-7);                  // Top Point Of Body
		glVertex2d(-7, 0);                  // Left Point Of Body
		glVertex2d(-7, 0);                  // Left Point Of Body
		glVertex2d( 0, 7);                  // Bottom Point Of Body
		glVertex2d( 0, 7);                  // Bottom Point Of Body
		glVertex2d( 7, 0);                  // Right Point Of Body
		glVertex2d( 7, 0);                  // Right Point Of Body
		glVertex2d( 0,-7);                  // Top Point Of Body
		glEnd();                            // Done Drawing Enemy Body
		glRotatef(enemy[loop1].spin,0.0f,0.0f,1.0f);            // Rotate The Enemy Blade
		glColor3f(1.0f,0.0f,0.0f);                  // Make Enemy Blade Red
		glBegin(GL_LINES);                      // Start Drawing Enemy Blade
		glVertex2d(-7,-7);                  // Top Left Of Enemy
		glVertex2d( 7, 7);                  // Bottom Right Of Enemy
		glVertex2d(-7, 7);                  // Bottom Left Of Enemy
		glVertex2d( 7,-7);                  // Top Right Of Enemy
		glEnd();                            // Done Drawing Enemy Blade
	}

	
}

void reshape ( int w, int h )   // Create The Reshape Function (the viewport)
{
	glViewport     ( 0, 0, w, h );
	glMatrixMode   ( GL_PROJECTION );  // Select The Projection Matrix
	glLoadIdentity ( );                // Reset The Projection Matrix
	if ( h==0 )  // Calculate The Aspect Ratio Of The Window
	{
		h = 1;
	}

	glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
	glLoadIdentity();                           // Reset The Projection Matrix

	glOrtho(0.0f,w,h,0.0f,-1.0f,1.0f);             // Create Ortho 640x480 View (0,0 At Top Left)

	glMatrixMode(GL_MODELVIEW);                     // Select The Modelview Matrix
	glLoadIdentity();    

}

void IdleFunc(void)
{
	float start=TimerGetTime();             // Grab Timer Value Before We Draw

	// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
	if (active)  // Active?  Was There A Quit Received?
	{
		display();
		glutSwapBuffers();
	}
	else                            // Not Time To Quit, Update Screen
	{
		exit(0);
	}

	while(TimerGetTime()<start+float(steps[adjust]*2.0f)) {}// Waste Cycles On Fast Systems 

	if (!gameover && active)                // If Game Isn't Over And Programs Active Move Objects
	{
		for (loop1=0; loop1<(stage*level); loop1++)  // Loop Through The Different Stages
		{
			if ((enemy[loop1].x<player.x) && (enemy[loop1].fy==enemy[loop1].y*40))
			{
				enemy[loop1].x++;       // Move The Enemy Right
			}

			if ((enemy[loop1].x>player.x) && (enemy[loop1].fy==enemy[loop1].y*40))
			{
				enemy[loop1].x--;       // Move The Enemy Left
			}

			if ((enemy[loop1].y<player.y) && (enemy[loop1].fx==enemy[loop1].x*60))
			{
				enemy[loop1].y++;       // Move The Enemy Down
			}

			if ((enemy[loop1].y>player.y) && (enemy[loop1].fx==enemy[loop1].x*60))
			{
				enemy[loop1].y--;       // Move The Enemy Up
			}

			if (delay>(3-level) && (hourglass.fx!=2))        // If Our Delay Is Done And Player Doesn't Have Hourglass
			{
				delay=0;                    // Reset The Delay Counter Back To Zero
				for (loop2=0; loop2<(stage*level); loop2++)  // Loop Through All The Enemies
				{
					if (enemy[loop2].fx<enemy[loop2].x*60)   // Is Fine Position On X Axis Lower Than Intended Position?
					{
						enemy[loop2].fx+=steps[adjust]; // If So, Increase Fine Position On X Axis
						enemy[loop2].spin+=steps[adjust];   // Spin Enemy Clockwise
					}
					if (enemy[loop2].fx>enemy[loop2].x*60)   // Is Fine Position On X Axis Higher Than Intended Position?
					{
						enemy[loop2].fx-=steps[adjust]; // If So, Decrease Fine Position On X Axis
						enemy[loop2].spin-=steps[adjust];   // Spin Enemy Counter Clockwise
					}
					if (enemy[loop2].fy<enemy[loop2].y*40)   // Is Fine Position On Y Axis Lower Than Intended Position?
					{
						enemy[loop2].fy+=steps[adjust]; // If So, Increase Fine Position On Y Axis
						enemy[loop2].spin+=steps[adjust];   // Spin Enemy Clockwise
					}
					if (enemy[loop2].fy>enemy[loop2].y*40)   // Is Fine Position On Y Axis Higher Than Intended Position?
					{
						enemy[loop2].fy-=steps[adjust]; // If So, Decrease Fine Position On Y Axis
						enemy[loop2].spin-=steps[adjust];   // Spin Enemy Counter Clockwise
					}
				}
			}
			// Are Any Of The Enemies On Top Of The Player?
			if ((enemy[loop1].fx==player.fx) && (enemy[loop1].fy==player.fy))
			{
				lives--;            // If So, Player Loses A Life

				if (lives==0)           // Are We Out Of Lives?
				{
					gameover=TRUE;      // If So, gameover Becomes TRUE
				}

				ResetObjects();         // Reset Player / Enemy Positions
				PlaySound("Data/Die.wav", NULL, SND_SYNC);  // Play The Death Sound
			}
		}

		if (player.fx<player.x*60)           // Is Fine Position On X Axis Lower Than Intended Position?
		{
			player.fx+=steps[adjust];       // If So, Increase The Fine X Position
		}
		if (player.fx>player.x*60)           // Is Fine Position On X Axis Greater Than Intended Position?
		{
			player.fx-=steps[adjust];       // If So, Decrease The Fine X Position
		}
		if (player.fy<player.y*40)           // Is Fine Position On Y Axis Lower Than Intended Position?
		{
			player.fy+=steps[adjust];       // If So, Increase The Fine Y Position
		}
		if (player.fy>player.y*40)           // Is Fine Position On Y Axis Lower Than Intended Position?
		{
			player.fy-=steps[adjust];       // If So, Decrease The Fine Y Position
		}
	}

	if (filled)                     // Is The Grid Filled In?
	{
		PlaySound("Data/Complete.wav", NULL, SND_SYNC); // If So, Play The Level Complete Sound
		stage++;                    // Increase The Stage
		if (stage>3)                 // Is The Stage Higher Than 3?
		{
			stage=1;                // If So, Set The Stage To One
			level++;                // Increase The Level
			level2++;               // Increase The Displayed Level
			if (level>3)             // Is The Level Greater Than 3?
			{
				level=3;            // If So, Set The Level To 3
				lives++;            // Give The Player A Free Life
				if (lives>5)         // Does The Player Have More Than 5 Lives?
				{
					lives=5;        // If So, Set Lives To Five
				}
			}
		}
		ResetObjects();                 // Reset Player / Enemy Positions

		for (loop1=0; loop1<11; loop1++)     // Loop Through The Grid X Coordinates
		{
			for (loop2=0; loop2<11; loop2++) // Loop Through The Grid Y Coordinates
			{
				if (loop1<10)            // If X Coordinate Is Less Than 10
				{
					hline[loop1][loop2]=FALSE;  // Set The Current Horizontal Value To FALSE
				}
				if (loop2<10)            // If Y Coordinate Is Less Than 10
				{
					vline[loop1][loop2]=FALSE;  // Set The Current Vertical Value To FALSE
				}
			}
		}
	}
	// If The Player Hits The Hourglass While It's Being Displayed On The Screen
	if ((player.fx==hourglass.x*60) && (player.fy==hourglass.y*40) && (hourglass.fx==1))
	{
		// Play Freeze Enemy Sound
		PlaySound("Data/freeze.wav", NULL, SND_ASYNC | SND_LOOP);
		hourglass.fx=2;                 // Set The hourglass fx Variable To Two
		hourglass.fy=0;                 // Set The hourglass fy Variable To Zero
	}
	player.spin+=0.5f*steps[adjust];            // Spin The Player Clockwise
	if (player.spin>360.0f)                  // Is The spin Value Greater Than 360?
	{
		player.spin-=360;               // If So, Subtract 360
	}
	hourglass.spin-=0.25f*steps[adjust];            // Spin The Hourglass Counter Clockwise
	if (hourglass.spin<0.0f)             // Is The spin Value Less Than 0?
	{
		hourglass.spin+=360.0f;             // If So, Add 360
	}
	hourglass.fy+=steps[adjust];                // Increase The hourglass fy Variable
	if ((hourglass.fx==0) && (hourglass.fy>6000/level))  // Is The hourglass fx Variable Equal To 0 And The fy
	{                           // Variable Greater Than 6000 Divided By The Current Level?
		PlaySound("Data/hourglass.wav", NULL, SND_ASYNC);   // If So, Play The Hourglass Appears Sound
		hourglass.x=rand()%10+1;            // Give The Hourglass A Random X Value
		hourglass.y=rand()%11;              // Give The Hourglass A Random Y Value
		hourglass.fx=1;                 // Set hourglass fx Variable To One (Hourglass Stage)
		hourglass.fy=0;                 // Set hourglass fy Variable To Zero (Counter)
	}
	if ((hourglass.fx==1) && (hourglass.fy>6000/level))  // Is The hourglass fx Variable Equal To 1 And The fy
	{                           // Variable Greater Than 6000 Divided By The Current Level?
		hourglass.fx=0;                 // If So, Set fx To Zero (Hourglass Will Vanish)
		hourglass.fy=0;                 // Set fy to Zero (Counter Is Reset)
	}
	if ((hourglass.fx==2) && (hourglass.fy>500+(500*level)))// Is The hourglass fx Variable Equal To 2 And The fy
	{                           // Variable Greater Than 500 Plus 500 Times The Current Level?
		PlaySound(NULL, NULL, 0);           // If So, Kill The Freeze Sound
		hourglass.fx=0;                 // Set hourglass fx Variable To Zero
		hourglass.fy=0;                 // Set hourglass fy Variable To Zero
	}
	delay++;                        // Increase The Enemy Delay Counter
}



void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{
	switch ( key ) {
	case 27:        // When Escape Is Pressed...
		exit ( 0 );   // Exit The Program
		break;        // Ready For Next Case
	case 113: //q

		break;
	case 119: //w
		if(player.y > 0) //Check to see if player is at boarder
		{
			player.y--;             // Move The Player Up
			vline[player.x][player.y]=TRUE;     // Mark The Current Verticle Border As Filled
		}
		break;
	case 101: //e
		anti=!anti;                 // Toggle Antialiasing
		break;
	case 97: //a
		if(player.x > 0) //Check to see if player is at boarder
		{
			player.x--;             // Move The Player Left
			hline[player.x][player.y]=TRUE;     // Mark The Current Horizontal Border As Filled
		}
		break; 
	case 115: //s
		if(player.y < 10) //Check to see if player is at boarder
		{
			vline[player.x][player.y]=TRUE;     // Mark The Current Verticle Border As Filled
			player.y++;             // Move The Player Down
		}
		break;
	case 100: //d
		if(player.x <10) //Check to see if player is at boarder 
		{
			hline[player.x][player.y]=TRUE;     // Mark The Current Horizontal Border As Filled
			player.x++;             // Move The Player Right
		}
		break;
	case 120: //x

		break;
	case 32: //space
		if(gameover)
		{
			gameover=FALSE;             // gameover Becomes FALSE
			filled=TRUE;                // filled Becomes TRUE
			level=1;                // Starting Level Is Set Back To One
			level2=1;               // Displayed Level Is Also Set To One
			stage=1;                // Game Stage Is Set To Zero
			lives=5;                // Lives Is Set To Five
			ResetObjects();                 // Reset Player / Enemy Positions

			for (loop1=0; loop1<11; loop1++)     // Loop Through The Grid X Coordinates
			{
				for (loop2=0; loop2<11; loop2++) // Loop Through The Grid Y Coordinates
				{
					if (loop1<10)            // If X Coordinate Is Less Than 10
					{
						hline[loop1][loop2]=FALSE;  // Set The Current Horizontal Value To FALSE
					}
					if (loop2<10)            // If Y Coordinate Is Less Than 10
					{
						vline[loop1][loop2]=FALSE;  // Set The Current Vertical Value To FALSE
					}
				}
			}
		}
		break;
	case 102: //r

		break;
	case 114: //f

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
	glutInitWindowSize(640, 500); // If glutFullScreen wasn't called this is the window size
	glutCreateWindow("NeHe's OpenGL Framework"); // Window Title (argv[0] for current directory as title)
	//glutFullScreen();          // Put Into Full Screen

	init();
	glutDisplayFunc(display);  // Matching Earlier Functions To Their Counterparts
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(arrow_keys);

	glutIdleFunc(IdleFunc);
	glutMainLoop();          // Initialize The Main Loop

	return 0;
}

