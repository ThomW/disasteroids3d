

#define APP_TITLE "Disasteroids 3D"
#define VERSION "VERSION 0.2B (2000.04.25)"

#define INITGUID

#include <windows.h>			// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <math.h>				// Header File For Math Library
#include <time.h>				// Header File For Time Library
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

// DirectSound
#include <dsound.h>			// DirectSound header file
#include "dsutil.h"			// DirectSound utility header file

// DirectInput
#include <cguid.h>			// GUID Header required by DirectInput
#include "diutil.h"			// DirectInput utility header file

// Game data
#include "modeldata.h"		// Model data
#include "resource.h"		// Resource file data

// Timing code (from Ryan Haksi)
#include "timing.c"

// Function declarations
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

BOOL InitializeGame(void);
void TerminateGame(void);

// Windows variables
HDC		hDC					= NULL;	// Private GDI Device Context
HGLRC		hRC					= NULL;	// Permanent Rendering Context
HWND		hWnd					= NULL;	// Holds Our Window Handle
HINSTANCE	hInstance		= NULL;	// Holds The Instance Of The Application
PIXELFORMATDESCRIPTOR pfd;				// Pixel Format Descriptor variable
BOOL g_bGLAccelerated = FALSE;		// Flag that's TRUE when hardware supports
												// OpenGL acceleration

BOOL	g_bActive	= TRUE;				// Window active flag
BOOL	keys[256];							// Array Used For The Keyboard Routine
BOOL	fullscreen	= TRUE;				// Fullscreen Flag Set To Fullscreen Mode By Default


// Storage For Textures
const int NUM_TEXTURES = 13;
GLuint texture[NUM_TEXTURES];

// Font list
GLuint font_list;

// Open GL lists
const int NUM_LISTS = 12;						// Modified By NeHe (04/30/00)
GLuint player_list;
GLuint rock_list;
GLuint shot_list;
GLuint shields_list;
GLuint debris_list;
GLuint blast_list;
GLuint smoke_list;
GLuint lsaucer_list;
GLuint sfire_list;
GLuint background_list;
GLuint logo_list;
GLuint bigspace_list;							// Added By NeHe (04/30/00)


BOOL	g_bGlLightingEnabled		= TRUE;		// Lighting enabled flag
BOOL	g_bGlBlendingEnabled		= TRUE;		// Blending enabled flag
BOOL	g_bGlTexturesEnabled		= TRUE;		// Textures enabled flag
BOOL	g_bGlWireframeEnabled	= FALSE;		// Wireframe mode flag
BOOL	g_bDrawBoundingBoxes		= FALSE;		// Bounding box display flag

BOOL	g_bShowFps					= FALSE;		// Display frames per second flag
BOOL	g_bGameOver					= TRUE;		// Game over flag
int	g_nSpaceInMotion			= 2;			// SpaceInMotion has various modes
BOOL	g_bShowHelp					= FALSE;		// Help screen flag
BOOL	g_bGamePaused				= FALSE;		// Game paused flag
BOOL	g_bShowBindKeys			= FALSE;		// Key binding mode

int	g_nFps						= 0;			// Frames per second variable
float	g_fGameLength				= 0;			// Application run time variable
int	g_nLevelNum					= 0;			// Level number indicator

// Difficulty settings
const int MIN_DIFFICULTY		= 0;
const int SLOW_DIFFICULTY		= 0;
const int ORIGINAL_DIFFICULTY = 1;
const int MAX_DIFFICULTY		= 1;
int g_nDifficultySetting		= SLOW_DIFFICULTY;


// Default lighting setup for light 1
GLfloat LightAmbient[]	= { 0.05f, 0.05f, 0.05f, 1.0f };
GLfloat LightDiffuse[]	= { 0.3f, 0.3f, 0.7f, 1.0f }; 
GLfloat LightPosition[]	= { 7.0f, 7.0f, 5.0f, 1.0f };


const	float WORLD_WIDTH			= 22;		// Holds The Width Of Our World
const	float WORLD_HALFWIDTH	= 11;		// Holds Half The Width Of Our World
const	float WORLD_HEIGHT		= 16;		// Holds The Height Of Our World
const	float WORLD_HALFHEIGHT	= 8;		// Holds Half The Height Of Our World
const	float WORLD_DEPTH			= -20;	// The distance all the objects are from the camera


// Different actors have different behaviors and 
// rendering characteristics.  
const int ACTOR_NONE					= 0;
const int ACTOR_PLAYER				= 1;
const int ACTOR_ROCK					= 2;
const int ACTOR_SHOT					= 3;
const int ACTOR_SHIELDS				= 4;
const int ACTOR_DEBRIS				= 5;
const int ACTOR_BLAST				= 6;
const int ACTOR_PARTICLE			= 7;
const int ACTOR_FLAMINGPARTICLE	= 8;
const int ACTOR_LSAUCER				= 9;
const int ACTOR_SFIRE				= 10;
const int ACTOR_FLAMES				= 11;


// This is the maximum number of elements
// in the actor array.
const int MAX_ACTORS					= 210;


// Define how actors[] is split up
//
// Make sure the 3D objects are before the 2D
// so that things are rendered correctly
//
// In the future, I'm going to do this with
// pointers, but for now, this works.
const int IDX_NO_ACTOR			= 0;	// No actor can have index 0
const int IDX_FIRST_ROCK		= 1;
const int IDX_LAST_ROCK			= 29;
const int IDX_LSAUCER			= 30;
const int IDX_PLAYER				= 31;
const int IDX_FIRST_DEBRIS		= 32;
const int IDX_LAST_DEBRIS		= 116;
const int IDX_FIRST_BLAST		= 117;
const int IDX_LAST_BLAST		= 126;
const int IDX_FIRST_PARTICLE	= 127;
const int IDX_LAST_PARTICLE	= 186;
const int IDX_FIRST_SHOT		= 187;	// Make sure all the shots are lumped together
const int IDX_LAST_SHOT			= 198;
const int IDX_FIRST_SFIRE		= 199;
const int IDX_LAST_SFIRE		= 208;	// End of shots
const int IDX_SHIELDS			= 209;

// Game timing variables
float g_fNow			= 0;		// Holds current game Tick
float g_fNowOffset	= 0;		// Holds number of Ticks game was paused


// Color structure
typedef struct {
	GLbyte r;		// Red intensity (0-255)
	GLbyte g;		// Green intensity (0-255)
	GLbyte b;		// Blue intensity (0-255)
} udtRgbColor;


// Actor definition
typedef struct _actorinfo {
	int		type;				// This value should be set to one of the ACTORs above
	BOOL		active;			// Is this actor active (TRUE/FALSE)
	GLfloat	x;					// X position of the actor
	GLfloat	y;					// Y position of the actor
	GLfloat	z;					// Z position of the actor
	GLfloat	rx;				// X rotation of the actor
	GLfloat	ry;				// Y rotation of the actor
	GLfloat	rz;				// Z rotation of the actor
	GLfloat	vx;				// X velocity of the actor
	GLfloat	vy;				// Y velocity of the actor
	GLfloat	vz;				// Z velocity of the actor
	GLuint	displaylistid;	// Defines the display list id for this actor
	udtRgbColor color;		// Defines the color for this actor
	float		LifeStartTime;	// Start time of actor's life cycle
	float		LifeEndTime;	// End time of the actor's life cycle
	float		LifePercent;	// Percentage of life remaing for actor
	float		size;			
	BOOL		ShieldsUp;
	GLfloat	ShieldsPower;
	long		Lives;
	long		NextBonus;
	long		Score;
	float		ShotDelay;
	float		ChangeDirectionDelay;
	long		PointValue;
	float		InvincibleEndTime;

	float	xcolldist;			// X collision distance (bounding box x radius)
	float	ycolldist;			// Y collision distance (bounding box y radius)
	
} actorinfo;

actorinfo actors[MAX_ACTORS];
actorinfo *player;


// Precomputed values used throughout
const float pi						= 3.1415926535f;
const float inversepi			= 0.3183098861f;
const float convertdegtorad	= 0.0174532925f;


// Background motion velocity
GLfloat g_fBackgroundRot = 0.0f;


// Sound stuff (mostly ripped from DirectX SDK Duel and modified)
BOOL g_bDirectSoundEnabled = FALSE;
BOOL g_bSoundEnabled = FALSE;
SoundObject* g_pBulletFiringSound	= NULL;
SoundObject* g_pSmallExplodeSound	= NULL;
SoundObject* g_pMediumExplodeSound  = NULL;
SoundObject* g_pLargeExplodeSound	= NULL;
SoundObject* g_pShipThrustSound		= NULL;
SoundObject* g_pThumpHiSound			= NULL;
SoundObject* g_pThumpLoSound			= NULL;
SoundObject* g_pLSaucerSound			= NULL;
SoundObject* g_pSaucerFireSound		= NULL;
SoundObject* g_pLifeSound				= NULL;
const int NUM_SHIP_TYPES = 9;


// DirectInput stuff (ripped from DirectX SDK FFDonuts and modified)
BOOL g_bDirectInputEnabled							= FALSE;
LPDIRECTINPUTDEVICE2 g_pdidJoystick				= NULL;
FFEffectObject*      g_pExplodeFFEffect		= NULL;
DWORD                g_dwForceFeedbackGain	= 100L;
BOOL g_bJoystickEnabled			= FALSE;
BOOL g_bForceFeedbackCapable	= FALSE;
BOOL g_bForceFeedbackEnabled	= FALSE;
const int JOYSTICK_LEFT		= 1;
const int JOYSTICK_RIGHT	= 2;
const int JOYSTICK_UP		= 4;
const int JOYSTICK_DOWN		= 8;
const int JOYSTICK_FIRE		= 16;
const int JOYSTICK_SHIELD	= 32;
const int JOYSTICK_STOP		= 64;


// Variables for dealing with highscore initial entry
// High score variables and initialization
const int MAX_HIGHSCORES = 10;
typedef struct _highscoreinfo {
	char		Initials[4];
	long		Score;
	int		Level;
} highscoreinfo;
// Init the highscores with my initials -- I'm a WINNER!!!! :)
highscoreinfo	highscores[MAX_HIGHSCORES] = {
		"TCW", 1000, 1,
		"TCW", 1000, 1,
		"TCW", 1000, 1,
		"TCW", 1000, 1,
		"TCW", 1000, 1,
		"TCW", 1000, 1,
		"TCW", 1000, 1,
		"TCW", 1000, 1,
		"TCW", 1000, 1,
		"TCW", 1000, 1 };
BOOL	g_bEnteringHighScoreInitials		= FALSE;
char	g_nHighScoreInitialsIdx				= 0;
const char g_cLegalHighScoreChars[38]	= " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
highscoreinfo *playerhighscore			= NULL;


// Message display variables
const long MESSAGE_LIFE = 2;
long g_lMessageEndTime	= 0;
char g_sMessage[256]		= "";


// Key binds
const int THRUST_KEY		= 0;
const int SHIELDS_KEY	= 1;
const int ROTLEFT_KEY	= 2;
const int ROTRIGHT_KEY	= 3;
const int FIRE_KEY		= 4;
const int GAMESTART_KEY = 5;
const int NUM_KEY_BINDS = 6;
char g_sKeyBindsText[NUM_KEY_BINDS][40] = 
{
	"THRUST", "SHIELDS", "ROTATE LEFT", "ROTATE RIGHT", "FIRE", "START GAME"
};
int g_anKeyBinds[6] = 
{
	VK_UP,
	VK_DOWN,
	VK_LEFT,
	VK_RIGHT,
	' ',
	'1'
};

// Pre-Copernicus Mode
BOOL	g_bPreCopernicusMode		= FALSE;
float g_fBackgroundX = 0;
float g_fBackgroundY = 0;
BOOL  g_bStarsActive = FALSE;
const int NUM_STARS = 50;
const float STAR_WORLD_WIDTH = WORLD_WIDTH + 4.0f;
const float STAR_WORLD_HALFWIDTH = STAR_WORLD_WIDTH * 0.5f;
const float STAR_WORLD_HEIGHT = WORLD_HEIGHT + 4.0f;
const float STAR_WORLD_HALFHEIGHT = STAR_WORLD_HEIGHT * 0.5f;
const float MIN_STAR_Z = WORLD_DEPTH - 5.0f;
const float MAX_STAR_Z = WORLD_DEPTH + 5.0f;
const float MIN_STAR_SIZE = 0.05f;
const float MAX_STAR_SIZE = 0.15f;
typedef struct _starinfo {
	GLfloat	x;					// X position of the actor
	GLfloat	y;					// Y position of the actor
	GLfloat	z;					// Z position of the actor
	GLfloat	size;				// Size of star
} starinfo;
starinfo stars[NUM_STARS];


//
// TCW
// Sin and Cos lookup code
//
float fSinTable[360]; 
float fCosTable[360];

void InitLookupTables(void)
{
	int i = 0;
	float fRadAngle = 0;

	for (i = 0; i < 360; i++)
	{
		fRadAngle = i * convertdegtorad;
		fSinTable[i] = (float)sin(fRadAngle);
		fCosTable[i] = (float)cos(fRadAngle);
	}
}

inline float CosLookup(const int& angle)
{
	return fCosTable[abs(angle) % 360];
}

inline float CosLookup(const float& angle)
{
	return CosLookup(int(angle));
}

inline float SinLookup(const int& angle)
{
	if (angle < 0) 
		return -fSinTable[abs(angle) % 360];
	else
		return fSinTable[angle % 360];
}

inline float SinLookup(const float& angle)
{
	return SinLookup(int(angle));
}


//--------------------------------------------------------
// TCW
//
// Send TRUE to Pause to pause game, send FALSE
// to resume game
//--------------------------------------------------------
void PauseGame(const BOOL& Start)
{
	static float fStartTime = 0;

	if (Start)
	{
		if (!g_bGamePaused) 
		{
			fStartTime = TimingGetTime();
			g_bGamePaused = TRUE;
		}
	}
	else
	{
		if (g_bGamePaused)
		{
			g_fNowOffset += TimingGetTime() - fStartTime;
			g_bGamePaused = FALSE;
		}
	}
}


//--------------------------------------------------------
// TCW
// Get an random integer within a specified range
//--------------------------------------------------------
int randInt(const int& min, const int& max) {
    return ((rand()%(int)(((max) + 1)-(min)))+ (min)); 
}

//--------------------------------------------------------
// Get a random float within a specified range
//--------------------------------------------------------
float randFloat(const float& min, const float& max) {
	float range = max - min;
	float num = range * rand() / RAND_MAX;
	return (num + min);
}


//--------------------------------------------------------
// TCW
//
// Rendering feature flags
//
// This code was added after noticing the screen flashing
// white after making the debris untextured.  I remembered
//	reading that glEnabling GL_TEXTURE_2D unnecessarily
// causes problems with GeForce cards, and also that 
// calling glEnable and glDisable is a big waste of time,
// so I came up with this system of internal flags to 
// control the various rendering options.  It could be
// expanded to include a lot more, but this is all that
// I need for this game.
//--------------------------------------------------------
const int render_blend		= 1;
const int render_texture	= 2;
const int render_depthtest	= 4;
const int render_lighting	= 8;
int renderflags = 0;

void renderenable(const int& flags)
{
	if (((flags & render_blend) == render_blend) && ((renderflags & render_blend) != render_blend))
		glEnable(GL_BLEND);
 
	if (((flags & render_texture) == render_texture) && ((renderflags & render_texture) != render_texture))
		glEnable(GL_TEXTURE_2D);

	if (((flags & render_depthtest) == render_depthtest) && ((renderflags & render_depthtest) != render_depthtest))
		glEnable(GL_DEPTH_TEST);

	if (((flags & render_lighting) == render_lighting) && ((renderflags & render_lighting) != render_lighting))
		glEnable(GL_LIGHTING);

	renderflags = renderflags | flags;
}
 
void renderdisable(const int& flags)
{
	if (((flags & render_blend) == render_blend) && ((renderflags & render_blend) == render_blend))
		glDisable(GL_BLEND);

	if (((flags & render_texture) == render_texture) && ((renderflags & render_texture) == render_texture))
		glDisable(GL_TEXTURE_2D);

	if (((flags & render_depthtest) == render_depthtest) && ((renderflags & render_depthtest) == render_depthtest))
		glDisable(GL_DEPTH_TEST);

	if (((flags & render_lighting) == render_lighting) && ((renderflags & render_lighting) == render_lighting))
		glDisable(GL_LIGHTING);

	renderflags = renderflags - (renderflags & flags);
}

void renderset(const int& flags, const BOOL& value)
{
	if (value)
		renderenable(flags);
	else
		renderdisable(flags);
}

// This routine is used to reset the rendering
// options after reinitializing the window during
// fullscreen/window toggle
void renderinit(void)
{
	if ((renderflags & render_blend) == render_blend)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
 
	if ((renderflags & render_texture) == render_texture) 
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
	
	if ((renderflags & render_depthtest) == render_depthtest) 
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	if ((renderflags & render_lighting) == render_lighting) 
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

}
 

//
// This source code, along with the object definitions were generated
// by 3D Explorer.  I made some modifications to each to allow me to 
// load both player and rock object data.  After the player and rock
// are setup, shots and debris are hand-coded and VERY impessive (that's sarcasm, folks)
//
GLint Gen3DObjectList()
{
 
	int i =0;
	int j =0;

	// Create display lists
	player_list = glGenLists(NUM_LISTS);

	// Player
	glNewList(player_list, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBegin (GL_TRIANGLES);
			for(i=0;i<sizeof(player_face_indicies)/sizeof(player_face_indicies[0]);i++)
			{
				for(j=0;j<3;j++)
				{
					int vi = player_face_indicies[i][j];
					int ni = player_face_indicies[i][j+3];
					int ti = player_face_indicies[i][j+6];
					glNormal3f (player_normals[ni][0],player_normals[ni][1],player_normals[ni][2]);
					glTexCoord2f(player_textures[ti][0],player_textures[ti][1]);
					glVertex3f (player_verticies[vi][0],player_verticies[vi][1],player_verticies[vi][2]);         
				}
			}
		glEnd ();
	glEndList();

	// Read rock data from arrays
	rock_list = player_list + 1;
	glNewList(rock_list, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[6]);
		glBegin (GL_TRIANGLES);
			for(i=0;i<sizeof(rock_face_indicies)/sizeof(rock_face_indicies[0]);i++)
			{
				for(j=0;j<3;j++)
				{
					int vi=rock_face_indicies[i][j];
					int ni=rock_face_indicies[i][j+3];
					int ti=rock_face_indicies[i][j+6];
					glNormal3f (rock_normals[ni][0],rock_normals[ni][1],rock_normals[ni][2]);
					glTexCoord2f(rock_textures[ti][0],rock_textures[ti][1]); 
					glVertex3f (rock_verticies[vi][0],rock_verticies[vi][1],rock_verticies[vi][2]);
				}
			}
		glEnd ();
	glEndList();

	// Draw shot as textured quad
	shot_list = rock_list + 1;
	glNewList(shot_list, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,-0.5f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,-0.5f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f, 0.5f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.0f);

		glEnd();
	glEndList();

	// Draw shields as textured quad
	shields_list = shot_list + 1;
	glNewList(shields_list, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();
	glEndList();

	// Debris - textured triangle
	debris_list = shields_list + 1;
	glNewList(debris_list, GL_COMPILE);
		glBegin(GL_QUADS);
			glVertex3f(-0.15f, -0.15f,  0.0f);
			glVertex3f( 0.15f, -0.15f,  0.0f);
			glVertex3f( 0.15f,  0.15f,  0.0f);
			glVertex3f(-0.15f,  0.15f,  0.0f);
		glEnd();

	glEndList();

	// Blast
	blast_list = debris_list + 1;
	glNewList(blast_list, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glBegin(GL_QUADS);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  0.0f);
		glEnd();
	glEndList();
		
	// Smoke - textured quad
	smoke_list = blast_list + 1;
	glNewList(smoke_list, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glBegin(GL_QUADS);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.25f, -0.25f,  0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.25f, -0.25f,  0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.25f,  0.25f,  0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.25f,  0.25f,  0.0f);
		glEnd();
	glEndList();

	// Read ufo data from arrays
	lsaucer_list = smoke_list + 1;
	glNewList(lsaucer_list, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[11]);
		glBegin (GL_TRIANGLES);
		for(i=0;i<sizeof(ufo_face_indicies)/sizeof(ufo_face_indicies[0]);i++)
			{
				for(j=0;j<3;j++)
				{
					int vi = ufo_face_indicies[i][j];
					int ni = ufo_face_indicies[i][j+3];
					int ti = ufo_face_indicies[i][j+6];
					glNormal3f  (ufo_normals[ni][0]  ,ufo_normals[ni][1],  ufo_normals[ni][2]);
					glTexCoord2f(ufo_textures[ti][0] ,ufo_textures[ti][1]);
					glVertex3f  (ufo_verticies[vi][0],ufo_verticies[vi][1],ufo_verticies[vi][2]);
				}
			}
		glEnd ();
	glEndList();

	// Draw sfire as textured quad
	sfire_list = lsaucer_list + 1;
	glNewList(sfire_list, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glBegin(GL_QUADS);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();
	glEndList();

	// Draw background as textured quad
	background_list = sfire_list + 1;
	glNewList(background_list, GL_COMPILE);
		
		// Upper left
		glBindTexture(GL_TEXTURE_2D, texture[7]);
		glBegin(GL_QUADS);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-WORLD_WIDTH, WORLD_HEIGHT * 1.4f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-WORLD_WIDTH, 0.0f        , 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f       , 0.0f        , 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.0f       , WORLD_HEIGHT * 1.4f, 0.0f);
		glEnd();
		
		// Upper right
		glBindTexture(GL_TEXTURE_2D, texture[8]);
		glBegin(GL_QUADS); 
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f       , WORLD_HEIGHT * 1.4f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f       , 0.0f        , 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( WORLD_WIDTH, 0.0f        , 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( WORLD_WIDTH, WORLD_HEIGHT * 1.4f, 0.0f);
		glEnd();
		
		// Lower left
		glBindTexture(GL_TEXTURE_2D, texture[9]);
		glBegin(GL_QUADS); 
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-WORLD_WIDTH, 0.0f        , 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-WORLD_WIDTH,-WORLD_HEIGHT * 1.4f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f       ,-WORLD_HEIGHT * 1.4f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.0f       , 0.0f        , 0.0f);
		glEnd();
		
		// Lower right
		glBindTexture(GL_TEXTURE_2D, texture[10]);
		glBegin(GL_QUADS); 
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f       , 0.0f        , 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f       ,-WORLD_HEIGHT * 1.4f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( WORLD_WIDTH,-WORLD_HEIGHT * 1.4f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( WORLD_WIDTH, 0.0f        , 0.0f);
		glEnd();
	glEndList();

	// Logo
	logo_list = background_list + 1;
	glNewList(logo_list, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[12]);
		glBegin(GL_QUADS);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f,-0.5f); glVertex3f(-2.0f,-0.5f, 0.0f);
			glTexCoord2f(1.0f,-0.5f); glVertex3f( 0.0f,-0.5f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f, 0.5f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, 0.5f, 0.0f);
		glEnd();
		glBegin(GL_QUADS);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f,-1.0f); glVertex3f(-0.0f,-0.5f, 0.0f);
			glTexCoord2f(1.0f,-1.0f); glVertex3f( 2.0f,-0.5f, 0.0f);
			glTexCoord2f(1.0f,-0.5f); glVertex3f( 2.0f, 0.5f, 0.0f);
			glTexCoord2f(0.0f,-0.5f); glVertex3f(-0.0f, 0.5f, 0.0f);
		glEnd();
	
		
	glEndList();
	
// ******************** Modified By NeHe (04/30/00) ********************
	
	// Big Space
	bigspace_list = logo_list + 1;
	glNewList(bigspace_list, GL_COMPILE);
		glColor4ub(255, 255, 255, 128);
		glTranslatef(-WORLD_WIDTH*2,-WORLD_HEIGHT*2,0.0f);
		glCallList(background_list);
		glTranslatef(+WORLD_WIDTH*2,0.0f,0.0f);
		glCallList(background_list);
		glTranslatef(+WORLD_WIDTH*2,0.0f,0.0f);
		glCallList(background_list);
		glTranslatef(-WORLD_WIDTH*2,0.0f,0.0f);

		glTranslatef(-WORLD_WIDTH*2,+WORLD_HEIGHT*2,0.0f);
		glCallList(background_list);
		glTranslatef(+WORLD_WIDTH*2,0.0f,0.0f);
		glCallList(background_list);
		glTranslatef(+WORLD_WIDTH*2,0.0f,0.0f);
		glCallList(background_list);
		glTranslatef(-WORLD_WIDTH*2,0.0f,0.0f);

		glTranslatef(-WORLD_WIDTH*2,+WORLD_HEIGHT*2,0.0f);
		glCallList(background_list);
		glTranslatef(+WORLD_WIDTH*2,0.0f,0.0f);
		glCallList(background_list);
		glTranslatef(+WORLD_WIDTH*2,0.0f,0.0f);
		glCallList(background_list);
		glTranslatef(-WORLD_WIDTH*2,0.0f,0.0f);
	glEndList();

// ******************** Modified By NeHe (04/30/00) ********************

	return player_list;
}


GLvoid KillDisplayLists(GLvoid)
{
	glDeleteLists(player_list, NUM_LISTS);
}


int LoadGLTextures()		
{
/////////////////////
	HBITMAP hBMP;
	BITMAP	BMP;
/////////////////////


	// NOTE TO OTHERS:
	//
	//		Does anyone know how to load 8-bit bitmaps from
	//		a resource file?  Right now all of the textures
	//		are 16-bit and really don't need to be.  Thanks!


	int		loop = 0;
	int		Status = TRUE;
	byte	Texture[]={	IDB_PLAYER,	IDB_STAR,	IDB_SHIELD,
						IDB_BLAST,	IDB_SMOKE,	IDB_FONT,
						IDB_ROCK,	IDB_SPACEUL,IDB_SPACEUR,
						IDB_SPACELL,IDB_SPACELR,IDB_SAUCER,
						IDB_LOGO};

	glGenTextures(sizeof(Texture), &texture[0]);

	for (loop = 0; loop < sizeof(Texture); loop++)
	{
		hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL),MAKEINTRESOURCE(Texture[loop]), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

		if (hBMP)
		{
			GetObject(hBMP,sizeof(BMP), &BMP);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			glBindTexture(GL_TEXTURE_2D, texture[loop]);
			if ((loop > 6) && (loop < 11) || (loop == 12))
			{
				glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, BMP.bmWidth, BMP.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
			}
		}
		else
		{
			Status=FALSE;
		}
	}
	return Status;
}



// NeHe
GLvoid BuildFont(GLvoid)								// Build Our Font Display List
{
	float	cx;												// Holds Our X Character Coord
	float	cy;												// Holds Our Y Character Coord
	int loop = 0;

	font_list=glGenLists(256);							// Creating 256 Display Lists
	glBindTexture(GL_TEXTURE_2D, texture[5]);		// Select Our Font Texture
	for (loop=0; loop<256; loop++)					// Loop Through All 256 Lists
	{
		cx=float(loop%16)/16.0f;						// X Position Of Current Character
		cy=float(loop/16)/16.0f;						// Y Position Of Current Character

		glNewList(font_list+loop,GL_COMPILE);		// Start Building A List
			glBegin(GL_QUADS);							// Use A Quad For Each Character
				glTexCoord2f(cx,1-cy-0.0625f);		// Texture Coord (Bottom Left)
				glVertex2f(0.0f,0.0f);					// Vertex Coord (Bottom Left)
				glTexCoord2f(cx+0.0625f,1-cy-0.0625f);	// Texture Coord (Bottom Right)
				glVertex2f(1.0f,0.0f);					// Vertex Coord (Bottom Right)
				glTexCoord2f(cx+0.0625f,1-cy);		// Texture Coord (Top Right)
				glVertex2f(1.0f,1.0f);					// Vertex Coord (Top Right)
				glTexCoord2f(cx,1-cy);					// Texture Coord (Top Left)
				glVertex2f(0.0f,1.0f);					// Vertex Coord (Top Left)
			glEnd();											// Done Building Our Quad (Character)
			glTranslatef(0.6f,0.0f,0.0f);				// Move To The Right Of The Character
		glEndList();										// Done Building The Display List
	}															// Loop Until All 256 Are Built
}

GLvoid KillFont(GLvoid)									// Delete The Font From Memory
{
	glDeleteLists(font_list,256);						// Delete All 256 Display Lists
}



// (NeHe)
GLvoid glPrint(int set, const GLfloat& x, const GLfloat& y, const char *string, ...)
{
	if (set>1)
	{
		set=1;
	}

	char	text[256];									// Holds Our String
	va_list	ap;											// Pointer To List Of Arguments

	if (string==NULL)
	{
		return;
	}
	
	// Parse The String For Variables And Convert 
	// Symbols To Actual Numbers
	va_start(ap, string);
		vsprintf(text, string, ap);
	va_end(ap);

	glLoadIdentity();									// Reset The Projection Matrix

	// Setup rendering options
	renderenable(render_blend | render_texture);
	renderdisable(render_depthtest | render_lighting);

	glListBase(font_list - 32 + (128 * set));			// Choose The Font Set (0 or 1)
	glBindTexture(GL_TEXTURE_2D, texture[5]);			// Select Our Font Texture
	glTranslated(x,y,-30);								// Position The Text
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Write The Text To The Screen
	
}


//
// TCW, but based 99% on code from Jeff
//
GLvoid glPrintCentered(int set, const GLfloat& y, const char *string, ...)
{

	if (set>1)
		set=1;

	char	text[256];									// Holds Our String
	va_list	ap;											// Pointer To List Of Arguments
	GLfloat x = 0;

	if (string==NULL)
		return;
	
	// Parse The String For Variables And Convert 
	// Symbols To Actual Numbers
	va_start(ap, string);
		vsprintf(text, string, ap);
	va_end(ap);

	// Calc x
	x = (GLfloat)-(strlen(text) * 0.6 / 2.0f);

	// Call glPrint
	glPrint(set, x, y, text);

}


//--------------------------------------------------------
// TCW
// This routine gives me an easy way to post one line
// of messages along the bottom of the screen to notify
// the user when controls are enabled/disabled and 
// rendering options are toggled.
//--------------------------------------------------------
void UpdateMessage(const char *string, ...)
{

	char	text[256];
	va_list	ap;

	// Deactivate any messages if the 
	// message string is not specified
	if (string == NULL)
	{
		g_lMessageEndTime = 0;
		return;
	}
	
	// Parse The String For Variables And Convert 
	// Symbols To Actual Numbers
	va_start(ap, string);
		vsprintf(text, string, ap);
	va_end(ap);

	strcpy(g_sMessage, text);
	g_lMessageEndTime = (long)g_fNow + MESSAGE_LIFE;
}


// (NeHe)
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
		height=1;										// Making Height Equal One

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}


//======================================================
// (NeHe)
// TCW
//	I made some modifications to load all of the
//	various elements that I'm using in the program
//	such as the lighting model, 3D object loading,
//	font building, etc.
//======================================================
int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	// Required routines to enable texture mapping
	if (!LoadGLTextures())	
		return FALSE;									// If Texture Didn't Load Return FALSE

	// Enable Texture Mapping
	renderenable(render_texture);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glShadeModel(GL_FLAT);								// Enable Flat Shading
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup

	renderenable(render_depthtest);						// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	
	// Attempt to use this to fix the lighting...
	glEnable(GL_NORMALIZE); 

	renderenable(render_lighting);						// Enable Lighting
	
	//	glEnable(GL_LIGHT0);								// Quick And Dirty Lighting (Assumes Light0 Is Set Up)
	
	// Fixed by NeHe
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT,GL_SHININESS,128);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	// Added for drawing transparent objects
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);					// Set The Blending Function For Translucency
	renderenable(render_blend);							// Enable Blending

	// Setup default light
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);								// Enable Light One
	
	// Build ObjectLists of 3d objects
	Gen3DObjectList();

	// Build the font
	BuildFont();
	
	// Initialization Went OK
	return TRUE;
}


// TCW
// Load highscores
BOOL LoadHighScores(char *Filename)				
{
	FILE *File=NULL;
	
	if (!Filename)
		return FALSE;
	
	File=fopen(Filename,"r");

	if (File)
	{
		fread( highscores, sizeof( highscoreinfo ), MAX_HIGHSCORES, File);
		fclose(File);
		return TRUE;
	}

	return FALSE;
}


// TCW
// Save highscores
BOOL SaveHighScores(char *Filename)				
{
	FILE *File=NULL;
	
	if (!Filename)
		return FALSE;
	
	File=fopen(Filename,"w");
	if (File)
	{
		fwrite( highscores, sizeof( highscoreinfo ), MAX_HIGHSCORES, File);
		fclose(File);
		return TRUE;
	}
	
	return FALSE;
}



// TCW
// This is where the magic happens ;)
//
int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{

	int		i						= 0;
	int		nLastType			= ACTOR_NONE;
	float		fScalar				= 0;
	int		nLightIdx			= 0;
	int		nLightCount			= 0;
	float		fRemainingLifePct	= 0;
	float		fx						= 0;
	float		fy						= 0;
	float		fz						= 0;
	float		fs						= 0;
	static int nLastLightCount;


	GLfloat afLightData[4]  = {0.0f, 0.0f, 0.0f, 0.0f};
	
	// Clear The Screen And The Depth Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	
	// Set global flag to indicate that the stars
	// aren't being rendered -- when the stars ARE
	// rendered, the flag will be set then to indicate
	// that they ARE :)
	g_bStarsActive = FALSE;
	

	// Draw background
	if (g_bGlTexturesEnabled) 
	{

		if (g_bPreCopernicusMode)
		{
			glClearDepth(1.0f);

			switch(g_nSpaceInMotion)
			{

			// Method zero draws the background twice, 
			// once shifted for the player's movement,
			// then again further away and rotated 180
			// degrees.
			//
			// It looks really cool, but I don't know
			// when to reset the view, so I'm not going
			// to use it.
			case 0:
		
				glLoadIdentity();
				renderenable(render_texture | render_depthtest | render_blend);
				renderdisable(render_lighting);
				
				glClearDepth(1.0f);

				fScalar = float(0.25f * sin(g_fNow));
				glTranslatef(fScalar, fScalar, -30.0f - fScalar);
				glRotatef(fScalar, fScalar, -fScalar, 1.0f);

				glMatrixMode(GL_TEXTURE_2D);
				glMatrixMode(GL_MODELVIEW);
				
// ******************** Modified By NeHe (04/30/00) ********************
				if (g_fBackgroundX>WORLD_WIDTH*2)
				{
					g_fBackgroundX-=WORLD_WIDTH*2;
				}
				if (g_fBackgroundX<-WORLD_WIDTH*2)
				{
					g_fBackgroundX+=WORLD_WIDTH*2;
				}

				if (g_fBackgroundY>WORLD_HEIGHT*2)
				{
					g_fBackgroundY-=WORLD_HEIGHT*2;
				}
				if (g_fBackgroundY<-WORLD_HEIGHT*2)
				{
					g_fBackgroundY+=WORLD_HEIGHT*2;
				}

				glTranslatef(g_fBackgroundX, g_fBackgroundY, 0);
				glColor4ub(255, 255, 255, 180);
				glCallList(bigspace_list);

				glTranslatef(0, 0, 5.0f);
				glColor4ub(255, 255, 255, 128);
				glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
				glCallList(bigspace_list);
				fScalar = -0.25f * (float)sin(g_fNow / 1000.0f);
// ******************** Modified By NeHe (04/30/00) ********************

				break;

			// Draw background then stars
			// (The break is missing on purpose)
			case 1:
				// Draw background
				glLoadIdentity();
				renderenable(render_texture | render_depthtest);
				renderdisable(render_blend | render_lighting);
				glColor4ub(255,255,255,128);
				fScalar = 0.25f * (float)sin(g_fNow);
				glTranslatef(fScalar, fScalar, -30.0f - fScalar);
				glRotatef(fScalar, fScalar, -fScalar, 1.0f);
				glMatrixMode(GL_TEXTURE_2D);
				glMatrixMode(GL_MODELVIEW);
				glCallList(background_list);

			case 2:
				// Draw stars

				// Setup global flag that tells main routine 
				// to recalc stars' positions next cycle
				g_bStarsActive = TRUE;

				// Triangle strips and glVertex3f used to 
				// speed up rendering.  See nehe.gamedev.net
				// lesson 19.  I did! :)
				glLoadIdentity();
				renderenable(render_blend | render_texture);
				renderdisable(render_depthtest | render_lighting);
				glBindTexture(GL_TEXTURE_2D, texture[1]);
				glColor4ub(255, 255, 255, 240);
				for (i = 0; i < NUM_STARS; i++)
				{

					fx = stars[i].x;
					fy = stars[i].y;
					fz = stars[i].z;
					fs = stars[i].size;

					glBegin(GL_TRIANGLE_STRIP);						// Build Quad From A Triangle Strip
						glTexCoord2d(1,1); glVertex3f(fx + fs, fy + fs, fz); // Top Right
						glTexCoord2d(0,1); glVertex3f(fx - fs, fy + fs, fz); // Top Left
						glTexCoord2d(1,0); glVertex3f(fx + fs, fy - fs, fz); // Bottom Right
						glTexCoord2d(0,0); glVertex3f(fx - fs, fy - fs, fz); // Bottom Left
					glEnd();										// Done Building Triangle Strip
				}

				break;
			}
		}


		else
		{
			switch (g_nSpaceInMotion)
			{
			case 0:
				glLoadIdentity();
				renderenable(render_texture | render_depthtest); 
				renderdisable(render_lighting);
				renderset(render_blend, g_bGlBlendingEnabled);
				glClearDepth(1.0f);
				glColor4ub(255,255,255,255);
				glTranslatef(0.0f, 0.0f, -35.0f);
				glCallList(background_list);
				break;
			case 1:
				glLoadIdentity();
				renderenable(render_texture | render_depthtest);
				renderdisable(render_blend | render_lighting);
				glClearDepth(1.0f);
				glColor4ub(255,255,255,128);
				fScalar = float(0.25f * sin(g_fNow));
				glTranslatef(fScalar, fScalar, -30.0f - fScalar);
				glRotatef(fScalar, fScalar, -fScalar, 1.0f);
				glMatrixMode(GL_TEXTURE_2D);
				glMatrixMode(GL_MODELVIEW);
				glCallList(background_list);
				break;
			case 2:
				glLoadIdentity();
				renderdisable(render_blend | render_lighting);
				renderenable(render_texture | render_depthtest);
				glClearDepth(1.0f);
				glColor4ub(255,255,255,128);
				glTranslatef(0.0f, 0.0f, -30.0f);
				glMatrixMode(GL_TEXTURE_2D);
				glRotatef(180.0f * float(cos(g_fBackgroundRot/10.0f)), 0.0f, 0.0f, 1.0f);
				glTranslatef((3.0f*float(cos(g_fBackgroundRot)))+(2.1f*float(sin(g_fBackgroundRot*1.4f))),(2.8f*float(sin(g_fBackgroundRot)))+(1.3f*float(sin(g_fBackgroundRot*1.4f))),0.0f);
				glRotatef(10.0f*float(sin(g_fBackgroundRot*1.2f)),1.0f,0.0f,0.0f);
				glMatrixMode(GL_MODELVIEW);
				glCallList(background_list);
				break;
			}
		}
	}

	// Display extra men
	glLoadIdentity();
	glTranslatef(-8.5f, 8.25f,-25.0f);
	glScalef(0.5f, 0.5f, 0.5f);
	for (i = 0; i < player->Lives; i++) {
		glTranslatef(1.5f,0.0f,0.0f);
		glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
		glCallList(player->displaylistid);
		glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);
	}

	
	// Don't do this if lighting is disabled (duh)
	if (g_bGlLightingEnabled)
	{
		// Position and enable lights for light sources
		nLightCount = 0;
		for (i = 0; i < MAX_ACTORS; i++) 
		{

			if ((actors[i].type == ACTOR_BLAST) || 
				(actors[i].type == ACTOR_SHOT)  ||
				(actors[i].type == ACTOR_SFIRE) ||
				(actors[i].type == ACTOR_PLAYER) ||
				(actors[i].type == ACTOR_LSAUCER)||
				(actors[i].type == ACTOR_SHIELDS))
			{
				nLightIdx = GL_LIGHT2 + nLightCount;
					
				if (nLightIdx < GL_LIGHT0 + GL_MAX_LIGHTS) 
				{
					if (actors[i].active)
					{

						glLoadIdentity();

						afLightData[0] = actors[i].x;
						afLightData[1] = actors[i].y;
						afLightData[2] = actors[i].z + 1;
						afLightData[3] = 1.0f;
						glLightfv(nLightIdx, GL_POSITION, afLightData);

						switch(actors[i].type)
						{
						case ACTOR_SFIRE:
							afLightData[0] = 1.0f;
							afLightData[1] = 0.25f;
							afLightData[2] = 0.25f;
							afLightData[3] = 1.0f;
							break;

						case ACTOR_BLAST:

							// Compute fLifePctRemaining
							fRemainingLifePct = 1.0f - actors[i].LifePercent;
							
							afLightData[0] = (0.75f * fRemainingLifePct) + 0.25f;
							afLightData[1] = (0.75f * fRemainingLifePct) + 0.25f;
							afLightData[2] = 0.25f;
							afLightData[3] = 1.0f;
							break;
						
						case ACTOR_SHIELDS:
							afLightData[0] = 1.0f * ((255 - player->ShieldsPower) / 255);
							afLightData[1] = 1.0f * (player->ShieldsPower / 255);
							afLightData[2] = 0.2f;
							afLightData[3] = 0.7f;
							break;

						default:
							afLightData[0] = 0.2f;
							afLightData[1] = 0.2f;
							afLightData[2] = 0.2f;
							afLightData[3] = 1.0f;
							break;

						}

						// Setup and enable the light
						glLightfv(nLightIdx, GL_DIFFUSE, afLightData);

						// Don't enable lights that are already on
						if (nLastLightCount <= nLightCount)
							glEnable(nLightIdx);

						// Increment the lights counter
						nLightCount++;
					}

				}
				else
				{
					if (i == IDX_FIRST_SHOT) 
					{
						glPrint(0, 0.0f, 0.0f, "MAX LIGHTS Exceeded");
						glPrint(0, 0.0f, 1.0f, "MAX_LIGHTS=%d nLightIdx=%d", GL_MAX_LIGHTS, nLightIdx);
					}
				}
			}
		}
	} // if (gbLightingEnabled)

	// Turn off any extra lights that were activated last
	// cycle that aren't needed now
	if (nLastLightCount > nLightCount)
	{
		for (i = nLightCount; i <= nLastLightCount; i++)
		{
			glDisable(GL_LIGHT2 + i);
		}
	}
	
	// Remember the number of lights used this 
	// cycle that need to be shut off next cycle
	nLastLightCount = nLightCount;


	//
	//	Position and draw actors
	// 
	for (i = 0; i < MAX_ACTORS; i++) {

		// If an actor is to be displayed, it must
		// be active
		if (actors[i].active) 
		{

			// Compute fLifePctRemaining
			fRemainingLifePct = 1.0f - actors[i].LifePercent;
			
			// Reset view
			glLoadIdentity();

			// Move actor
			glTranslatef(actors[i].x, actors[i].y, actors[i].z);
			glColor4ub(actors[i].color.r, actors[i].color.g, actors[i].color.b, 255);
			
			switch (actors[i].type) {
			
				// Two shots are drawn overlaying each other 
				// for twinkling effect
				case ACTOR_SHOT:
				case ACTOR_SFIRE:
					
					// Setup display characteristics for shots
					if (nLastType != actors[i].type) {
						
						// Setup rendering features
						renderset(render_blend, g_bGlBlendingEnabled);
						renderset(render_texture, g_bGlTexturesEnabled);
						renderenable(render_depthtest);		
						renderdisable(render_lighting);
					}

					glClearDepth(1.0f);
					
					if (actors[i].type == ACTOR_SHOT)
					{
						glColor4ub(255,255,255,254);
					}
					else
					{
						glScalef(0.7f, 0.7f, 0.7f);
						glColor4ub(255,0,0,254);
					}

					glRotatef(actors[i].rx, 0.0f, 0.0f, 1.0f);
					glCallList(actors[i].displaylistid);

					glClearDepth(1.0f);

					// Draw friendly shots blue, enemy shots red
					if (actors[i].type == ACTOR_SHOT)
						glColor4ub(0,0,255,254);
					else 
						glColor4ub(255,255,0,128);

					glTranslatef(0.0f, 0.0f, 0.1f);
					glRotatef(actors[i].rx * -2.0f, 0.0f, 0.0f, 1.0f);
					glCallList(actors[i].displaylistid);

					break;

				case ACTOR_BLAST:
					
					// Setup display characteristics
					if (nLastType != actors[i].type) {
						
						// Setup rendering features
						renderset(render_blend, g_bGlBlendingEnabled);
						renderset(render_texture, g_bGlTexturesEnabled);
						renderdisable(render_depthtest | render_lighting); 

					}

					
					// Draw the blast twice for effect
					fScalar = (float)(actors[i].size + 2.0f * actors[i].LifePercent);
					glScalef(fScalar, fScalar, fScalar);
					glColor4ub(actors[i].color.r, actors[i].color.g, actors[i].color.b, (GLuint)(fRemainingLifePct * 235.0f) + 20);
					glRotatef(actors[i].rx, 1.0f, 0.0f, 0.0f);
					glCallList(actors[i].displaylistid);
					
					glScalef(0.75f, 0.75f, 0.75f);
					glColor4ub(	(GLuint)(fRemainingLifePct * 128.0f) + 64, (GLuint)(fRemainingLifePct * 64.0f), 0, (GLuint)((fRemainingLifePct * 235.0f) + 20));
					glCallList(actors[i].displaylistid);
					glScalef(1.0f,1.0f,1.0f);

					break;

				case ACTOR_FLAMES:
					
					// Setup rendering options
					if (nLastType != actors[i].type) {

						// Setup rendering features
						renderset(render_blend,		g_bGlBlendingEnabled);
						renderset(render_texture,	g_bGlTexturesEnabled);
						renderenable(render_depthtest); 
						renderdisable(render_lighting);

						// Clear depth
						glLoadIdentity();
						glClearDepth(1.0f);

						// Bind texture
						glBindTexture(GL_TEXTURE_2D, texture[4]);

					}

					// Fade based on age
					glColor4ub(255, (GLint)(fRemainingLifePct * 200), 0, 128);

					fx = actors[i].x;
					fy = actors[i].y;
					fz = actors[i].z;
					fs = actors[i].size * 0.5f;

					glBegin(GL_TRIANGLE_STRIP);						
						glTexCoord2d(1,1); glVertex3f(fx + fs, fy + fs, fz); 
						glTexCoord2d(0,1); glVertex3f(fx - fs, fy + fs, fz); 
						glTexCoord2d(1,0); glVertex3f(fx + fs, fy - fs, fz); 
						glTexCoord2d(0,0); glVertex3f(fx - fs, fy - fs, fz); 
					glEnd();					

					break;

				case ACTOR_PARTICLE:
				case ACTOR_FLAMINGPARTICLE:

					// Setup rendering options
					if (nLastType != actors[i].type) {

						// Setup rendering features
						renderset(render_blend,		g_bGlBlendingEnabled);
						renderset(render_texture,	g_bGlTexturesEnabled);
						renderenable(render_depthtest); 
						renderdisable(render_lighting);

					}

					// Clear depth
					glClearDepth(1.0f);
					
					// Make actor's size at least one...
					if (actors[i].size <= 0)
					{
						actors[i].size = 1;
					}

					fScalar = (float)((actors[i].size + 1) * actors[i].LifePercent) + actors[i].size;
					glScalef(fScalar, fScalar, fScalar);
					
					// Rotate the actor (always faces the camera)
					glRotatef(actors[i].rz, 0.0f, 0.0f, 1.0f);
					
					// Fade based on age
					glColor4ub(actors[i].color.r, 
								actors[i].color.g, 
								actors[i].color.b, 
								(GLuint)((fRemainingLifePct) * 128.0f) + 64);

					// Display object
					glCallList(actors[i].displaylistid);

					// If flaming...
					if (actors[i].type == ACTOR_FLAMINGPARTICLE)
					{
						glScalef(0.3f, 0.3f, 0.3f);
						glRotatef(-actors[i].rz, 0.0f, 0.0f, 1.0f);
						glColor4ub(255,255,0,64);
						glCallList(actors[i].displaylistid);
						glScalef(1.0f, 1.0f, 1.0f);
					}
					
					break;

				case ACTOR_SHIELDS:
					
					// Setup display characteristics for
					if (nLastType != actors[i].type) {
						
						// Setup rendering features
						renderset(render_blend,		g_bGlBlendingEnabled);
						renderset(render_texture,	g_bGlTexturesEnabled);
						renderenable(render_depthtest); 
						renderdisable(render_lighting);
					}

					glClearDepth(1.0f);
					glColor4ub(actors[i].color.r, actors[i].color.g, actors[i].color.b, 254);
					glRotatef(actors[i].rx, 0.0f, 0.0f, 1.0f);
					glRotatef(actors[i].rz, 1.0f, 0.0f, 0.0f);
					glCallList(actors[i].displaylistid);

					glClearDepth(1.0f);
					glTranslatef(0.0f, 0.0f, 0.1f);
					glRotatef(actors[i].rx * -2.0f, 0.0f, 0.0f, 1.0f);
					glRotatef(actors[i].rz * -2.0f, 1.0f, 0.0f, 0.0f);
					glCallList(actors[i].displaylistid);

					glClearDepth(1.0f);
					glColor4ub(actors[i].color.r, 0, 0, 128);
					glRotatef(actors[i].rx * 1.5f, 0.0f, 0.0f, 1.0f);
					glRotatef(actors[i].rz * 1.5f, 1.0f, 0.0f, 0.0f);
					glCallList(actors[i].displaylistid);
					
					break;
			
				case ACTOR_DEBRIS:
					
					if (nLastType != actors[i].type) 
					{

						// Setup rendering features
						renderset(render_lighting,	g_bGlLightingEnabled);
						renderset(render_blend,		g_bGlBlendingEnabled);
						renderdisable(render_texture);
						renderenable(render_depthtest); 
					}
					
					if (actors[i].size) 
					{
						glScalef((float)(actors[i].size + 1), (float)(actors[i].size + 1), (float)(actors[i].size + 1));
					}
					
					glRotatef(actors[i].ry, 0.0f, 1.0f, 0.0f);
					glRotatef(actors[i].rx, 1.0f, 0.0f, 0.0f);
					glRotatef(actors[i].rz, 0.0f, 0.0f, 1.0f);
					
					glColor4ub(actors[i].color.r, actors[i].color.g, actors[i].color.b, (GLuint)(128 * fRemainingLifePct) + 128);
					
					glCallList(actors[i].displaylistid);

					glScalef(1.0f, 1.0f, 1.0f);
					
					break;
				
				case ACTOR_LSAUCER:
					
					// Setup display characteristics for shots
					if (nLastType != actors[i].type) {
						
						// Setup rendering features
						renderset(render_lighting,	g_bGlLightingEnabled);
						renderset(render_texture,	g_bGlTexturesEnabled);
						renderenable(render_depthtest); 
						renderdisable(render_blend);

					}

					glScalef(2.0f, 2.0f, 2.0f);
					glColor4ub(actors[i].color.r, actors[i].color.g, actors[i].color.b, 255);
					
					// Make the saucer wobble ;)
					glRotatef(-90.0f + (float)(sin(actors[i].x) * 15), 1.0f, 0.0f, 0.0f);

					glRotatef(actors[i].rz, 0.0f, 0.0f, 1.0f);

					glCallList(actors[i].displaylistid);

					glScalef(1.0f, 1.0f, 1.0f);

					break;

				default:
					
					if (nLastType != actors[i].type) {
						
						// Setup rendering features
						renderset(render_lighting,	g_bGlLightingEnabled);
						renderset(render_texture,	g_bGlTexturesEnabled);
						renderdisable(render_blend);
						renderenable(render_depthtest); 
					}

					if (actors[i].size)
						glScalef((float)(actors[i].size + 1), (float)(actors[i].size + 1), (float)(actors[i].size + 1));

					glRotatef(actors[i].ry, 0.0f, 1.0f, 0.0f);
					glRotatef(actors[i].rx, 1.0f, 0.0f, 0.0f);
					glRotatef(actors[i].rz, 0.0f, 0.0f, 1.0f);
					glCallList(actors[i].displaylistid);
					break;
			}
			
			// Store the current actor's type
			nLastType = actors[i].type;
		}
	}

	// Draw bounding boxes
	if (g_bDrawBoundingBoxes) 
	{
		// Setup rendering features
		renderenable(render_blend);
		renderdisable(render_lighting | render_depthtest | render_texture);

		// Display bounding boxes translucent white
		glColor4ub(255, 255, 255, 60);
		
		// Draw the bounding box of each actor
		for (i = 0; i < MAX_ACTORS; i++) {
			if (actors[i].active)
			{
				switch (actors[i].type)
				{
					// All of these actors have bounding boxes...
					case ACTOR_PLAYER:
					case ACTOR_ROCK:
					case ACTOR_SHOT:
					case ACTOR_SHIELDS:
					case ACTOR_LSAUCER:
					case ACTOR_SFIRE:
							glLoadIdentity();
							glTranslatef(actors[i].x, actors[i].y, actors[i].z);
							glBegin(GL_QUADS);
							glVertex3f(-actors[i].xcolldist, -actors[i].ycolldist,  0.0f);
							glVertex3f( actors[i].xcolldist, -actors[i].ycolldist,  0.0f);
							glVertex3f( actors[i].xcolldist,  actors[i].ycolldist,  0.0f);
							glVertex3f(-actors[i].xcolldist,  actors[i].ycolldist,  0.0f);
							glEnd();
				}
			}
		}
	}


	// Position The Text On The Screen
	glLoadIdentity();							// Reset The View
	glTranslatef(0.0f,0.0f,-20.0f);		// Move One Unit Into The Screen

	// Printing
	glColor4ub(255,255,255,255);
	
	// Display score
	glPrint(0, -16.0f, 9.5f, "PLAYER 1");
	glPrint(0, -16.0f, 8.5f,"%d", player->Score);
	glPrint(0, -16.0f, 7.5f, "LEVEL %d", g_nLevelNum);

	// Display FPS
	if (g_bShowFps)
		glPrint(0,-16.0f,-12.0f, "FPS %5d   Game Length %5.1f", g_nFps, g_fGameLength);

	// Show message (if one exists)
	if ((g_lMessageEndTime != 0) && (g_lMessageEndTime >= g_fNow)) 
		glPrintCentered(0, -11.0f, "%s", g_sMessage);

	// Print game over message on screen
	if (g_bGameOver) {

		if (g_bShowHelp)
		{
			glPrintCentered(0, 8, "HELP");
			
			glPrintCentered(0, 6.0, "PLAYER CONTROLS");
			glPrintCentered(0, 4.5, "   UP KEY - THRUST    ");
			glPrintCentered(0, 3.5, " DOWN KEY - SHIELDS   ");
			glPrintCentered(0, 2.5, " LEFT KEY - TURN LEFT ");
			glPrintCentered(0, 1.5, "RIGHT KEY - TURN RIGHT");
			glPrintCentered(0, 0.5, "    SPACE - FIRE      ");

			glPrintCentered(0,  -1.5, "GAME OPTION SELECTION");
			glPrintCentered(0,  -3, "R - FRAME RATE      W - WIREFRAME MODE");
			glPrintCentered(0,  -4, "B - BLENDING        U - LEVEL SKIP    ");
			glPrintCentered(0,  -5, "T - TEXTURES        X - BOUNDING BOXES");
			glPrintCentered(0,  -6, "F - FORCE FEEDBACK  V - VERSION INFO  ");
			glPrintCentered(0,  -7, "J - JOYSTICK        D - DIFFICULTY    ");
			glPrintCentered(0,  -8, "L - LIGHTING        H - HELP          ");
			glPrintCentered(0,  -9, "M - SPACE MODE                        ");
			glPrintCentered(0, -10, "S - SOUND           ESC - EXIT        ");
		}
		else
		{
							
			// Display highscore in the center of the top of the screen
			glPrintCentered(0, 9.5f, "%3s %7d", highscores[0].Initials, highscores[0].Score);

			// If the user's entering their initials, don't rotate
			// through screens
			if ((g_bEnteringHighScoreInitials) || ((long)(g_fNow / 4) % 3 == 0)) 
			{
				for (i = 0; i < MAX_HIGHSCORES; i++)
				{
					// If the user is entering their initials and 
					// playerhighscore's address is the same as the 
					// address of highscores[] at the current array
					// index, then draw the cursor
					if ((g_bEnteringHighScoreInitials) && (playerhighscore == &highscores[i])) 
					{
						glLoadIdentity();
						glColor3f(0.3f,0.0f,0.6f);
						glTranslatef(-4.5f + (g_nHighScoreInitialsIdx * 0.6f), 4.5f - i, -30.0f);
						glScalef(2.4f, 2.4f, 1.0f);
						renderenable(render_blend);
						renderdisable(render_lighting | render_depthtest | render_texture);
						glCallList(debris_list);
						glColor3f(1.0f,1.0f,1.0f);
					}
					glPrint(0, -5.0f, 4.0f - i, "%3s %2d %7d", highscores[i].Initials, highscores[i].Level, highscores[i].Score);
				}


				// Display message
				if (g_bEnteringHighScoreInitials)
				{
					glPrintCentered(0,  7.0f, "CONGRATULATIONS!");
					glPrintCentered(0,  6.0f, "YOU GOT A HIGH SCORE!");
					glPrintCentered(0, -8.0f, "TYPE IN YOUR INITIALS AND HIT ENTER WHEN DONE");
				}

			}
			else		
			{
				// Draw text
				glPrintCentered(0, -1.5f, "GAME OVER");
				glPrintCentered(0, -2.5f, "PRESS '1' TO START");
				
				glColor4ub(255, 255, 255, 160);
				glPrintCentered(0, - 7.25f, "PROGRAMMING / MODELS / TEXTURES");
				glPrintCentered(0, - 8.00f, "THOM WETZEL (www.LMNOpc.com)");
				
				// glPrintCentered(0, -10.25f, "OPENGL BASE CODE AND CODING ASSIST");
				// glPrintCentered(0, -11.00f, "JEFF MOLOFEE (nehe.gamedev.net)");
				glPrintCentered(0, - 9.25f, "HUGE THANKS TO JEFF MOLOFEE (nehe.gamedev.net)");
				glPrintCentered(0, -10.00f, "FOR OPENGL BASE CODE AND CODING ASSISTANCE!");


				glPrintCentered(0, -12.25f, "(C)2000 LMNOPC - THIS SOFTWARE MAY NOT BE SOLD");

				// Draw logo
				renderenable(render_blend | render_texture);
				renderdisable(render_lighting | render_depthtest);

				glLoadIdentity();
				glTranslatef(0.0f, 0.45f, -4.5f);
				glColor4ub(255, 255, 255, 160 + (char)(20.0f * sin(g_fNow / 0.250f)));
				glCallList(logo_list);
			}
		}
	}
	else // if not game over
	{
		// Display highscore in the center of the top of the screen
		// If the current player's score is higher than the current
		// highscore, then show the player's score
		if (player->Score <= highscores[0].Score) 
			glPrintCentered(0, 9.5f, "%3s %7d", highscores[0].Initials, highscores[0].Score);
		else
			glPrintCentered(0, 9.5f, "??? %7d", player->Score);
	}

	// Print game over message
	if (g_bGamePaused)
		glPrintCentered(0, 0, "GAME PAUSED -- PRESS P TO CONTINUE");
	
	// Print invincibility message
	if (player->InvincibleEndTime >= g_fNow)
		glPrintCentered(0, 3, "INVINCIBLE FOR %d", int((player->InvincibleEndTime - g_fNow)) + 1);


	return TRUE;										
}


// NeHe
GLvoid KillGLWindow(GLvoid)							// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);				// If So Switch Back To The Desktop
		ShowCursor(TRUE);									// Show Mouse Pointer
	}

	if (hRC)													// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))				// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))					// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;											// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;											// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))				// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;											// Set hWnd To NULL
	}
	
	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}


/*
 *	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height		- Height Of The GL Window Or Fullscreen Mode				*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	
*/
BOOL CreateGLWindow(char* title, int width, int height, int bits, BOOL fullscreenflag)
{
	GLuint	PixelFormat;				// Holds The Results After Searching For A Match
	WNDCLASS	wc;							// Windows Class Structure
	DWORD		dwExStyle;					// Window Extended Style
	DWORD		dwStyle;						// Window Style
	RECT		WindowRect;					// Grabs Rectangle Upper Left / Lower Right Values

	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;	// Set Bottom Value To Requested Height

	fullscreen = fullscreenflag;		// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);			// Grab An Instance For Our Window
	wc.style				= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;										// No Extra Window Data
	wc.cbWndExtra		= 0;										// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon				= LoadIcon(NULL, IDI_WINLOGO);	// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);	// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName	= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))	{								// Attempt To Register The Window Class
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;												// Return FALSE
	}
	
	if (fullscreen)	{											// Attempt Fullscreen Mode?
		
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;			// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;				// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)	{

			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",APP_TITLE, MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			else	{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)	{											// Are We Still In Fullscreen Mode?
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;											// Windows style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	// Create The Window
	if (!(hWnd = CreateWindowEx(	dwExStyle,		// Extended Style For The Window
											"OpenGL",		// Class Name
											title,			// Window Title
											dwStyle |		// Window Style
												WS_CLIPSIBLINGS |		// Required Window Style
												WS_CLIPCHILDREN,		// Required Window Style
											0, 0,				// Window Position
											WindowRect.right - WindowRect.left, // Calculate Window Width
											WindowRect.bottom - WindowRect.top,	// Calculate Window Height
											NULL,				// No Parent Window
											NULL,				// No Menu
											hInstance,		// Instance
											NULL)))			// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();									// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;										// Return FALSE
	}

	// Rewritten for C
	pfd.nSize				= sizeof(PIXELFORMATDESCRIPTOR);
   pfd.nVersion			= 1;							// Version number
   pfd.dwFlags				= PFD_DRAW_TO_WINDOW |	// Format Must Support Window
									PFD_SUPPORT_OPENGL |	// Format Must Support OpenGL
									PFD_DOUBLEBUFFER,		// Must Support Double Buffering
   pfd.iPixelType			= PFD_TYPE_RGBA;			// Request an RGBA format
   pfd.cColorBits			= bits;						// Select our color depth
   pfd.cRedBits			= 0;
   pfd.cRedShift			= 0;
   pfd.cGreenBits			= 0;
	pfd.cGreenShift		= 0;
   pfd.cBlueBits			= 0;
   pfd.cBlueShift			= 0;
   pfd.cAlphaBits			= 0;
   pfd.cAlphaShift		= 0;
   pfd.cAccumBits			= 0;
   pfd.cAccumRedBits		= 0;
   pfd.cAccumGreenBits	= 0;
   pfd.cAccumBlueBits	= 0;
   pfd.cAccumAlphaBits	= 0;
   pfd.cDepthBits			= 16;
   pfd.cStencilBits		= 0;
   pfd.cAuxBuffers		= 0;
   pfd.iLayerType			= PFD_MAIN_PLANE;
   pfd.bReserved			= 0;
   pfd.dwLayerMask		= 0;
   pfd.dwVisibleMask		= 0;
   pfd.dwDamageMask		= 0;
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}


	// Read back pfd_new to see if the hardware 
	// supports OpenGL acceleration
	PIXELFORMATDESCRIPTOR pfd_new;
	DescribePixelFormat (hDC, PixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd_new);
	int generic_format = pfd_new.dwFlags & PFD_GENERIC_FORMAT;
	int generic_accelerated = pfd_new.dwFlags & PFD_GENERIC_ACCELERATED;
	if (generic_format && ! generic_accelerated)
	{
		// software
		g_bGLAccelerated = FALSE;
	}
	else if (generic_format && generic_accelerated)
	{
		// hardware - MCD
		g_bGLAccelerated = TRUE;
	}
	else if (! generic_format && ! generic_accelerated)
	{
		// hardware - ICD
		g_bGLAccelerated = TRUE;
	}

	
	// Set pixel format
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())										// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}
	
	return TRUE;										// Success
}




// NeHe
LRESULT CALLBACK WndProc(	HWND	hWindow,		// Handle For This Window
							UINT	uMsg,					// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{

			if (!HIWORD(wParam))					// Check Minimization State
				g_bActive = TRUE;					// Program is active
			else
				g_bActive = FALSE;				// Program Is No Longer Active

			// DIUtil_ReacquireInputDevices();

			return 0L;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:						// Intercept System Commands
		{
			switch (wParam)						// Check System Calls
			{
				case SC_SCREENSAVE:				// Screensaver Trying To Start?
				case SC_MONITORPOWER:			// Monitor Trying To Enter Powersave?
				return 0L;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);					// Send A Quit Message
			return 0L;								// Jump Back
		}

		case WM_SYSKEYUP:
			switch( wParam )
			{
				// Handle ALT+ENTER (fullscreen/window mode)
				case VK_RETURN:
					// Mode switch is allowed only during the game
					if(g_bActive)
					{

						// Pause the game
						PauseGame(TRUE);

						// This will shutdown window/graphics/sound/control
						TerminateGame();
												
						// Toggle fullscreen flag
						fullscreen =! fullscreen;

						// Restart window/graphics/sound/control
						if (!InitializeGame())
							return 0;
		
					}
				break;
			}
			break;

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0L;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;				// If So, Mark It As FALSE
			return 0L;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0L;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWindow, uMsg, wParam, lParam);
}


//--------------------------------------------------------
// TCW
//
// This routine returs the index of the first free actor
// in the specified range
//--------------------------------------------------------
int FindFreeActor(const int& StartIdx, const int& EndIdx) {
	
	int i = StartIdx - 1;
	int nMax = EndIdx;

	// Make sure EndIdx is within bounds
	if (nMax > MAX_ACTORS)
		nMax = MAX_ACTORS;

	// Loop to find available actor
	do 
	{
		
		if (!actors[++i].active) 
			return i;
		
	} while(i < nMax);

	// No free actors were found
	return 0;
};


//-----------------------------------------------------------------------------
// Name: InitializeGameSounds()
// Desc: Ripped from Duel and modified to init my game sounds
//-----------------------------------------------------------------------------
HRESULT InitializeGameSounds()
{
    if (FAILED(DSUtil_InitDirectSound(hWnd)))
        return E_FAIL;

	g_pShipThrustSound		= DSUtil_CreateSound(TEXT("THRUST"),	1);
	g_pLSaucerSound			= DSUtil_CreateSound(TEXT("LSAUCER"),	1);
	g_pBulletFiringSound		= DSUtil_CreateSound(TEXT("FIRE"),		NUM_SHIP_TYPES);
	g_pSmallExplodeSound		= DSUtil_CreateSound(TEXT("EXPLODE3"),	NUM_SHIP_TYPES);
	g_pMediumExplodeSound	= DSUtil_CreateSound(TEXT("EXPLODE2"),	NUM_SHIP_TYPES);
	g_pLargeExplodeSound		= DSUtil_CreateSound(TEXT("EXPLODE1"),	NUM_SHIP_TYPES);
	g_pSaucerFireSound		= DSUtil_CreateSound(TEXT("SFIRE"),		3);
	g_pLifeSound				= DSUtil_CreateSound(TEXT("LIFE"),		2);
	g_pThumpHiSound			= DSUtil_CreateSound(TEXT("THUMPHI"),	1);
	g_pThumpLoSound			= DSUtil_CreateSound(TEXT("THUMPLO"),	1);

	// Play all of the sounds to get them buffered correctly
	DSUtil_PlaySound(g_pShipThrustSound,		0);
	DSUtil_PlaySound(g_pLSaucerSound,			0);
	DSUtil_PlaySound(g_pBulletFiringSound,		0);
	DSUtil_PlaySound(g_pSmallExplodeSound,		0);
	DSUtil_PlaySound(g_pMediumExplodeSound,	0);
	DSUtil_PlaySound(g_pLargeExplodeSound,		0);
	DSUtil_PlaySound(g_pLifeSound,				0);
	DSUtil_PlaySound(g_pThumpHiSound,			0);
	DSUtil_PlaySound(g_pThumpLoSound,			0);

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CleanupGameSounds()
// Desc: Ripped from Duel and modified to cleanup my game sounds
//-----------------------------------------------------------------------------
VOID CleanupGameSounds()
{
   
	DSUtil_DestroySound (g_pBulletFiringSound);
	DSUtil_DestroySound (g_pSmallExplodeSound);
	DSUtil_DestroySound (g_pMediumExplodeSound);
	DSUtil_DestroySound (g_pLargeExplodeSound);
	DSUtil_DestroySound (g_pShipThrustSound);
	DSUtil_DestroySound (g_pThumpHiSound);
	DSUtil_DestroySound (g_pThumpLoSound);
	DSUtil_DestroySound (g_pLSaucerSound);
	DSUtil_DestroySound (g_pSaucerFireSound);
	DSUtil_DestroySound (g_pLifeSound);

	g_pBulletFiringSound = NULL;
	g_pSmallExplodeSound = NULL;
	g_pMediumExplodeSound = NULL;
	g_pLargeExplodeSound = NULL;
	g_pShipThrustSound = NULL;
	g_pThumpHiSound = NULL;
	g_pThumpLoSound = NULL;
	g_pLSaucerSound	= NULL;
	g_pSaucerFireSound = NULL;
	g_pLifeSound = NULL;

	DSUtil_FreeDirectSound();
}


//-----------------------------------------------------------------------------
// Name: DestroyInput()
// Desc: Ripped from FFDonuts and modified for my ff effects
//-----------------------------------------------------------------------------
void DestroyInput()
{
    // Delete effect objects
    DIUtil_DeleteEffect( g_pExplodeFFEffect );
    
    // Unacquire and release input devices
    if( g_pdidJoystick )
    {
        g_pdidJoystick->Unacquire();
        g_pdidJoystick->Release();
    }
    
    // Release DirectInput
    DIUtil_CleanupDirectInput();
}

//-----------------------------------------------------------------------------
// Name: 
// Desc: Ripped from FFDonuts (unmodified)
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumFFJoysticksCallback( LPCDIDEVICEINSTANCE pInst, 
                                       LPVOID pbHasFFDevice )
{

    if( FALSE == IsEqualIID( pInst->guidInstance, GUID_NULL ) )
        *((BOOL*)pbHasFFDevice) = TRUE;

    return DIENUM_CONTINUE;
}

//-----------------------------------------------------------------------------
// Name: HasForceFeedbackJoystick()
// Desc: Ripped from FFDonuts and modified
//-----------------------------------------------------------------------------
BOOL HasForceFeedbackJoystick( HINSTANCE hInst )
{
    LPDIRECTINPUT pDI;
    BOOL    bHasFFDevice = FALSE;
    HRESULT hr;

    // Initialize DirectInput
    if( SUCCEEDED( DirectInputCreate( hInst,
                                      DIRECTINPUT_VERSION, 
                                      &pDI, NULL ) ) )
    {
        // Enumerate all DirectInput devices
        hr = pDI->EnumDevices( DIDEVTYPE_JOYSTICK,
                               EnumFFJoysticksCallback,
                               &bHasFFDevice,
                               DIEDFL_ATTACHEDONLY | DIEDFL_FORCEFEEDBACK );

        pDI->Release();
    }

    if( FAILED(hr) )
        return FALSE;
/*

	Don't display message -- just return FALSE -- FF isn't required

    if( FALSE == bHasFFDevice )
        MessageBox( NULL, "This sample requires a force feedback\n"
                    "input device.\n\nThis sample will now exit.", 
                    "FFDonuts", MB_ICONERROR );
*/
    return bHasFFDevice;
}

//-----------------------------------------------------------------------------
// Name: InitializeInput()
// Desc: Ripped from FFDonuts and modified
//-----------------------------------------------------------------------------
HRESULT InitializeInput( HWND hWnd )
{
    // Initialize DirectInpu
    if( FAILED( DIUtil_Initialize( hWnd ) ) )
    {
        // MessageBox( hWnd, "Can't Initialize DirectInput", APP_TITLE, MB_OK );
        return E_FAIL;
    }

    // Get the list of DirectInput devices
    DIDEVICEINSTANCE* pDevices;
    DWORD             dwNumDevices;
    DIUtil_GetDevices( &pDevices, &dwNumDevices );

    // Search through the list, looking for a joystick
    for( DWORD i=0; i< dwNumDevices; i++ )
    {
        DWORD dwDevType = pDevices[i].dwDevType & 0x000000ff;

        if( dwDevType == DIDEVTYPE_JOYSTICK )
        {
            g_pdidJoystick = DIUtil_CreateDevice( hWnd, &pDevices[i] );
            break;
        }
    }

    // This app does not requires a joystick
    if( g_pdidJoystick == NULL )
    {
        return E_FAIL;
    }

    // If the joystick is force feedback, ask user for options and setup effects
    if( DIUtil_IsForceFeedback( g_pdidJoystick ) )
    {
        // Create force feedback effects
        g_pExplodeFFEffect = DIUtil_CreateEffect();

        g_pExplodeFFEffect->diEffect.dwGain = ( g_dwForceFeedbackGain * 100L );

        DIUtil_SetupPeriodicEffect( g_pExplodeFFEffect, g_pdidJoystick );

		  // Make the stick shake on startup
		  DIUtil_PlayEffect( g_pExplodeFFEffect );

    }

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: GetDeviceInput()
// Desc: Processes data from the input device.  Uses GetDeviceState().
//		 Ripped from FFDonuts and modified
//-----------------------------------------------------------------------------
DWORD GetDeviceInput()
{
    HRESULT     hr;
    DIJOYSTATE  dijs;
    DWORD       dwInput = 0;

    // Poll the joystick to read the current state
    hr = g_pdidJoystick->Poll();

    // Read the device state
    hr = g_pdidJoystick->GetDeviceState( sizeof(DIJOYSTATE), &dijs );
    if( FAILED(hr) )
    {
        if( hr == DIERR_INPUTLOST )
        {
            // Reacquire the device
            g_pdidJoystick->Acquire();
        }
        // We did not read anything, return no motion
        return 0;
    }

    // Process device state
    // x-axis (left)
    if( dijs.lX < 0 )
        dwInput |= JOYSTICK_LEFT;
		
    // x-axis (right)
    if( dijs.lX > 0 )
        dwInput |= JOYSTICK_RIGHT;

    // y-axis (forward)
    if( dijs.lY < 0 )
        dwInput |= JOYSTICK_UP;

    // y-axis (backward)
    if( dijs.lY > 0 )
        dwInput |= JOYSTICK_DOWN;

    // button 0 (fire)
    if( dijs.rgbButtons[0] & 0x80 )
        dwInput |= JOYSTICK_FIRE;

    // button 1 (shield)
    if( dijs.rgbButtons[1] & 0x80 )
        dwInput |= JOYSTICK_SHIELD;

	// Not used...
	//
    // button 2 (stop) - requires a joystick with more than 2 buttons
    if( dijs.rgbButtons[2] & 0x80 )
        dwInput |= JOYSTICK_STOP;

    // return the new device state
    return dwInput;
}


//--------------------------------------------------------
// TCW
// CollisionTest is a centralized collision detection
// that determines if two actors are colliding.  This 
// is being done to eliminate multiple instances of the
// same code 
//
// NOTE:  CollisionTest 
//
//--------------------------------------------------------
inline BOOL CollisionTest(const actorinfo& actor1, const actorinfo& actor2)
{

/* Long hand version
	
	float xdist			= 0.0f;
	float ydist			= 0.0f;
	float xcolldist	= 0.0f;
	float ycolldist	= 0.0f;

	// Calculate difference between actors' origins
	xdist = absFloat(actors[idx1].x - actors[idx2].x);
	ydist = absFloat(actors[idx1].y - actors[idx2].y);

	// Add actors' bounding box values
	xcolldist = actors[idx1].xcolldist + actors[idx2].xcolldist;
	ycolldist = actors[idx1].ycolldist + actors[idx2].ycolldist;
	
	// Objects are colliding if actor distance
	// is less than the sum of their bounding boxes
	if ((xdist <= xcolldist) && (ydist <= ycolldist))
		return TRUE;

	return FALSE;

*/

	// Objects are colliding if actor distance
	// is less than the sum of their bounding boxes
	// return ((fabs(actor1->x - actor2->x) <= actor1->xcolldist + actor2->xcolldist) && (fabs(actor1->y - actor2->y) <= actor1->ycolldist + actor2->ycolldist));
	return ((fabs(actor1.x - actor2.x) <= actor1.xcolldist + actor2.xcolldist) && (fabs(actor1.y - actor2.y) <= actor1.ycolldist + actor2.ycolldist));

}



//--------------------------------------------------------
// TCW
// Sets up and spawns saucer.  Before the saucer is 
// spawned, a check is made to make sure the saucer isn't
// spwning in the middle of a rock.
//--------------------------------------------------------
BOOL CreateSaucer(const long& IDX) 
{
	int i = 0;	

	// Randomly start at left or right edge of screen
	if (randInt(1, 100) < 50)
	{
		actors[IDX].x = WORLD_HALFWIDTH;
		actors[IDX].vx = -5.0f;
	}
	else
	{
		actors[IDX].x = -WORLD_HALFWIDTH;
		actors[IDX].vx = 5.0f;
	}

	// Start anywhere on y axis 
	actors[IDX].y = randFloat(-(float)WORLD_HALFHEIGHT, (float)WORLD_HALFHEIGHT);

	// Setup collision variables
	actors[IDX].xcolldist = 0.6f;
	actors[IDX].ycolldist = 0.25f;

	// If a collision is going to be immediate, 
	// don't spawn ufo
	for (i = IDX_FIRST_ROCK; i <= IDX_LAST_ROCK; i++)
	{
		if (CollisionTest(actors[IDX], actors[i]))
		{
			return FALSE;
		}
	}
	
	actors[IDX].active			= TRUE;
	actors[IDX].type				= ACTOR_LSAUCER;
	actors[IDX].displaylistid	= lsaucer_list;
	
	actors[IDX].z					= WORLD_DEPTH;
	actors[IDX].rx					= -95;
	actors[IDX].ry					= 0;
	actors[IDX].rz					= 0;

	actors[IDX].vy					= 5.0f;
	actors[IDX].vz					= 5.0f;

	actors[IDX].color.r			= (char)128;
	actors[IDX].color.g			= (char)128;
	actors[IDX].color.b			= (char)128;
	actors[IDX].size			= 1;
	actors[IDX].ShotDelay		= 0;

	actors[IDX].ChangeDirectionDelay = g_fNow + randFloat(0.5f, 2.0f);

	if (g_bSoundEnabled)
	{
		DSUtil_PlaySound( g_pLSaucerSound, DSBPLAY_LOOPING );
	}

	return(TRUE);

}

//--------------------------------------------------------
// TCW
// This routine just deactivates the saucer actor and 
// turns off the saucer noise
//--------------------------------------------------------
void DestroySaucer(const long& IDX)
{
	actors[IDX].active = FALSE;

	if (g_bSoundEnabled)
	{
		DSUtil_StopSound(g_pLSaucerSound);
	}
}


//--------------------------------------------------------
// TCW
// This routine spawns the player, but first makes sure that
// the player won't be spawned into the center of an enemy.
//--------------------------------------------------------
BOOL SpawnPlayer(void) 
{

	int i						= 0;
	int group				= 0;
	int lbound				= 0;
	int ubound				= 0;
	static float firsttime = 0;
	BOOL bSpawn			= FALSE;

	const float fDefaultCollDist = 0.4f;
	
	// Set default player properties 
	player->active	= FALSE;
	player->type	= ACTOR_PLAYER;
	player->x		= 0;
	player->y		= 0;
	player->z		= -20;
	player->rx		= 0;
	player->ry		= 0;
	player->rz		= 0;
	player->vx		= 0;
	player->vy		= 0;
	player->vz		= 0;
	player->displaylistid = player_list;
	player->color.r		= (GLuint)255;
	player->color.g		= (GLuint)255;
	player->color.b		= (GLuint)255;
	player->LifeEndTime	= 0;
	player->size		= 0;
	player->ShieldsUp	= FALSE;
	player->ShieldsPower= 255;
		
	// If the player isn't respawned within ???
	// seconds, then spawn player as invincible for
	// five seconds
	if (firsttime == 0) 
	{
		firsttime = g_fNow;
	}
	else
	{

		// 3 Seconds of collisions gets you invincible
		if (firsttime + 3 <= g_fNow)
		{
			// Setup player as active and invincible
			player->InvincibleEndTime = g_fNow + 5.0f;

			// Set flag telling program to spawn player
			bSpawn = TRUE;
		}
	}


	// The bSpawn flag would be set to TRUE if the
	// player couldn't be spawned for 5 seconds and
	// the collision detection isn't necessary because
	// the player's invincible anyway.
	if (!bSpawn)
	{
		
		// Make the x and ycolldist larger so that the 
		// player has a chance to react before being
		// crushed by a rock after spawning
		player->xcolldist	= fDefaultCollDist * 4;
		player->ycolldist	= fDefaultCollDist * 4;

		// If a collision is going to be immediate, 
		// don't spawn player
		for (group=0; group <= 2; group++)
		{
			switch (group)
			{
				case 0:
					lbound = IDX_FIRST_ROCK;
					ubound = IDX_LAST_ROCK;
					break;
				case 1:
					lbound = IDX_LSAUCER;
					ubound = IDX_LSAUCER;
					break;
				case 2:
					lbound = IDX_FIRST_SFIRE;
					ubound = IDX_LAST_SFIRE;
					break;
			}

			for (i = lbound; i < ubound; i++)
			{
				if (actors[i].active) 
				{
					if (CollisionTest(actors[i], *player))
					{
						return FALSE;
					}
				}
			}
		}

		// If the program made it this far, it's safe to 
		// spawn the player
		bSpawn = TRUE;
	}

	// bSpawn indicates that it's safe to spawn the 
	// player.
	if (bSpawn)
	{

		// Set the x and ycolldist to the REAL values
		player->xcolldist	= fDefaultCollDist;
		player->ycolldist	= fDefaultCollDist;

		// If no collisions were detected, then spawn the player
		player->active = TRUE;

		// Reset firsttime for next time player is spawned
		firsttime = 0;

		// Indicate success
		return TRUE;
	
	}
	else
	{
		return FALSE;
	}

}


//--------------------------------------------------------
// TCW
// Create debris -- makes specified number of 
// polys based on actor[] sent into routine
//
// This routine will be replaced by a particle system
//
//--------------------------------------------------------
void CreateDebris(actorinfo *BaseActor, int nAmt) {

	int i = 0;
	int j = 0;
	int nMax = IDX_LAST_DEBRIS - IDX_FIRST_DEBRIS;
	
	if (nAmt > nMax)
	{
		nAmt = nMax;
	}

	// Create debris
	for (i = 0; i < nAmt; i++) 
	{

		j = FindFreeActor(IDX_FIRST_DEBRIS, IDX_LAST_DEBRIS);
		if (j) 
		{
			
			actors[j] = *BaseActor;
			
			actors[j].active = TRUE;

			actors[j].size = 0;

			actors[j].type = ACTOR_DEBRIS;
			actors[j].displaylistid = debris_list;

			actors[j].LifeStartTime = g_fNow;
			actors[j].LifeEndTime = g_fNow + randFloat(0.25f, 1.0f);

			actors[j].rx = randFloat(0, 360);
			actors[j].ry = randFloat(0, 360);
			actors[j].rz = randFloat(0, 360);
			
			actors[j].vx *= randFloat(0.5f, 1.0f);
			actors[j].vy *= randFloat(0.5f, 1.0f);
			actors[j].vz = randFloat(0.1f, 0.4f);

		}
		// If j is unable to find any open actors,
		// don't waste any more time -- actors are booked!
		else
		{
			return;
		}
	}
}

//--------------------------------------------------------
// TCW
// This checks for collisions between a selected *actor
// and the elements in the actors[] array between 
// LBound and UBound.  This is done so that actors whose
// collisions don't matter aren't evaluated.
//
// This routine is also responsible for the effect of 
// a collision, such as object destruction, debris 
// creation, sound playing, etc.
//--------------------------------------------------------
void CollisionDetection(actorinfo *actor, const int& LBound, const int& UBound)
{

	int i = 0;
	int k = 0;

	actorinfo *obj[2] = {NULL, NULL};

	// Make sure the player is active -- 
	// a collision may have already been found
	if (actor->active)
	{

		// Check collisions between rocks and bullets/player
		i = LBound;
		do
		{
			if (actors[i].active) 
			{
				if (CollisionTest(*actor, actors[i]))
				{
					obj[0] = actor;
					obj[1] = &actors[i];
				}
			}
			i++;
		}
		while ((i < UBound) && (obj[0] == NULL));
	}

	if (obj[0] != NULL)
	{
		for (i = 0; i <= 1; i++)
		{
			
			// Update the objects with the correct responses
			switch (obj[i]->type)
			{

			case ACTOR_SHOT:
			case ACTOR_SFIRE:
				obj[i]->active = FALSE;
				break;

			case ACTOR_PLAYER:

				// Shields up -- player okay
				if ((obj[i]->ShieldsUp) && (obj[i]->ShieldsPower > 0)) 
				{
					obj[i]->ShieldsPower -= 25;
				}
				else 
				{
					// Activate force feedback
					if (g_bJoystickEnabled && g_bForceFeedbackEnabled)
					{
						DIUtil_PlayEffect( g_pExplodeFFEffect );
					}

					// Player death sound
					if (g_bSoundEnabled) 
					{
						DSUtil_PlaySound(g_pLargeExplodeSound, 0);
					}
						
					// Player killed
					obj[i]->active = FALSE;

					// Create debris
					obj[i]->vx = randFloat(5.0f, 20.0f);
					obj[i]->vy = randFloat(5.0f, 20.0f);
					CreateDebris(obj[i], 999);
						
					// Create blast
					k = FindFreeActor(IDX_FIRST_BLAST, IDX_LAST_BLAST);
					if (k) 
					{
						actors[k]				= *obj[i];
						actors[k].active		= TRUE;
						actors[k].type			= ACTOR_BLAST;
						actors[k].displaylistid = blast_list;
						actors[k].color.r		= (GLuint)255;
						actors[k].color.g		= (GLuint)255;
						actors[k].color.b		= (GLuint)255;
						actors[k].LifeStartTime = g_fNow;
						actors[k].LifeEndTime	= g_fNow + 1.5f;
						actors[k].size			= 2;
						actors[k].rx			= randFloat(15, 75);
						actors[k].ry			= actors[k].rx;
					}
				}
				break;

			case ACTOR_ROCK:

				// Add points and play sound
				switch ((int)obj[i]->size) {
					
					// Large rock
					case 2:
						player->Score += 20;

						if (g_bSoundEnabled) 
							DSUtil_PlaySound(g_pLargeExplodeSound, 0); //, (float)(obj[i]->x / WORLD_HALFWIDTH));
						
						// Activate force feedback
						if (g_bJoystickEnabled && g_bForceFeedbackEnabled)
							DIUtil_PlayEffect( g_pExplodeFFEffect );
						
						break;
					
					// Medium rock
					case 1:
						
						player->Score += 50;
						
						if (g_bSoundEnabled)
							DSUtil_PlaySound(g_pMediumExplodeSound, 0); //, (float)(obj[i]->x / WORLD_HALFWIDTH));
						
						// Activate force feedback
						if (g_bJoystickEnabled && g_bForceFeedbackEnabled)
							DIUtil_PlayEffect( g_pExplodeFFEffect );
						
						break;

					// Small rock
					case 0:
						
						player->Score += 100;

						if (g_bSoundEnabled)
							DSUtil_PlaySound(g_pSmallExplodeSound, 0); //, (float)(obj[i]->x / WORLD_HALFWIDTH));
						
						// Activate force feedback
						if (g_bJoystickEnabled && g_bForceFeedbackEnabled)
							DIUtil_PlayEffect( g_pExplodeFFEffect );
						
						break;
								
				}
							
				// Create blast
				k = FindFreeActor(IDX_FIRST_BLAST, IDX_LAST_BLAST);
				if (k) {
					actors[k]					= *obj[i];
					actors[k].active			= TRUE;
					actors[k].type				= ACTOR_BLAST;
					actors[k].displaylistid	= blast_list;
					actors[k].color.r			= randInt(200,255);
					actors[k].color.g			= randInt(200,255);
					actors[k].color.b			= randInt(0, 100);
					actors[k].LifeEndTime	= g_fNow + 0.250f;
					actors[k].LifeStartTime	= g_fNow;
				}

				// Decrement the size of the current rock
				obj[i]->size--;

				// Spawn new rock using same settings as old rock
				k = FindFreeActor(IDX_FIRST_ROCK, IDX_LAST_ROCK);

				if (k) 
				{
					
					// Update original rock
					obj[i]->rx *= 2;
					obj[i]->ry *= 2;
					obj[i]->vx *= 2;
					obj[i]->vy *= 2;
					obj[i]->vz *= 2;
					
					obj[i]->xcolldist = 0.3f * (obj[i]->size + 1); // 0.5f * obj[i]->size + 0.2f;
					obj[i]->ycolldist = obj[i]->xcolldist;

					// Set rock[k] to the same values as rock[i]
					actors[k]		= *obj[i];
					
					// Make new rock darker than source rock
					actors[k].color.r = (GLuint)(actors[k].color.r * 0.75);
					actors[k].color.g = (GLuint)(actors[k].color.g * 0.75);
					actors[k].color.b = (GLuint)(actors[k].color.b * 0.75);

					// Set negative angles for rock[k]
					actors[k].rx	= -obj[i]->rx;
					actors[k].ry	= -obj[i]->ry;
					actors[k].vx	= -obj[i]->vx;
					actors[k].vy	= -obj[i]->vy;

					// Try to be intelligent about setting rz
					obj[i]->rz		= obj[i]->rz + 60;
					actors[k].rz	= obj[i]->rz - 60;

				}

				// Create debris
				CreateDebris(obj[i], int(4.0f * (obj[i]->size + 2.0f)));

				break;

				case ACTOR_LSAUCER:

					// Kill actor
					DestroySaucer(IDX_LSAUCER);

					// Activate force feedback
					if (g_bJoystickEnabled && g_bForceFeedbackEnabled)
					{
						DIUtil_PlayEffect( g_pExplodeFFEffect );
					}

					// Increase score and play explosion sound
					player->Score += 500;
					if (g_bSoundEnabled) 
					{
						DSUtil_PlaySound(g_pLargeExplodeSound, 0); //, (float)(obj[i]->x / WORLD_HALFWIDTH));
					}
					
					// Create blast
					k = FindFreeActor(IDX_FIRST_BLAST, IDX_LAST_BLAST);
					if (k) {
						actors[k]					= *obj[i];
						actors[k].active			= TRUE;
						actors[k].type				= ACTOR_BLAST;
						actors[k].displaylistid	= blast_list;
						actors[k].color.r			= randInt(200,255);
						actors[k].color.g			= randInt(200,255);
						actors[k].color.b			= randInt(0, 100);
						actors[k].LifeEndTime	= g_fNow + 0.250f;
						actors[k].LifeStartTime	= g_fNow;
					}

					// Create debris
					CreateDebris(obj[i], 10);

				break;

				default:
					obj[i]->active = FALSE;
			}
		}
	}
	return;
}


// Game initialization 
BOOL InitializeGame(void)
{
	
	// Create Our OpenGL Window
	if (!CreateGLWindow(APP_TITLE,640,480,16,fullscreen))
		return FALSE;									// Quit If Window Was Not Created

	// Can play game without sound. Do not exit
	if(FAILED(InitializeGameSounds())) 
		g_bDirectSoundEnabled = FALSE;
	else 
		g_bDirectSoundEnabled = TRUE;

	// Default sound enabled on if the system
	// is sound capable
	g_bSoundEnabled = g_bDirectSoundEnabled;

	// Attempt to setup DirectInput.  Don't exit if it fails
	if( FAILED(InitializeInput(hWnd)))
		g_bDirectInputEnabled = FALSE;
	else
	{
		g_bDirectInputEnabled = TRUE;

		// Enable joystick by default
		g_bJoystickEnabled = TRUE;
		
		// Attempt to activate force feedback and enable it if it's present
		g_bForceFeedbackCapable = BOOL(HasForceFeedbackJoystick(hInstance) == TRUE);
		g_bForceFeedbackEnabled = g_bForceFeedbackCapable;

	}

	// Setup rendering flags
	renderinit();

	// Return  success
	return TRUE;
}


// Kill GL lists, sounds, controls, window
void TerminateGame(void)
{
	KillFont();									// Kill GL fonts
	KillDisplayLists();						// Kill my 3D objects
	CleanupGameSounds();						// Turn off DirectSound
	DestroyInput();							// Turn off DirectInput
	KillGLWindow();							// Kill The Window
}


//--------------------------------------------------------
//
// TCW
// There's WAY too much code in WinMain.  All of the game 
// logic is contained in here.  Next game this routine
// won't contain anything game-related except a call to 
// a routine named game_main.
//
//--------------------------------------------------------

int WINAPI WinMain(	HINSTANCE	hInstance,		// Instance
							HINSTANCE	hPrevInstance,	// Previous Instance
							LPSTR			lpCmdLine,		// Command Line Parameters
							int			nCmdShow)		// Window Show State
{
	MSG	msg;									// Windows Message Structure
	BOOL	done			= FALSE;				// Bool Variable To Exit Loop

	//
	// TCW variables
	//
	int		i				= 0;		// i,j,k are counter variables
	int		j				= 0;
	int		k				= 0;
	
	float	velocity			= 0.0f;	// Variable to store thruster velocity
	BOOL	bPlayerThrust	= FALSE; // Variable replaces the use of velocity
											// to flag when player is thrusting. 
											// Based on the amount of time between
											// frames, velocity would sometimes be zero.

	// These variables are the replacements for the 
	// above variables
	float fNow				= 0;		// Fractional time in seconds
	float fLastLoopTime	= 0;		// Fractional time when last loop was started
	float fDeltaTime		= 0;		// Change in fractional seconds since last iteration
	
	// Level variables
	BOOL	bAllEnemiesInactive	= TRUE;	// Init TRUE to reload all rocks
	float fLevelStartTime		= 0;		// Stores time on current level
	float	fTimeForNewRocks		= 1;		// Controls time between level clear and new level
	float	fPlayerRespawnTime	= 0;
	float	fThrustDebrisTime		= 0;

	// Enemy variables
	float fTimeForNewLSaucer	= 0;
	
	// Sound flags
	BOOL	bThrustSoundOn		= FALSE;
	BOOL	bPlayThump			= FALSE;	// Toggles between hi and lo thump
	float	fNextThumpTime		= 0;		// Ticks when next thump will occur


	// Key down flags
	BOOL	lastkeys[256];			// Array to use to determine
										// when a key is first pressed

	// FPS variables
	long	lFramesRendered			= 0;
	float	fProgramStartTime			= 0;

	// DirectInput variables
	DWORD	DirectInput					= 0;
	DWORD	LastDirectInput			= 0;

	
	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
		fullscreen=FALSE;


	// Setup graphics, controls and sounds
	if (!InitializeGame())
		return 0;

	// Initialize Sin and Cos lookup tables
	InitLookupTables();

	// New random number seed
	srand( (unsigned)time( NULL ) );

	// Load the highscores
	LoadHighScores("highscores.dat");

	// Initialze stars
	for (i = 0; i < NUM_STARS; i++)
	{
		stars[i].x = randFloat(-STAR_WORLD_HALFWIDTH, STAR_WORLD_HALFWIDTH);
		stars[i].y = randFloat(-STAR_WORLD_HALFHEIGHT, STAR_WORLD_HALFHEIGHT);
		stars[i].z = randFloat(MIN_STAR_Z, MAX_STAR_Z);
		stars[i].size = randFloat(MIN_STAR_SIZE, MAX_STAR_SIZE);
	}

	// Initialize actors
	for (i = 0; i < MAX_ACTORS; i++) {
		actors[i].active			= FALSE;
		actors[i].type				= ACTOR_NONE;
		actors[i].displaylistid = NULL;
		actors[i].x					= 0;
		actors[i].y					= 0;
		actors[i].z					= -20;
		actors[i].rx				= 0;
		actors[i].ry				= 0;
		actors[i].rz				= 0;
		actors[i].vx				= 0;
		actors[i].vy				= 0;
		actors[i].vz				= 0;
		actors[i].color.r			= (char)255;
		actors[i].color.g			= (char)255;
		actors[i].color.b			= (char)255;
		actors[i].LifeStartTime = 0;
		actors[i].LifeEndTime	= 0;
		actors[i].LifePercent	= 0.0f;
		actors[i].size				= 0;
		actors[i].ShieldsUp		= FALSE;
		actors[i].ShieldsPower	= 0;
		actors[i].Lives			= 0;
		actors[i].NextBonus		= 0;
		actors[i].Score			= 0;
		actors[i].ShotDelay		= 0;
		actors[i].ChangeDirectionDelay = 0;
		actors[i].PointValue		= 0;
		actors[i].InvincibleEndTime = 0;
		actors[i].xcolldist		= 0;
		actors[i].ycolldist		= 0;
	}
	
	
	// Create blanks in the actors array for the 
	// player and bullets
	player = &actors[IDX_PLAYER];

	// Setup shields
	actors[IDX_SHIELDS].active = TRUE;
	actors[IDX_SHIELDS].type = ACTOR_SHIELDS;
	actors[IDX_SHIELDS].displaylistid = shields_list;
	actors[IDX_SHIELDS].x		= 0;
	actors[IDX_SHIELDS].y		= 0;
	actors[IDX_SHIELDS].z		= -20;
	actors[IDX_SHIELDS].rx		= 0;
	actors[IDX_SHIELDS].ry		= 0;
	actors[IDX_SHIELDS].rz		= 0;
	actors[IDX_SHIELDS].vx		= 0;
	actors[IDX_SHIELDS].vy		= 0;
	actors[IDX_SHIELDS].vz		= 0;
	actors[IDX_SHIELDS].color.r		= (char)255;
	actors[IDX_SHIELDS].color.g		= (char)255;
	actors[IDX_SHIELDS].color.b		= (char)255;
	actors[IDX_SHIELDS].LifeEndTime		= 0;
	actors[IDX_SHIELDS].size			= 0;
	actors[IDX_SHIELDS].ShieldsUp		= FALSE;
	actors[IDX_SHIELDS].ShieldsPower	= 0;
	actors[IDX_SHIELDS].xcolldist = 0.5f;
	actors[IDX_SHIELDS].ycolldist = 0.5f;

	// Initialize timing
	TimingInit();
	g_fNow = TimingGetTime();
	fProgramStartTime = g_fNow;
	
	// Display version info on startup j
	UpdateMessage(VERSION);

	InitializeGame();
	
	// Main program loop
	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done = TRUE;						// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);			// Translate The Message
				DispatchMessage(&msg);			// Dispatch The Message
			}
		}
		else											// If There Are No Messages
		{
			UpdateGame();
		}
	}

	TerminateGame();


			
	
	//------------------------------------------------------
	// Shutdown
	//------------------------------------------------------

	// Save high score records
	SaveHighScores("highscores.dat");

	// Kill data, sounds, control, window
	TerminateGame();

	// Exit The Program
	return (msg.wParam);

}

