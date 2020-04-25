/*

Disasteroids 3D
Copyright (c) 2000 Thom Wetzel, Jr.

This software is provided 'as-is', without any express 
or implied warranty. In no event will the authors be held 
liable for any damages arising from the use of this software.

*/

#define APP_TITLE "Disasteroids 3D"

// #define VERSION "VERSION 1.1 (2000.06.01)"
// #define VERSION "VERSION 1.2 (2000.10.22)"
#define VERSION "VERSION 1.3 (2000.02.25)"
const float g_VersionNumber = 1.3f;

const char csConfigFilename[] = "Disasteroids3D.cfg";

// DirectSound
#define INITGUID
#include <dsound.h>
#include "dsutil.h"

// DirectInput
#include <cguid.h>			// GUID Header required by DirectInput
#include "diutil.h"			// DirectInput utility header file

#include "game.h"


// Game data
#include "modeldata.h"		// Model data
#include "resource.h"		// Resource file data

// Lookup table code
#include "LookupTables.h"


// Function declarations
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

// Windows variables
HDC		hDC					= NULL;	// Private GDI Device Context
HGLRC		hRC					= NULL;	// Permanent Rendering Context
HWND		hWnd					= NULL;	// Holds Our Window Handle
HINSTANCE	hInstance		= NULL;	// Holds The Instance Of The Application
PIXELFORMATDESCRIPTOR pfd;				// Pixel Format Descriptor variable
BOOL g_bGLAccelerated = FALSE;		// Flag that's TRUE when hardware supports
DEVMODE			DMsaved;					// Saves The Previous Screen Settings ( NEW )
												// OpenGL acceleration

BOOL	g_bActive		= TRUE;			// Window active flag

int	keys[256];					// Array Used For The Keyboard Routine
int	lastkeys[256];				// Array to use to determine when a key is first pressed


// Console variables
cvar_t g_cvFullscreen			= {"Fullscreen",		"2",		TRUE};
cvar_t g_cvSpaceMode				= {"SpaceMode",		"2",		TRUE};
cvar_t g_cvCopernicusMode		= {"CopernicusMode", "0",		TRUE};



// Resolution stuff
//
typedef struct resinfo_t
{
	int width;
	int height;
} resinfo;
const int RESOLUTIONS_COUNT = 5;
resinfo resolutions[RESOLUTIONS_COUNT] = {{ 640, 480 }, { 800, 600 }, { 1024, 768 }, {1280, 1024}, {1600, 1200}};
cvar_t g_cvResolution = {"Resolution", "0", TRUE};


// Color depth stuff
const int COLORDEPTHS_COUNT = 3;
int colordepths[COLORDEPTHS_COUNT] = { 16, 24, 32 };
cvar_t g_cvColorDepth = {"ColorDepth", "0", TRUE};

// Gamma ramp stuff
static WORD old_gamma_ramp[768];
cvar_t g_cvGammaGamma		= {"GammaGamma",			"0.5", TRUE};
cvar_t g_cvGammaBrightness = {"GammaBrightness",	"0.5", TRUE};
cvar_t g_cvGammaContrast	= {"GammaContrast",		"0.5", TRUE};


// Difficulty settings
const int EASY_DIFFICULTY		= 0;
const int NORMAL_DIFFICULTY	= 1;
const int HARD_DIFFICULTY		= 2;
cvar_t g_cvDifficulty			= {"Difficulty",		"1",		TRUE};
int g_nDifficulty					= 0;

// Cheats
BOOL	 g_bCheatsEnabled			= FALSE;
cvar_t g_cvInfiniteLives		= {"InfiniteLives",	"0",		FALSE};
cvar_t g_cvInfiniteShields		= {"InfiniteShields","0",		FALSE};
cvar_t g_cvInvincibility		= {"Invincibility",	"0",		FALSE};

// Storage For Textures
const int NUM_TEXTURES = 14;
GLuint texture[NUM_TEXTURES];

// Font list
GLuint font_list;

// Open GL lists
const int NUM_LISTS = 14;
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
GLuint extraship_list;
GLuint extraship5_list;

cvar_t g_cvLightingEnabled			= {"LightingEnabled",		"-1", FALSE};	// Lighting enabled flag
cvar_t g_cvBlendingEnabled			= {"BlendingEnabled",		"-1", FALSE};	// Blending enabled flag
cvar_t g_cvTexturesEnabled			= {"TexturesEnabled",		"-1", FALSE};	// Textures enabled flag
cvar_t g_cvWireframeEnabled		= {"WireframeEnabled",		"0",	FALSE};	// Wireframe mode flag
cvar_t g_cvBoundingBoxesEnabled	= {"BoundingBoxesEnabled",	"0",	FALSE};	// Bounding box display flag
cvar_t g_cvFpsEnabled				= {"FpsEnabled",				"0",	FALSE};	// Display frames per second flag
cvar_t g_cvVsyncEnabled				= {"VsyncEnabled",			"0",	TRUE};	// Vsync enabled flag
cvar_t g_cvSpaceSize					= {"SpaceSize",				"1",	TRUE};	// Space size value

const float g_cvMaxFps					= 60.0f;

BOOL	g_bExitApp					= FALSE;		// Global loop ending variable

BOOL	g_bStartGame				= FALSE;		// Flag to use to trigger the start of a new game from the menus
BOOL	g_bGameOver					= TRUE;		// Game over flag
BOOL	g_bGamePaused				= FALSE;		// Game paused flag
BOOL	g_bShowMenu					= FALSE;

int	g_nFps						= 0;			// Frames per second variable
float	g_fGameLength				= 0;			// Application run time variable
int	g_nLevelNum					= 0;			// Level number indicator

float g_fSwapBufferInterval	= 0;			// Vsync timing variable

// Default lighting setup for light 1
GLfloat LightAmbient[]	= { 0.05f, 0.05f, 0.05f, 1.0f };
GLfloat LightDiffuse[]	= { 0.3f, 0.3f, 0.7f, 1.0f }; 
GLfloat LightPosition[]	= { 7.0f, 7.0f, 5.0f, 1.0f };

/* V 1.2 

  I changed all these from consts to straight floats so that
  I can now change the space size in the game
*/
float WORLD_WIDTH			=  44.0f;		// Holds The Width Of Our World
float WORLD_HALFWIDTH	=  22.0f;		// Holds Half The Width Of Our World
float WORLD_HEIGHT		=  32.0f;		// Holds The Height Of Our World
float WORLD_HALFHEIGHT	=  16.0f;		// Holds Half The Height Of Our World
float WORLD_DEPTH			= -20.0f;	// The distance all the objects are from the camera

float SCREEN_WIDTH		=  22.0f;
float	SCREEN_HALFWIDTH	=  11.0f;
float SCREEN_HEIGHT		=  16.0f;
float	SCREEN_HALFHEIGHT	=   8.0f;
float SCREEN_DEPTH		= -20.0f;



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
const int IDX_PLAYER1			= 31;
const int IDX_PLAYER2			= 32;
const int IDX_FIRST_DEBRIS		= 33;
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
typedef struct rgbcolor_t {
	GLbyte r;		// Red intensity (0-255)
	GLbyte g;		// Green intensity (0-255)
	GLbyte b;		// Blue intensity (0-255)
} udtRgbColor;


// Actor definition
typedef struct actor_t {

	// All of the following properties are used
	// by all the actors
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
	float		size;				// Visual scaling factor (different size rocks/blasts)

	// These properties are shared by actors that collide 
	//
	long		PointValue;		// Value awarded to player when this actor is destroyed
	float		xcolldist;		// X collision distance (bounding box x radius)
	float		ycolldist;		// Y collision distance (bounding box y radius)


	// These properties are unique to the player(s)
	//
	BOOL		ShieldsUp;
	GLfloat	ShieldsPower;
	long		Lives;
	long		NextBonus;
	long		Score;
	float		ShotDelay;
	float		InvincibleEndTime;
	BOOL		Hyperspacing;
	float		HyperspaceInterval;
	float		HyperspaceDelay;

	// These properties are unique to the saucer(s)
	//
	float		ChangeDirectionDelay;

	// These properties are unique to particle-emitters
	float		ParticleSpawnDelay;	// Time until next particle should be spawned

	
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
cvar_t g_cvSoundEnabled					= {"SoundEnabled", "2", TRUE};
cvar_t g_cvVolume							= {"Volume", "1.0", TRUE};
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
SoundObject* g_pMenuBeepSound			= NULL;
const int NUM_SHIP_TYPES				= 9;
BOOL g_bThrustSoundOn					= FALSE;
const float cStereoSeperation = 0.1f;

// DirectInput stuff (ripped from DirectX SDK FFDonuts and modified)
BOOL g_bDirectInputEnabled							= FALSE;
LPDIRECTINPUTDEVICE2 g_pdidJoystick				= NULL;
FFEffectObject*      g_pExplodeFFEffect		= NULL;
DWORD                g_dwForceFeedbackGain	= 100L;
cvar_t g_cvJoystickEnabled			= {"JoystickEnabled", "-1", FALSE};
cvar_t g_cvForceFeedbackEnabled	= {"ForcefeedbackEnabled", "0", TRUE};
BOOL g_bForceFeedbackCapable	= FALSE;
const int JOYSTICK_LEFT			= 1;
const int JOYSTICK_RIGHT		= 2;
const int JOYSTICK_UP			= 4;
const int JOYSTICK_DOWN			= 8;
const int JOYSTICK_FIRE			= 16;
const int JOYSTICK_SHIELDS		= 32;
const int JOYSTICK_HYPERSPACE	= 64;

// Mouse variables
LPDIRECTINPUTDEVICE	g_pdidMouse	= NULL;
DIMOUSESTATE g_dims;
cvar_t g_cvMouseEnabled				= {"MouseEnabled", "-1", TRUE};
cvar_t g_cvMouseSensitivity		= {"MouseSensitivity", "0.5", TRUE};

// Other control stuff
cvar_t g_cvShipRotateAdjust		= { "ShipRotateAdjust", "0.5", TRUE};


// Variables for dealing with highscore initial entry
// High score variables and initialization
const int MAX_HIGHSCORES = 10;

/*
typedef struct _highscoreinfo {
	char		Initials[4];
	long		Score;
	int		Level;

} highscoreinfo;
*/
typedef struct _highscoreinfo {
	char		Initials[4];
	long		Score;
	int		Level;
	char		Difficulty;		// E, N, H
	char		SpaceSize;		// S, M, L

} highscoreinfo;

// Init the highscores with my initials -- I'm a WINNER!!!! :)
highscoreinfo	highscores[MAX_HIGHSCORES] = {
		"TCW", 1000, 1, 'N', 'M', 
		"TCW", 1000, 1, 'N', 'M', 
		"TCW", 1000, 1, 'N', 'M', 
		"TCW", 1000, 1, 'N', 'M', 
		"TCW", 1000, 1, 'N', 'M', 
		"TCW", 1000, 1, 'N', 'M', 
		"TCW", 1000, 1, 'N', 'M', 
		"TCW", 1000, 1, 'N', 'M', 
		"TCW", 1000, 1, 'N', 'M', 
		"TCW", 1000, 1, 'N', 'M' };
BOOL	g_bEnteringHighScoreInitials		= FALSE;
char	g_nHighScoreInitialsIdx				= 0;
const char g_cLegalHighScoreChars[38]	= " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
highscoreinfo *playerhighscore			= NULL;


// Message display variables
const long MESSAGE_LIFE = 2;
long g_lMessageEndTime	= 0;
char g_sMessage[256]		= "";


// Key binds
const int NUM_KEY_BINDS		= 8;

char g_asKeyBindsText[NUM_KEY_BINDS][40] = 
{
	"THRUST", "SHIELDS", "HYPERSPACE", "ROTATE LEFT", "ROTATE RIGHT", "FIRE", "START GAME", "PAUSE GAME"
};
cvar_t g_cvKeyThrust[2]			= {{"KeyBind_Thrust", "38", TRUE}, {"KeyBind_ThrustAlt", "0", TRUE}};
cvar_t g_cvKeyShields[2]		= {{"KeyBind_Shields", "40", TRUE}, {"KeyBind_ShieldsAlt", "0", TRUE}};
cvar_t g_cvKeyHyperspace[2]	= {{"KeyBind_Hyperspace", "72", TRUE}, {"KeyBind_HyperspaceAlt", "0", TRUE}};
cvar_t g_cvKeyRotateLeft[2]	= {{"KeyBind_RotateLeft", "37", TRUE}, {"KeyBind_RotateLeftAlt", "0", TRUE}};
cvar_t g_cvKeyRotateRight[2]	= {{"KeyBind_RotateRight", "39", TRUE}, {"KeyBind_RotateRightAlt", "0",	TRUE}};
cvar_t g_cvKeyFire[2]			= {{"KeyBind_Fire", "32", TRUE}, {"KeyBind_FireAlt", "0", TRUE}};
cvar_t g_cvKeyStartGame[2]		= {{"KeyBind_StartGame", "49", TRUE}, {"KeyBind_StartGameAlt", "0", TRUE}};
cvar_t g_cvKeyPause[2]			= {{"KeyBind_Pause", "80", TRUE}, {"KeyBind_Pause", "0", TRUE}};
// Array of keybind console variables.  This is used for the menu system.
// In the future, I need to create a keybind type that contains both the 
// description and cvar of each bind, but this will do... for now.
cvar_t *g_acvKeyBinds[NUM_KEY_BINDS * 2] = 
{
	&g_cvKeyThrust[0],		&g_cvKeyThrust[1],
	&g_cvKeyShields[0],		&g_cvKeyShields[1],
	&g_cvKeyHyperspace[0],	&g_cvKeyHyperspace[1],
	&g_cvKeyRotateLeft[0],	&g_cvKeyRotateLeft[1],
	&g_cvKeyRotateRight[0], &g_cvKeyRotateRight[1],
	&g_cvKeyFire[0],			&g_cvKeyFire[1],
	&g_cvKeyStartGame[0],	&g_cvKeyStartGame[1],
	&g_cvKeyPause[0],			&g_cvKeyPause[1]
};


// Pre-Copernicus Mode
float g_fBackgroundX = 0;
float g_fBackgroundY = 0;
BOOL  g_bStarsActive = FALSE;
const int NUM_STARS = 50;
const float STAR_WORLD_WIDTH = SCREEN_WIDTH + 4.0f;
const float STAR_WORLD_HALFWIDTH = STAR_WORLD_WIDTH * 0.5f;
const float STAR_WORLD_HEIGHT = SCREEN_HEIGHT + 4.0f;
const float STAR_WORLD_HALFHEIGHT = STAR_WORLD_HEIGHT * 0.5f;
const float MIN_STAR_Z = SCREEN_DEPTH - 5.0f;
const float MAX_STAR_Z = SCREEN_DEPTH + 5.0f;
const float MIN_STAR_SIZE = 0.05f;
const float MAX_STAR_SIZE = 0.15f;
typedef struct _starinfo {
	GLfloat	x;					// X position of the actor
	GLfloat	y;					// Y position of the actor
	GLfloat	z;					// Z position of the actor
	GLfloat	size;				// Size of star
} starinfo;
starinfo stars[NUM_STARS];


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
			fStartTime = TimerGetTime();
			g_bGamePaused = TRUE;

			// Unacquire mouse
			if (NULL != g_pdidMouse)
				g_pdidMouse->Unacquire();
		}
	}
	else
	{
		if (g_bGamePaused)
		{
			g_fNowOffset += TimerGetTime() - fStartTime;
			g_bGamePaused = FALSE;

			// Reacquire mouse
			if (NULL != g_pdidMouse)
			{
				g_pdidMouse->Unacquire();
				g_pdidMouse->Acquire();
			}

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
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-SCREEN_WIDTH, SCREEN_HEIGHT * 1.4f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-SCREEN_WIDTH, 0.0f                , 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f        , 0.0f                , 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.0f        , SCREEN_HEIGHT * 1.4f, 0.0f);
		glEnd();
		
		// Upper right
		glBindTexture(GL_TEXTURE_2D, texture[8]);
		glBegin(GL_QUADS); 
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f        , SCREEN_HEIGHT * 1.4f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f        , 0.0f                , 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( SCREEN_WIDTH, 0.0f                , 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( SCREEN_WIDTH, SCREEN_HEIGHT * 1.4f, 0.0f);
		glEnd();
		
		// Lower left
		glBindTexture(GL_TEXTURE_2D, texture[9]);
		glBegin(GL_QUADS); 
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-SCREEN_WIDTH, 0.0f                , 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-SCREEN_WIDTH,-SCREEN_HEIGHT * 1.4f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f        ,-SCREEN_HEIGHT * 1.4f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.0f        , 0.0f                , 0.0f);
		glEnd();
		
		// Lower right
		glBindTexture(GL_TEXTURE_2D, texture[10]);
		glBegin(GL_QUADS); 
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f        , 0.0f                , 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f        ,-SCREEN_HEIGHT * 1.4f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( SCREEN_WIDTH,-SCREEN_HEIGHT * 1.4f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( SCREEN_WIDTH, 0.0f                , 0.0f);
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
		glTranslatef(-SCREEN_WIDTH*2,-SCREEN_HEIGHT*2,0.0f);
		glCallList(background_list);
		glTranslatef(+SCREEN_WIDTH*2,0.0f,0.0f);
		glCallList(background_list);
		glTranslatef(+SCREEN_WIDTH*2,0.0f,0.0f);
		glCallList(background_list);
		glTranslatef(-SCREEN_WIDTH*2,0.0f,0.0f);

		glTranslatef(-SCREEN_WIDTH*2,+SCREEN_HEIGHT*2,0.0f);
		glCallList(background_list);
		glTranslatef(+SCREEN_WIDTH*2,0.0f,0.0f);
		glCallList(background_list);
		glTranslatef(+SCREEN_WIDTH*2,0.0f,0.0f);
		glCallList(background_list);
		glTranslatef(-SCREEN_WIDTH*2,0.0f,0.0f);

		glTranslatef(-SCREEN_WIDTH*2,+SCREEN_HEIGHT*2,0.0f);
		glCallList(background_list);
		glTranslatef(+SCREEN_WIDTH*2,0.0f,0.0f);
		glCallList(background_list);
		glTranslatef(+SCREEN_WIDTH*2,0.0f,0.0f);
		glCallList(background_list);
		glTranslatef(-SCREEN_WIDTH*2,0.0f,0.0f);
	glEndList();

// ******************** Modified By NeHe (04/30/00) ********************

	
	// Extra ship
	extraship_list = bigspace_list + 1;
	glNewList(extraship_list, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[13]);
		glBegin(GL_QUADS);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.0f);
			glTexCoord2f(0.5f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.0f);
			glTexCoord2f(0.5f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.0f);
		glEnd();
	glEndList();

	// Five extra ships
	extraship5_list = extraship_list + 1;
	glNewList(extraship5_list, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[13]);
		glBegin(GL_QUADS);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.5f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.0f);
			glTexCoord2f(0.5f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.0f);
		glEnd();
	glEndList();

	return player_list;
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
						IDB_LOGO, IDB_EXTRASHIP};

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
GLvoid glPrintVar(int set, const GLfloat& x, const GLfloat& y, const char *string, ...)
{
	if (set > 1)
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

	// Reset The Projection Matrix
	glPushMatrix();

	// Setup rendering options
	renderenable(render_blend | render_texture);
	renderdisable(render_depthtest | render_lighting | render_wireframe);

	glListBase(font_list - 32 + (128 * set));				// Choose The Font Set (0 or 1)
	glBindTexture(GL_TEXTURE_2D, texture[5]);				// Select Our Font Texture
	glTranslated(x,y,-30);										// Position The Text
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Write The Text To The Screen

	glPopMatrix();

}


GLvoid glPrint(int set, const GLfloat& x, const GLfloat& y, const char *text)
{

	if (text==NULL)
		return;
	
	if (set>1)
		set=1;

	// Reset The Projection Matrix
	glPushMatrix();

	// Setup rendering options
	renderenable(render_blend | render_texture);
	renderdisable(render_depthtest | render_lighting | render_wireframe);

	glListBase(font_list - 32 + (128 * set));				// Choose The Font Set (0 or 1)
	glBindTexture(GL_TEXTURE_2D, texture[5]);				// Select Our Font Texture
	glTranslated(x,y,-30);										// Position The Text
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Write The Text To The Screen

	// Restore the matrix
	glPopMatrix();

}


//
// TCW, but based 99% on code from Jeff
//
GLvoid glPrintCentered(int set, const GLfloat& y, const char *text)
{

	GLfloat x = 0;

	if (text == NULL)
		return;
	
	if (set>1)
		set=1;

	// Calc x
	x = -((GLfloat(strlen(text)) + 0.75f) * 0.3f);

	// Call glPrint
	glPrint(set, x, y, text);

}


//
// TCW, but based 99% on code from Jeff
//
GLvoid glPrintCenteredVar(int set, const GLfloat& y, const char *string, ...)
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
	x = -((GLfloat(strlen(text)) + 0.75f) * 0.3f);

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
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);					// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);					// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);					// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}



BOOL GLSupportsExtension(char extName[])
{

	if (strstr((const char*)glGetString(GL_EXTENSIONS), extName) == NULL)
		return FALSE;

	return TRUE;
}


/* SetVsync

  This allows me to set/unset Vsync as necessary.  Since 
  Disasteroids' rendering isn't throttled, when Vsync is enabled,
  the game stutters in attempts to remain at a constant fps.

*/
/*

	Since very few video cards support this extension at 
	this time, this routine will be simplified to only 
	set the flags to the correct values, and this routine
	may be reinstated at a later time


typedef BOOL (APIENTRY * PFNWGLEXTSWAPINTERVALPROC) (GLint interval);
BOOL SetVsync(const BOOL value)
{
	
	PFNWGLEXTSWAPINTERVALPROC wglSwapIntervalEXT  = NULL;

	wglSwapIntervalEXT = (PFNWGLEXTSWAPINTERVALPROC) wglGetProcAddress("wglSwapIntervalEXT"); 
	if (wglSwapIntervalEXT != NULL)
	{
		if (value)
		{
			wglSwapIntervalEXT(0);
			Cvar_SetString("VsyncEnabled", "0");
			UpdateMessage("VSYNC DISABLED");
		}
		else		
		{
			wglSwapIntervalEXT(1);
			Cvar_SetString("VsyncEnabled", "-1");
			UpdateMessage("VSYNC ENABLED");
		}

		return TRUE;
	}

	UpdateMessage("OpenGL extension wglSwapIntervalEXT not supported");

	return FALSE;
}
*/

/*
	This routine DOES NOT actually play with a video card's vsync
	settings.

	What this routine DOES do is allow a video card with vsync to
	play the game when enabled by retrieving the refresh timing rate
	and setting the vsync enable flag, which tells the main loop
	to run in vsync mode.

  This is useful for those people running cards like the ATI Rage
  and Intel integrated 3d which don't allow a program to disable 
  vsync from the code.

*/

BOOL SetVsync(const BOOL value)
{
	if (value)
	{
		Cvar_SetString("VsyncEnabled", "0");
		// UpdateMessage("VSYNC DISABLED");
	}
	else
	{
		Cvar_SetString("VsyncEnabled", "-1");
		// UpdateMessage("VSYNC ENABLED");

		// Update the vsync timing offsets
		g_fSwapBufferInterval = GetRefreshTimingRate();

	}

	return TRUE;
}




/*
	
	SetGammaRamp(double gamma, double bright, double contrast)
	gamma [0,255] (increased gamma < 127, decreased gamma > 127)
	bright [0,255] (darker < 127, brighter > 127)
	contrast [0,255] (lower < 127, higher > 127)

	Thanks to 'DFrey' for posting this information in the 
	discussion boards on OpenGL.org.

*/

void SetGammaRamp(double gamma, double bright, double contrast)
{

	static WORD gamma_ramp[768];
	double v;
	const double ft = 2.0 / 255.0;
	static bool savedramp = false;

	// save old gamma ramp if this is first time modified	
	// FIXME: should get the gamma ramp anyway to see if it has been changed from what was	last set	
	if (!savedramp)
	{
		GetDeviceGammaRamp(hDC, old_gamma_ramp);
		savedramp = true;
	}
	
	for (int x = 0; x < 256; x++)
	{
		if (255 == gamma)
		{
			v = 0.0;
		}
		else
		{
			v = pow((double)x / 255, gamma / (255 - gamma));
		}
		
		v += ft * bright - 1.0;
		v += (ft * (double)x - 1.0) * (contrast / 255.0 -.5) * (ft * contrast + 1.0);
		v *= 65535;
		if (v<0)
		{
			v=0;
		}
		else if (v>65535)
		{
				v=65535;
		}
				
		gamma_ramp[x] = (WORD)v;
	}

	memcpy(gamma_ramp + 256, gamma_ramp, 512);
	memcpy(gamma_ramp + 512, gamma_ramp, 512);
	
	SetDeviceGammaRamp(hDC, gamma_ramp);
	
}

void SetGamma()
{

	double dblGamma		= double(127 + (+0.5 - g_cvGammaGamma.value)			* 127);
	double dblBrightness	= double(127 + (-0.5 + g_cvGammaBrightness.value)	* 127);
	double dblContrast	= double(127 + (-0.5 + g_cvGammaContrast.value)		* 127);

	// UpdateMessage("Gamma %f %f %f", dblGamma, dblBrightness, dblContrast);

	SetGammaRamp(dblGamma, dblBrightness, dblContrast);
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
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);			// Black Background

	glClearDepth(1.0f);									// Depth Buffer Setup

	renderenable(render_depthtest);					// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	
	// Clear The Screen And The Depth Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	
	// Attempt to use this to fix the lighting...
	glEnable(GL_NORMALIZE); 

	renderenable(render_lighting);					// Enable Lighting
	
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



void LoadCfg()
{
	FILE *File = NULL;

	const int MAX_LINE_LEN = 80;
	char line[MAX_LINE_LEN];

	// Open configuration file
	File = fopen(csConfigFilename, "r");
	
	// Make sure file exists
	if (!File)
		return;

	// Read each line in the file until the end of the file
	while (fgets(line, MAX_LINE_LEN, File) != NULL)
	{
		
		// Each line in the .cfg file should contain two tokens;
		// the variable name and the string value.  Lines with more
		// than two tokens are ignored
		if (Tokenize(line) == 2)
		{
			// Set the cvar's string value
			if (!Cvar_SetString(g_Tokens[0], g_Tokens[1]))
			{
				/*
				// Display MessageBox with error message
				char sErrMsg[80];
				sprintf(sErrMsg, "Unknown Cvar '%s' cannot set value '%s'", g_Tokens[0], g_Tokens[1]);
				MessageBox(NULL, sErrMsg, "UNKNOWN CVAR", 0);
				*/
			}
		}
	}

	// Close File
	fclose(File);

}



// TCW
// Load highscores
BOOL LoadHighScores(char *Filename)				
{
	FILE *File = NULL;

	// Exit this function if Filename is not specified
	if (!Filename)
		return FALSE;
	
	// Open the file named Filename
	File = fopen(Filename,"r");

	// Process the file if it was opened successfully
	if (File)
	{
		fread(highscores, sizeof(highscoreinfo), MAX_HIGHSCORES, File);
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

	if (g_bCheatsEnabled)
		return FALSE;
	
	if (!Filename)
		return FALSE;
	
	File=fopen(Filename,"w");
	if (File)
	{
		fwrite( highscores, sizeof(highscoreinfo), MAX_HIGHSCORES, File);
		fclose(File);
		return TRUE;
	}
	
	return FALSE;
}


/*

	I'm using this routine in place of glClear(GL_COLOR_BUFFER_BIT)
	where possible.  It seems to make a difference especially on 
	ATI cards

*/
GLvoid BlackOutScreen(GLvoid)
{
	// This will allow me to draw transparent black
	// shapes on the screen.  Without this the box 
	// isn't drawn because black is the transparent color
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Overwrite the current display with an untextured black square
	renderdisable(render_lighting | render_depthtest | render_texture | render_blend);
	glColor4ub(0, 0, 0, 0);
	const float fx = 0;
	const float fy = 0;
	const float fz = -4.0f;
	const float fs = 3.0f;
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2d(1,1); glVertex3f(fx + fs, fy + fs, fz);
	glTexCoord2d(0,1); glVertex3f(fx - fs, fy + fs, fz);
	glTexCoord2d(1,0); glVertex3f(fx + fs, fy - fs, fz);
	glTexCoord2d(0,0); glVertex3f(fx - fs, fy - fs, fz);
	glEnd();

	// Restore the blend func for translucency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}


void DrawGLBackground(GLvoid)
{
	int	i			= 0;
	float	fScalar	= 0;
	float	fx			= 0;
	float	fy			= 0;
	float	fz			= 0;
	float	fs			= 0;

	if (g_cvCopernicusMode.value != 0)
	{

		// Pre-Copernicus Mode

		// switch(g_nSpaceInMotion)
		switch((int)g_cvSpaceMode.value)
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

			// The screen needs to be cleared for this to work
			BlackOutScreen();
	
			glPushMatrix();

			renderenable(render_texture | render_blend);
			renderdisable(render_lighting | render_depthtest);
			
			fScalar = float(0.25f * sin(g_fNow));
			glTranslatef(fScalar, fScalar, -30.0f - fScalar);
			glRotatef(fScalar, fScalar, -fScalar, 1.0f);

// ******************** Modified By NeHe (04/30/00) ********************
			if (g_fBackgroundX > SCREEN_WIDTH*2)
			{
				g_fBackgroundX -= SCREEN_WIDTH*2;
			}
			if (g_fBackgroundX < -SCREEN_WIDTH*2)
			{
				g_fBackgroundX += SCREEN_WIDTH*2;
			}

			if (g_fBackgroundY > SCREEN_HEIGHT*2)
			{
				g_fBackgroundY -= SCREEN_HEIGHT*2;
			}
			if (g_fBackgroundY < -SCREEN_HEIGHT*2)
			{
				g_fBackgroundY += SCREEN_HEIGHT*2;
			}

			glTranslatef(g_fBackgroundX, g_fBackgroundY, 0);
			glColor4ub(255, 255, 255, 255);
			glCallList(bigspace_list);

			renderenable(render_blend);

			glTranslatef(0, 0, 5.0f);
			glColor4ub(255, 255, 255, 128);
			glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
			glCallList(bigspace_list);
			fScalar = -0.25f * (float)sin(g_fNow * 0.001f);
// ******************** Modified By NeHe (04/30/00) ********************

			glPopMatrix();

			break;

		// Draw background then stars
		// (The break is missing on purpose)
		case 1:
			// Draw background
			renderenable(render_texture);
			renderdisable(render_blend | render_lighting | render_depthtest);
			glColor4ub(255,255,255,128);
			fScalar = 0.25f * (float)sin(g_fNow);

			glPushMatrix();
				glTranslatef(fScalar, fScalar, -30.0f - fScalar);
				glRotatef(fScalar, fScalar, -fScalar, 1.0f);
				glCallList(background_list);
			glPopMatrix();

		// Draw stars
		case 2:
			

			// If the stars alone are being drawn, the old
			// background needs to be cleared
			if ((int)g_cvSpaceMode.value == 2) 
			{

// Change this next line to #if 0 to enable motion blur code
#if 1
				BlackOutScreen();

/*				
	Motion blur
*/
#else
				// This will allow me to draw transparent black
				// shapes on the screen.  Without this the box 
				// isn't drawn because black is the transparent color
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				// Overwrite the current display with an untextured black square
				renderdisable(render_lighting | render_depthtest | render_texture);
				renderenable(render_blend);
				glColor4ub(0, 0, 0, 25);
				fx = 0;
				fy = 0;
				fz = -4;
				fs = 3;
				glBegin(GL_TRIANGLE_STRIP);
					glTexCoord2d(1,1); glVertex3f(fx + fs, fy + fs, fz);
					glTexCoord2d(0,1); glVertex3f(fx - fs, fy + fs, fz);
					glTexCoord2d(1,0); glVertex3f(fx + fs, fy - fs, fz);
					glTexCoord2d(0,0); glVertex3f(fx - fs, fy - fs, fz);
				glEnd();

				// Restore the blend func for translucency
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
#endif

			}


			// Setup global flag that tells main routine 
			// to recalc stars' positions next cycle
			g_bStarsActive = TRUE;

			// Triangle strips and glVertex3f used to 
			// speed up rendering.  See nehe.gamedev.net
			// lesson 19.
			renderenable(render_texture | render_blend );
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

		// Post-Copernicus Mode - Ship NOT center of game

		switch((int)g_cvSpaceMode.value)
		{
		case 0:
			renderenable(render_texture); 
			renderdisable(render_lighting | render_blend | render_depthtest);
			glColor4ub(255,255,255,255);
			glPushMatrix();
				glTranslatef(0.0f, 0.0f, -35.0f);
				glCallList(background_list);
			glPopMatrix();
			break;
		
		case 1:
			renderenable(render_texture);
			renderdisable(render_blend | render_lighting | render_depthtest);
			glColor4ub(255,255,255,255);
			glPushMatrix();
				fScalar = float(0.25f * sin(g_fNow));
				glTranslatef(fScalar, fScalar, -30.0f - fScalar);
				glRotatef(fScalar, fScalar, -fScalar, 1.0f);
				glCallList(background_list);
			glPopMatrix();
			break;

		case 2:

			// Clear screen
			BlackOutScreen();

			renderdisable(render_blend | render_lighting | render_depthtest);
			renderenable(render_texture);
			glColor4ub(255, 255, 255, 255);
			glPushMatrix();
				glTranslatef(0.0f, 0.0f, -30.0f);
				glRotatef(180.0f * float(cos(g_fBackgroundRot/10.0f)), 0.0f, 0.0f, 1.0f);
				glTranslatef((3.0f*float(cos(g_fBackgroundRot)))+(2.1f*float(sin(g_fBackgroundRot*1.4f))),(2.8f*float(sin(g_fBackgroundRot)))+(1.3f*float(sin(g_fBackgroundRot*1.4f))),0.0f);
				glRotatef(10.0f*float(sin(g_fBackgroundRot*1.2f)),1.0f,0.0f,0.0f);
				glCallList(background_list);
			glPopMatrix();
			break;
		}
	}

	glPopMatrix();
}

GLvoid SetupGLLights(GLvoid)
{

	int		i						= 0;
	int		nLightIdx			= 0;
	int		nLightCount			= 0;
	float		fRemainingLifePct	= 0;
	GLfloat	afLightData[4]		= {0.0f, 0.0f, 0.0f, 0.0f};

	static int nLastLightCount;

	// Position and enable lights for light sources
	nLightCount = 0;
	for (i = 0; i < MAX_ACTORS; i++) 
	{

		if (((actors[i].type == ACTOR_BLAST) || 
			(actors[i].type == ACTOR_SHOT)  ||
			(actors[i].type == ACTOR_SFIRE) ||
			(actors[i].type == ACTOR_PLAYER) ||
			(actors[i].type == ACTOR_LSAUCER)||
			(actors[i].type == ACTOR_SHIELDS)) && 
			(actors[i].active))
		{

			// Increment light index number
			nLightIdx = GL_LIGHT2 + nLightCount;

			// Make sure light index is in bounds
			if (nLightIdx < GL_LIGHT0 + GL_MAX_LIGHTS) 
			{

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
			else
			{
				if (i == IDX_FIRST_SHOT) 
				{
					glPrint(0, 0.0f, 0.0f, "MAX LIGHTS Exceeded");
					glPrintVar(0, 0.0f, 1.0f, "MAX_LIGHTS=%d nLightIdx=%d", GL_MAX_LIGHTS, nLightIdx);
				}
			}
		}
	}

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

}


// TCW
// This is where the magic happens ;)
//
int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{

	int		i						= 0;
	int		j						= 0;
	int		nLastType			= ACTOR_NONE;
	float		fScalar				= 0;
	float		fRemainingLifePct	= 0;
	float		fx						= 0;
	float		fy						= 0;
	float		fz						= 0;
	float		fs						= 0;
	char		c						= 0;

	const int EnableVomitComet = 0;


	// Only clear the depth bit.  The color bit is cleared
	// handled in the draw background routine when the last
	// frame isn't completely overdrawn
	glClear(GL_DEPTH_BUFFER_BIT);

	// Reset view
	glLoadIdentity();


	// Rotate background around player (bleh!)
	if (EnableVomitComet && g_cvCopernicusMode.value)
	{
		glRotatef(-player->rz, 0.0f, 0.0f, 1.0f);
	}


	// Set global flag to indicate that the stars
	// aren't being rendered -- when the stars ARE
	// rendered, the flag will be set then to indicate
	// that they ARE :)
	g_bStarsActive = FALSE;

	// Set wireframe mode 
	renderset(render_wireframe, (g_cvWireframeEnabled.value != 0));
	
	// Draw background 
	if (g_cvTexturesEnabled.value)
		DrawGLBackground();

	// Setup light sources
	if (g_cvLightingEnabled.value)
		SetupGLLights();

	//	Position and draw actors
	for (i = 0; i < MAX_ACTORS; i++) {

		// If an actor is to be displayed, it must
		// be active
		if (actors[i].active) 
		{

			// Compute fLifePctRemaining
			fRemainingLifePct = 1.0f - actors[i].LifePercent;
			
			// Store current matrix
			glPushMatrix();

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
						renderset(render_blend, (g_cvBlendingEnabled.value != 0));
						renderset(render_texture, (g_cvTexturesEnabled.value != 0));
						renderdisable(render_lighting | render_depthtest);
					}

					
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
						renderset(render_blend, (g_cvBlendingEnabled.value != 0)); 
						renderset(render_texture, (g_cvTexturesEnabled.value != 0)); 
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

				// The flames use a different rendering technique
				// from the rest of the objects.  Rather than using
				// display lists, the flames' vertexes are calculated
				// and sent on the fly.  This should be quicker than
				// the other method because glLoadIdentity is only 
				// called once.
				case ACTOR_FLAMES:
					
					// Setup rendering options
					if (nLastType != actors[i].type) {

						// Setup rendering features
						renderset(render_blend,		(g_cvBlendingEnabled.value != 0));
						renderset(render_texture,	(g_cvTexturesEnabled.value != 0));
						renderdisable(render_lighting | render_depthtest);
					
						// Bind texture
						glBindTexture(GL_TEXTURE_2D, texture[4]);

					}

					// Fade based on age
					glColor4ub(255, (GLint)(fRemainingLifePct * 180), 0, 128);

					// I'm cheating here a little by adding .1 to the z
					// value.  This will force the flames to appear on top
					// of the smoke so that the smoke won't fog out the
					// flame effect.
					fx = actors[i].x;
					fy = actors[i].y;
					fz = actors[i].z + 0.1f;
					fs = actors[i].size * 0.75f;

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
						renderset(render_blend,		(g_cvBlendingEnabled.value != 0)); 
						renderset(render_texture,	(g_cvTexturesEnabled.value != 0)); 
						renderdisable(render_lighting | render_depthtest);

					}
					
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
						renderset(render_blend,		(g_cvBlendingEnabled.value != 0));
						renderset(render_texture,	(g_cvTexturesEnabled.value != 0));
						renderenable(render_depthtest); 
						renderdisable(render_lighting);

					}
					glColor4ub(actors[i].color.r, actors[i].color.g, actors[i].color.b, 254);
					glRotatef(actors[i].rx, 0.0f, 0.0f, 1.0f);
					glRotatef(actors[i].rz, 1.0f, 0.0f, 0.0f);
					glCallList(actors[i].displaylistid);

					glTranslatef(0.0f, 0.0f, 0.1f);
					glRotatef(actors[i].rx * -2.0f, 0.0f, 0.0f, 1.0f);
					glRotatef(actors[i].rz * -2.0f, 1.0f, 0.0f, 0.0f);
					glCallList(actors[i].displaylistid);

					break;
			
				case ACTOR_DEBRIS:
					
					if (nLastType != actors[i].type) 
					{

						// Setup rendering features
						renderset(render_lighting,	(g_cvLightingEnabled.value != 0)); 
						renderset(render_blend,		(g_cvBlendingEnabled.value != 0)); 
						renderdisable(render_texture | render_depthtest);
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
						renderset(render_lighting,	(g_cvLightingEnabled.value != 0));
						renderset(render_texture,	(g_cvTexturesEnabled.value != 0));
						renderdisable(render_blend);
						renderenable(render_depthtest);
					}

					glScalef(2.0f, 2.0f, 2.0f);
					glColor4ub(actors[i].color.r, actors[i].color.g, actors[i].color.b, 255);
					
					// Make the saucer wobble ;)
					glRotatef(-90.0f + (float)(sin(actors[i].x) * 15), 1.0f, 0.0f, 0.0f);

					glRotatef(actors[i].rz, 0.0f, 0.0f, 1.0f);

					glCallList(actors[i].displaylistid);

					glScalef(1.0f, 1.0f, 1.0f);

					break;

				case ACTOR_PLAYER:
					
					if (nLastType != actors[i].type) {
						
						// Setup rendering features
						renderset(render_lighting,	(g_cvLightingEnabled.value != 0)); 
						renderset(render_texture,	(g_cvTexturesEnabled.value != 0)); 
						renderdisable(render_blend);
						renderenable(render_depthtest); 
					}

					if (actors[i].size)
						glScalef((float)(actors[i].size + 1), (float)(actors[i].size + 1), (float)(actors[i].size + 1));

					glRotatef(actors[i].rz, 0.0f, 0.0f, 1.0f);
					glRotatef(actors[i].rx, 1.0f, 0.0f, 0.0f);
					glRotatef(actors[i].ry, 0.0f, 1.0f, 0.0f);
					glCallList(actors[i].displaylistid);
					break;


				default:
					
					if (nLastType != actors[i].type) {
						
						// Setup rendering features
						renderset(render_lighting,	(g_cvLightingEnabled.value != 0)); 
						renderset(render_texture,	(g_cvTexturesEnabled.value != 0)); 
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

			// Restore previous matrix
			glPopMatrix();
		}
	}

	// Draw bounding boxes
	if (g_cvBoundingBoxesEnabled.value) 
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
							glPushMatrix();
							glTranslatef(actors[i].x, actors[i].y, actors[i].z);
							glBegin(GL_QUADS);
								glVertex3f(-actors[i].xcolldist, -actors[i].ycolldist,  0.0f);
								glVertex3f( actors[i].xcolldist, -actors[i].ycolldist,  0.0f);
								glVertex3f( actors[i].xcolldist,  actors[i].ycolldist,  0.0f);
								glVertex3f(-actors[i].xcolldist,  actors[i].ycolldist,  0.0f);
							glEnd();
							glPopMatrix();
				}
			}
		}
	}


	// Un-rotate background before drawing the text
	if (EnableVomitComet && g_cvCopernicusMode.value)
		glRotatef(player->rz, 0.0f, 0.0f, 1.0f);

	// Don't display shields when the game's over
	if (!g_bGameOver)
	{
	
		// Write word 'Shields' -- duh.
		glColor4ub(255,255,255,255);
		glPrint(0, -16.0f, -10.25f, "SHIELDS");

		// Display shields
		for (i = 0; i <= 9; i++)
		{
			
			fScalar = float(i + 1) * 0.1f;
			glColor4f(1.0f - fScalar, fScalar, 0, 1.0f);

			if ((float)player->ShieldsPower / 255.0f < (float)i * 0.1f)
			{
				c = (char)134;
			}
			else
			{
				c = (char)135;
			}

			glPrintVar(0, -16.0f + (i * 0.6f), -11.0f, "%c", c);
		}
	}

	// Set text color 
	glColor4ub(255,255,255,255);
	
	// Display score
	glPrint(0, -16.0f, 9.5f, "PLAYER 1");
	glPrintVar(0, -16.0f, 8.5f,"%d", player->Score);
	glPrintVar(0, -16.0f, 7.5f, "LEVEL %d", g_nLevelNum);

/*
	// Display FPS
	if (g_cvFpsEnabled.value != 0)
		glPrintVar(0,-16.0f,-12.0f, "FPS %5d IS WRONG -- Game Length %5.1f", g_nFps, g_fGameLength);
*/

	// Show message (if one exists)
	if ((g_lMessageEndTime != 0) && (g_lMessageEndTime >= g_fNow)) 
		glPrintCenteredVar(0, -11.0f, "%s", g_sMessage);

	// Print game over message on screen
	if (g_bGameOver) {

		if (!g_bShowMenu)
		{
							
			// Display highscore in the center of the top of the screen
			glPrintCenteredVar(0, 9.5f, "%3s %7d", highscores[0].Initials, highscores[0].Score);

			// If the user's entering their initials, don't rotate
			// through screens
			if ((g_bEnteringHighScoreInitials) || ((long)(g_fNow* 0.25f) % 3 == 0)) 
			{
				for (i = 0; i < MAX_HIGHSCORES; i++)
				{
					// If the user is entering their initials and 
					// playerhighscore's address is the same as the 
					// address of highscores[] at the current array
					// index, then draw the cursor
					if ((g_bEnteringHighScoreInitials) && (playerhighscore == &highscores[i])) 
					{
						// Store current matrix
						glPushMatrix();

						// Draw the cursor
						glColor3f(0.3f,0.0f,0.6f);
						glTranslatef(-6.4f + (g_nHighScoreInitialsIdx * 0.6f), 4.5f - i, -30.0f);
						glScalef(2.4f, 2.4f, 1.0f);
						renderenable(render_blend);
						renderdisable(render_lighting | render_depthtest | render_texture);
						glCallList(debris_list);
						glColor3f(1.0f,1.0f,1.0f);

						// Restore matrix
						glPopMatrix();
					}
					glPrintVar(0, -7.0f, 4.0f - i, "%3s %2d %7d   %c / %c", highscores[i].Initials, highscores[i].Level, highscores[i].Score, highscores[i].Difficulty, highscores[i].SpaceSize);
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
				
				glPrintCentered(0, - 9.25f, "HUGE THANKS TO JEFF MOLOFEE (nehe.gamedev.net)");
				glPrintCentered(0, -10.00f, "FOR OPENGL BASE CODE AND CODING ASSISTANCE!");


				glPrintCentered(0, -12.25f, "(C)2000 LMNOPC - THIS SOFTWARE MAY NOT BE SOLD");

				// Draw logo
				renderenable(render_blend | render_texture);
				renderdisable(render_lighting | render_depthtest);
				glPushMatrix();
				glTranslatef(0.0f, 0.45f, -4.5f);
				// glColor4ub(255, 255, 255, 160 + (char)(20.0f * sin(g_fNow / 0.250f)));
				glColor4ub(255, 255, 255, 255);
				glCallList(logo_list);
				glPopMatrix();
			

			}
		}
	}
	else // if not game over
	{

		// Display highscore in the center of the top of the screen
		// If the current player's score is higher than the current
		// highscore, then show the player's score
		if (player->Score <= highscores[0].Score) 
			glPrintCenteredVar(0, 9.5f, "%3s %7d", highscores[0].Initials, highscores[0].Score);
		else
			glPrintCenteredVar(0, 9.5f, "??? %7d", player->Score);
	}

	// Print game over message
	if (g_bGamePaused && !g_bShowMenu)
		glPrintCenteredVar(0, 0, "GAME PAUSED -- PRESS '%c' TO CONTINUE", (int)g_cvKeyPause[0].value);
	
	// Print invincibility message
	if (player->InvincibleEndTime >= g_fNow)
		glPrintCenteredVar(0, 3, "INVINCIBLE FOR %d", int((player->InvincibleEndTime - g_fNow)) + 1);


	// Display extra men
	glColor4ub(235,235,255,255);
	renderenable(render_texture | render_blend);
	renderdisable(render_depthtest | render_lighting);
	i = player->Lives;
	glPushMatrix();
		glTranslatef(-9.0f, 7.25f,-25.0f);
		while (i > 6)
		{
			glTranslatef(1.0f, 0, 0);
			glCallList(extraship5_list);
			i -= 5;
		}
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-9.0f, 8.25f,-25.0f);
		while (i > 1)
		{
			glTranslatef(1.0f, 0, 0);
			glCallList(extraship_list);
			i -= 1;
		}
	glPopMatrix();
	 

	// Draw menu
	if (g_bShowMenu)
		Menu_DrawGL();

	
	// Return success at the end of this function
	return TRUE;										
}

/*
// NeHe
GLvoid KillGLWindow(GLvoid)							// Properly Kill The Window
{
	if (g_cvFullscreen.value)
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
*/

GLvoid KillGLWindow(GLvoid)						// Properly Kill The Window
{
	if (g_cvFullscreen.value)							// Are We In Fullscreen Mode?
	{
		// If The Shortcut Doesn't Work ( NEW )
		if (!ChangeDisplaySettings(NULL, CDS_TEST)) 
		{ 		
			ChangeDisplaySettings(NULL, CDS_RESET);		// Do It Anyway (To Get The Values Out Of The Registry) ( NEW )
			ChangeDisplaySettings(&DMsaved, CDS_RESET);	// Change It To The Saved Settings ( NEW )
		} 
		else 
		{
			ChangeDisplaySettings(NULL,CDS_RESET);		// If It Works, Go Right Ahead ( NEW )
		}
			
		ShowCursor(TRUE);					// Show Mouse Pointer
	}

	if (hRC)							// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))				// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))				// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;						// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))				// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;						// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))				// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;						// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;						// Set hInstance To NULL
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

	// Set The Global Fullscreen Flag
	Cvar_SetValue("Fullscreen", (float)fullscreenflag);

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

	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DMsaved);	// Save The Current Display State ( NEW )

	// Attempt Fullscreen Mode?
	if (g_cvFullscreen.value)
	{
		
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
				// Windowed Mode Selected.  Fullscreen = FALSE
				Cvar_SetValue("Fullscreen", 0);
			else	{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	
	// Attempt To Register The Window Class
	if (!RegisterClass(&wc))
	{
		// Display error message and exit routine if function fails
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	

	// Setup window style for fullscreen window
	if (g_cvFullscreen.value) 
	{
		dwExStyle = WS_EX_APPWINDOW;							// Window Extended Style
		dwStyle = WS_POPUP;										// Windows style
		ShowCursor (FALSE);										// Hide Mouse Pointer
	}
	// Setup window style for window
	else	
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;						// Windows Style

		// Adjust Window To True Requested Size
		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		
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

	// Set pixel format
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
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

/*
	// Check that OpenGL acceleration is present and dump out if not
	if (!g_bGLAccelerated)
	{
		KillGLWindow();
		MessageBox(NULL, "This program requires an OpenGL accelerator.", "ERROR", MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
*/	
		
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

			// Check Minimization State
			
			// Program is active
			if (!HIWORD(wParam))					
			{
				g_bActive = TRUE;					
			}
			// Program Is No Longer Active
			else
			{
				g_bActive = FALSE;
				PauseGame(TRUE);
			}

			// Reacquire DirectInput devices
			// DIUtil_ReacquireInputDevices();

			// Return To The Message Loop
			return 0L;
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

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			if (g_bActive)
				keys[wParam] = TRUE;				// If So, Mark It As TRUE
			return 0L;								// Jump Back
			
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			if (g_bActive)
				keys[wParam] = FALSE;			// If So, Mark It As FALSE
			return 0L;								// Jump Back
			
		}

/*
		// Mouse buttons are mapped to unused keys
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
			if (g_bActive)
				keys[253] = bool(uMsg == WM_LBUTTONDOWN);
			return 0L;
		case WM_MBUTTONUP:
		case WM_MBUTTONDOWN:
			if (g_bActive)
				keys[254] = bool(uMsg == WM_MBUTTONDOWN);
			return 0L;
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
			if (g_bActive)
				keys[255] = bool(uMsg == WM_RBUTTONDOWN);
			return 0L;
*/

		// Resize The OpenGL Window
		case WM_SIZE:
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
	g_pMenuBeepSound			= DSUtil_CreateSound(TEXT("MENUBEEP"), 1);

    return S_OK;
}


//
// NewValue needs to be a value between 0 and 1.0f
//
void SetVolume(float NewValue)
{

	#define DSVOLUME_TO_DB(volume) ((DWORD)(-30 * (100 - int(volume * 100.0f))))
	
	LONG lVolume = 0;

	lVolume = DSVOLUME_TO_DB(NewValue);

	DSUtil_SetVolume(g_pShipThrustSound,		lVolume);
	DSUtil_SetVolume(g_pLSaucerSound,			lVolume);
	DSUtil_SetVolume(g_pBulletFiringSound,		lVolume);
	DSUtil_SetVolume(g_pSmallExplodeSound,		lVolume);
	DSUtil_SetVolume(g_pMediumExplodeSound,	lVolume);
	DSUtil_SetVolume(g_pLargeExplodeSound,		lVolume);
	DSUtil_SetVolume(g_pSaucerFireSound,		lVolume);
	DSUtil_SetVolume(g_pLifeSound,				lVolume);
	DSUtil_SetVolume(g_pThumpHiSound,			lVolume);
	DSUtil_SetVolume(g_pThumpLoSound,			lVolume);
	DSUtil_SetVolume(g_pMenuBeepSound,			lVolume);

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
	DSUtil_DestroySound (g_pMenuBeepSound);

	g_pBulletFiringSound		= NULL;
	g_pSmallExplodeSound		= NULL;
	g_pMediumExplodeSound	= NULL;
	g_pLargeExplodeSound		= NULL;
	g_pShipThrustSound		= NULL;
	g_pThumpHiSound			= NULL;
	g_pThumpLoSound			= NULL;
	g_pLSaucerSound			= NULL;
	g_pSaucerFireSound		= NULL;
	g_pLifeSound				= NULL;
	g_pMenuBeepSound			= NULL;

	DSUtil_FreeDirectSound();

}



/* 
	These functions are used in the menus to play
	sounds when the user navigates through the menu.

	Normally I would have the sound code right in
	the menus, but it would mean that a bunch of stuff
	that DirectSound requires to get going would 
	have to be referenced in menus.cpp.  This way,
	menus.cpp stays clean.
*/

void PlayMenuBeepSound()
{
	if (g_cvSoundEnabled.value)
		DSUtil_PlaySound(g_pMenuBeepSound, 0);
}

void PlayMenuLowSound()
{
	if (g_cvSoundEnabled.value) 
		DSUtil_PlaySound(g_pThumpLoSound, 0);
}

void PlayMenuExplosionSound()
{
	if (g_cvSoundEnabled.value)
		DSUtil_PlaySound(g_pLargeExplodeSound, 0);
}

/*
	ResumeLoopingSounds
	KillLoopingSounds

  These are support functions that go along with SetSound
  and could be called from any where in the program where
  all the looping sounds in the game should be enabled/disabled
  en masse.
*/
void ResumeLoopingSounds(void)
{
	// Start playing looped sounds
	if (g_bThrustSoundOn)
		DSUtil_PlaySound(g_pShipThrustSound, DSBPLAY_LOOPING);
	if (actors[IDX_LSAUCER].active == TRUE)
		DSUtil_PlaySound(g_pLSaucerSound, DSBPLAY_LOOPING);
}	



void KillLoopingSounds(void)
{
	// Stop playing looped sounds
	if (g_bThrustSoundOn)
		DSUtil_StopSound(g_pShipThrustSound);
	if (actors[IDX_LSAUCER].active == TRUE)
		DSUtil_StopSound(g_pLSaucerSound);
}


/*
 This is used to enable/disable the sound.  It takes 
 care of everything from setting the SoundEnabled cvar
 to en/disabling the looping sounds.
*/
void SetSound(const BOOL value)
{
	if (g_bDirectSoundEnabled)
	{
		Cvar_SetValue("SoundEnabled", (FLOAT)value);

		if (g_cvSoundEnabled.value)
		{
			UpdateMessage("SOUND ENABLED");

			// Start playing looped sounds
			ResumeLoopingSounds();

		}
		else
		{
			UpdateMessage("SOUND DISABLED");

			// Stop playing looped sounds
			KillLoopingSounds();

		}
	}
	else
	{
		UpdateMessage("DIRECTSOUND INITIALIZATION FAILED");
	}
}

/* This is used to enable/disable the mouse.  It takes
	care of everything from setting the MouseEnabled cvar
	to un/acquiring the mouse.
*/
void SetMouse(const BOOL value)
{

	if (g_bDirectInputEnabled)
	{
		
		// Set cvar 
		Cvar_SetValue("MouseEnabled", (FLOAT)value);
	
		// Based on value, enable the mouse
		if (value)
		{
			if (g_pdidMouse)
			{
				g_pdidMouse->Unacquire();
				g_pdidMouse->Acquire();
			}
			// UpdateMessage("MOUSE ENABLED");
		}
		else
		{
			if (g_pdidMouse) 
			{
				g_pdidMouse->Unacquire();
			}
			// UpdateMessage("MOUSE DISABLED");
		}
	}
}


/*
	This is used by the key bind menu to release the mouse
	once key binding is complete
*/
void UnacquireMouse()
{
	if (g_pdidMouse)
		g_pdidMouse->Unacquire();
}


//-----------------------------------------------------------------------------
// Name: DestroyInput()
// Desc: Ripped from FFDonuts and modified for my ff effects
//-----------------------------------------------------------------------------
void DestroyInput()
{
	// Delete effect objects
	DIUtil_DeleteEffect( g_pExplodeFFEffect );

	// Unacquire and release joystick
	if( g_pdidJoystick )
	{
		g_pdidJoystick->Unacquire();
		g_pdidJoystick->Release();
		g_pdidJoystick = NULL;
	}

	// Unacquire and release mouse
	if (g_pdidMouse)
	{
		g_pdidMouse->Unacquire();
		g_pdidMouse->Release();
		g_pdidMouse = NULL;
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

    // Initialize DirectInput
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
        }
		  else if (dwDevType == DIDEVTYPE_MOUSE)
		  {
			  g_pdidMouse = DIUtil_CreateDevice(hWnd, &pDevices[i]);
		  }
    }
	 
    // Try to find and initialize force feedback
    if(g_pdidJoystick != NULL)
    {
		 // If the joystick is force feedback, ask user for options and setup effects
		 if(DIUtil_IsForceFeedback(g_pdidJoystick))
		 {
			  // Create force feedback effects
			  g_pExplodeFFEffect = DIUtil_CreateEffect();
			  g_pExplodeFFEffect->diEffect.dwGain = (g_dwForceFeedbackGain * 100L);
			  DIUtil_SetupPeriodicEffect(g_pExplodeFFEffect, g_pdidJoystick);

			  // Make the stick shake on startup
			  DIUtil_PlayEffect( g_pExplodeFFEffect );

		 }
	 }

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: GetDeviceInput()
// Desc: Processes data from the input device.  Uses GetDeviceState().
//		 Ripped from FFDonuts and modified.  I also added mouse handling
//		code from one of the samples as well.
//-----------------------------------------------------------------------------
DWORD GetDeviceInput()
{
    HRESULT     hr;
    DIJOYSTATE  dijs;
    DWORD       dwInput = 0;

	 // Make sure a joystick exists
	 if ((g_pdidJoystick != NULL) && (g_cvJoystickEnabled.value != 0))
	 {

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
			  dwInput |= JOYSTICK_SHIELDS;

		 if (dijs.rgbButtons[2] & 0x80)
			 dwInput |= JOYSTICK_HYPERSPACE;

	 }
	 

	// Do mouse
	if ((g_pdidMouse != NULL) && (g_cvMouseEnabled.value != 0))
	{
		// get the input's device state, and put the state in dims
		hr = g_pdidMouse->GetDeviceState(sizeof(DIMOUSESTATE), &g_dims);

		// If the mouse input is lost, try to reacquire it
		if (hr == DIERR_INPUTLOST)
		{
			// DirectInput is telling us that the input stream has
			// been interrupted.  We aren't tracking any state
			// between polls, so we don't have any special reset
			// that needs to be done.  We just re-acquire and
			// try again.
			g_pdidMouse->Acquire();

			// Nothing was read - exit until next loop
			return 0;
		}
	}

/*
	// Mouse debug message
	UpdateMessage("dims = %d %d %c %c %c %c", g_dims.lX, 
			g_dims.lY, 
			g_dims.rgbButtons[0] & 0x80 ? '0' : ' ',
			g_dims.rgbButtons[1] & 0x80 ? '1' : ' ',
			g_dims.rgbButtons[2] & 0x80 ? '2' : ' ',
			g_dims.rgbButtons[3] & 0x80 ? '3' : ' ');
*/
	
	// Cheat and map mouse buttons to keys for user binds
	keys[252] = (g_dims.rgbButtons[0] & 0x80);
	keys[253] = (g_dims.rgbButtons[1] & 0x80);
	keys[254] = (g_dims.rgbButtons[2] & 0x80);
	keys[255] = (g_dims.rgbButtons[3] & 0x80);
	
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

	if (g_cvSoundEnabled.value)
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

	if (g_cvSoundEnabled.value)
	{
		DSUtil_StopSound(g_pLSaucerSound);
	}
}


//--------------------------------------------------------
// TCW
// This routine spawns the player, but first makes sure that
// the player won't be spawned into the center of an enemy.
//--------------------------------------------------------
BOOL SpawnPlayer(BOOL hyperspace = FALSE) 
{

	int i						= 0;
	int group				= 0;
	int lbound				= 0;
	int ubound				= 0;
	static float firsttime = 0;
	BOOL bSpawn			= FALSE;

	const float fDefaultCollDist = 0.4f;
	
	// Set default player properties 
	if (!hyperspace) {

		player->active	= FALSE;
		player->type	= ACTOR_PLAYER;
		player->x		= 0;
		player->y		= 0;
		player->z		= WORLD_DEPTH;
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

	}

	// Randomly position player after hyperspacing
	else {
		player->x		= randFloat(-WORLD_HALFWIDTH  * 0.75f,  WORLD_HALFWIDTH * 0.75f);
		player->y		= randFloat(-WORLD_HALFHEIGHT * 0.75f, WORLD_HALFHEIGHT * 0.75f);
		player->rz		= randFloat(0, 360);
		player->vx		= 0;
		player->vy		= 0;
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

			// Time until next debris is spawned (in seconds)
			actors[j].ParticleSpawnDelay = randFloat(0.01f, 0.05f);

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
		while ((i <= UBound) && (obj[0] == NULL));
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
					if (!g_cvInfiniteShields.value)
						obj[i]->ShieldsPower -= 25;
				}
				else if (g_cvInvincibility.value)
				{
					// Player is invincible -- no damage
				}
				else 
				{
					// Activate force feedback
					if ((g_cvJoystickEnabled.value != 0) && (g_cvForceFeedbackEnabled.value != 0))
					{
						DIUtil_PlayEffect( g_pExplodeFFEffect );
					}

					// Player death sound
					if (g_cvSoundEnabled.value) 
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

						if (g_cvSoundEnabled.value) 
						{
							// DSUtil_PlaySound(g_pLargeExplodeSound, 0);
							DSUtil_PlayPannedSound(g_pLargeExplodeSound, obj[i]->x / WORLD_HALFWIDTH * cStereoSeperation);
						}
						
						// Activate force feedback
						if ((g_cvJoystickEnabled.value != 0) && (g_cvForceFeedbackEnabled.value != 0))
						{
							DIUtil_PlayEffect( g_pExplodeFFEffect );
						}
						
						break;
					
					// Medium rock
					case 1:
						
						player->Score += 50;
						
						if (g_cvSoundEnabled.value)
						{
							// DSUtil_PlaySound(g_pMediumExplodeSound, 0);
							DSUtil_PlayPannedSound(g_pMediumExplodeSound, obj[i]->x / WORLD_HALFWIDTH * cStereoSeperation);
						}
						
						// Activate force feedback
						if ((g_cvJoystickEnabled.value != 0) && (g_cvForceFeedbackEnabled.value != 0))
							DIUtil_PlayEffect( g_pExplodeFFEffect );
						
						break;

					// Small rock
					case 0:
						
						player->Score += 100;

						if (g_cvSoundEnabled.value)
						{
							// DSUtil_PlaySound(g_pSmallExplodeSound, 0);
							DSUtil_PlayPannedSound(g_pSmallExplodeSound, obj[i]->x / WORLD_HALFWIDTH * cStereoSeperation);
						}
						
						// Activate force feedback
						if ((g_cvJoystickEnabled.value != 0) && (g_cvForceFeedbackEnabled.value != 0))
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
					actors[k].LifeEndTime	= g_fNow + 0.4f;
					actors[k].LifeStartTime	= g_fNow;
				}


				// Decrement the size of the current rock
				obj[i]->size--;

				if (obj[i]->size >= 0)
				{

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
						obj[i]->xcolldist = 0.3f * (obj[i]->size + 1);
						obj[i]->ycolldist = obj[i]->xcolldist;

						// Set rock[k] to the same values as rock[i]
						actors[k]		= *obj[i];
						
						// Make new rock darker than source rock
						actors[k].color.r = (GLuint)(actors[k].color.r * 0.75f);
						actors[k].color.g = (GLuint)(actors[k].color.g * 0.75f);
						actors[k].color.b = (GLuint)(actors[k].color.b * 0.75f);
						
						// Make sure resulting rock isn't too dark
						AdjustRockColor(k);

						// Set negative angles for rock[k]
						actors[k].rx	= -obj[i]->rx;
						actors[k].ry	= -obj[i]->ry;
						actors[k].vx	= -obj[i]->vx;
						actors[k].vy	= -obj[i]->vy;

						// Set objects to +/- 60 degrees of the main rock's former heading
						obj[i]->rz		= obj[i]->rz + 60;
						actors[k].rz	= obj[i]->rz - 60;

					}
				}

				// Create debris
				CreateDebris(obj[i], int(8.0f * (obj[i]->size + 2.0f)));

				break;

				case ACTOR_LSAUCER:

					// Kill actor
					DestroySaucer(IDX_LSAUCER);

					// Activate force feedback
					if ((g_cvJoystickEnabled.value != 0) && (g_cvForceFeedbackEnabled.value != 0))
					{
						DIUtil_PlayEffect( g_pExplodeFFEffect );
					}

					// Increase score and play explosion sound
					player->Score += 500;
					if (g_cvSoundEnabled.value) 
					{
						// DSUtil_PlaySound(g_pLargeExplodeSound, 0); 
						DSUtil_PlayPannedSound(g_pLargeExplodeSound, obj[i]->x / WORLD_HALFWIDTH * cStereoSeperation);
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


/*

	GetRefreshTimingRate()

	This function returns the time between two SwapBuffers().
	The time includes a second call to SwapBuffers, so it is
	slightly inaccurate.
  
	This routine is the first step towards the intelligent
	refresh rate control.  

*/
float GetRefreshTimingRate(void)
{
	int nConsec = 0;
	float fTime = 0;
	float fLastTime = 0;
	int nAttempts = 0;

	// Give the program some method of escaping if the
	// refresh rates never agree
	while (nAttempts++ < 25)
	{

		// Swap buffers
		SwapBuffers(hDC);

		// Get current time
		fTime = TimerGetTime();

		// Swap buffers again
		SwapBuffers(hDC);

		// Calc time between swaps
		fTime = TimerGetTime() - fTime;

		// Compare the current value with the last 
		// one (with some variance) 
		if (fabs(fTime - fLastTime) < 0.0005f)
		{
			nConsec++;

			// Make sure that the last three timing 
			// agree to avoid accepting bad data
			if (nConsec >= 10)
				return fTime;

		}

		// Remember the current value of fTime for 
		// the next evaluation
		fLastTime = fTime;

	} 

	// Display message to let user know that this 
	// routine didn't work.
	UpdateMessage("GetRefreshTimingRate FAILED");

	return 0;
}



// Game initialization 
BOOL InitializeGame(void)
{
	int nWindowWidth = 0;
	int nWindowHeight = 0;
	int nColorDepth = 0;

	// Use g_cvResolution to get window res 
	// 
	// Make sure that value of resolution is within bounds before 
	// setting values
	if ((g_cvResolution.value >= RESOLUTIONS_COUNT) || (g_cvResolution.value < 0))
	{
		Cvar_SetValue("Resolution", 0);
	}
	nWindowWidth = resolutions[(int)g_cvResolution.value].width;
	nWindowHeight = resolutions[(int)g_cvResolution.value].height;

	// Use g_cvColorDepth to get window color depth
	// 
	// Make sure that value of ColorDepth is within bounds before setting values
	if ((g_cvColorDepth.value >= COLORDEPTHS_COUNT) || (g_cvColorDepth.value < 0))
	{
		Cvar_SetValue("ColorDepths", 0);
	}
	nColorDepth = colordepths[(int)g_cvColorDepth.value];
		

	// Create Our OpenGL Window -- quit if window was not created
	if (!CreateGLWindow(APP_TITLE, nWindowWidth, nWindowHeight, nColorDepth, (BOOL)g_cvFullscreen.value))
		return FALSE;

	// Can play game without sound. Do not exit
	if(FAILED(InitializeGameSounds())) 
		g_bDirectSoundEnabled = FALSE;
	else 
		g_bDirectSoundEnabled = TRUE;

	// Default sound enabled on if the system
	// is sound capable
	if ((g_cvSoundEnabled.value == 2.0f) || !g_bDirectSoundEnabled)
	{
		Cvar_SetValue("SoundEnabled", (float)g_bDirectSoundEnabled);
	}

	// Set the sound's volume
	SetVolume(g_cvVolume.value);
		
	// Attempt to setup DirectInput.  Don't exit if it fails
	if( FAILED(InitializeInput(hWnd)))
	{
		g_bDirectInputEnabled = FALSE;
	}
	else
	{
		g_bDirectInputEnabled = TRUE;

		// Enable joystick by default
		Cvar_SetString("JoystickEnabled", "-1");
		
		// Attempt to activate force feedback and enable it if it's present
		g_bForceFeedbackCapable = BOOL(HasForceFeedbackJoystick(hInstance) == TRUE);

		// If the joystick is capable of force feedback, enable it 
		Cvar_SetValue("ForceFeedbackEnabled", (g_bForceFeedbackCapable != 0));
		
	}

	// Setup rendering flags
	renderinit();

	// Setup Vsync
	SetVsync(!(int)Cvar_GetValue("VsyncEnabled"));

	// Get refresh timing rate for automatic frame rate control
	g_fSwapBufferInterval = GetRefreshTimingRate();

	// Set gamma
	SetGamma();

	// Return  success
	return TRUE;
}


// Kill GL lists, sounds, controls, window
void TerminateGame(void)
{
	// Kill GL fonts
	KillFont();

	// Kill display lists
	glDeleteLists(player_list, NUM_LISTS);

	// Kill textures
	glDeleteTextures(NUM_TEXTURES, &texture[0]);

	// Turn off DirectSound
	CleanupGameSounds();

	// Turn off DirectInput
	DestroyInput();

	// Restore gamma ramp before exit
	SetDeviceGammaRamp(hDC, old_gamma_ramp);

	// Kill The Window
	KillGLWindow();

}


/*

  Used in AdjustRockColor to convert bytes into something usable

  I don't know if there's a built-in C++ function to do this,
  so I wrote this myself.  ;)

*/
unsigned int ConvertGLbyteToUint(const GLbyte& value)
{
	if (value < 0)
		return (unsigned int)abs(value) + 128;
	else
		return (unsigned int)value;
}


/* 
	This function makes sure that rocks don't get 
	too dark, while still maintaining the cool rock
	colors of previous versions of the game.
*/
void AdjustRockColor(const int& i)
{

	unsigned int r = 0;
	unsigned int g = 0;
	unsigned int b = 0;
	
	float fRGBSum = 0;
	float fRGBAvg = 0;
	float fPctR = 0;
	float fPctG = 0;
	float fPctB = 0;
	int	nTemp = 0;
	
	const float cMinAvg = 102.0f;
	const float cMinSum = cMinAvg * 3.0f;

	// Convert bytes to unsigned int values
	r = ConvertGLbyteToUint(actors[i].color.r);
	g = ConvertGLbyteToUint(actors[i].color.g);
	b = ConvertGLbyteToUint(actors[i].color.b);

	// Add values
	fRGBSum = float(r) + float(g) + float(b);
	fRGBAvg = fRGBSum / 3.0f;

	if (fRGBAvg < cMinAvg)
	{
		fPctR = r / fRGBSum;
		fPctG = g / fRGBSum;
		fPctB = b / fRGBSum;

		nTemp = int(cMinSum * fPctR);
		if (nTemp > 255) 
			nTemp = 255;
		actors[i].color.r = (GLbyte)nTemp;

		nTemp = int(cMinSum * fPctG);
		if (nTemp > 255)
			nTemp = 255;
		actors[i].color.g = GLbyte(nTemp);

		nTemp = int(cMinSum * fPctB);
		if (nTemp > 255)
			nTemp = 255;
		actors[i].color.b = (GLbyte)nTemp;

	}

}

// CVar code 
void InitCvars()
{

	Cvar_Register(&g_cvResolution);
	Cvar_Register(&g_cvColorDepth);

	Cvar_Register(&g_cvGammaGamma);
	Cvar_Register(&g_cvGammaBrightness);
	Cvar_Register(&g_cvGammaContrast);

	Cvar_Register(&g_cvFullscreen);
	Cvar_Register(&g_cvSpaceMode);
	Cvar_Register(&g_cvCopernicusMode);
	Cvar_Register(&g_cvDifficulty);

	Cvar_Register(&g_cvKeyThrust[0]);
	Cvar_Register(&g_cvKeyThrust[1]);
	Cvar_Register(&g_cvKeyShields[0]);
	Cvar_Register(&g_cvKeyShields[1]);
	Cvar_Register(&g_cvKeyHyperspace[0]);
	Cvar_Register(&g_cvKeyHyperspace[1]);
	Cvar_Register(&g_cvKeyRotateLeft[0]);
	Cvar_Register(&g_cvKeyRotateLeft[1]);
	Cvar_Register(&g_cvKeyRotateRight[0]);
	Cvar_Register(&g_cvKeyRotateRight[1]);
	Cvar_Register(&g_cvKeyFire[0]);
	Cvar_Register(&g_cvKeyFire[1]);
	Cvar_Register(&g_cvKeyStartGame[0]);
	Cvar_Register(&g_cvKeyStartGame[1]);
	Cvar_Register(&g_cvKeyPause[0]);
	Cvar_Register(&g_cvKeyPause[1]);

	Cvar_Register(&g_cvShipRotateAdjust);
	
	Cvar_Register(&g_cvMouseSensitivity);
	Cvar_Register(&g_cvMouseEnabled);

	Cvar_Register(&g_cvLightingEnabled);
	Cvar_Register(&g_cvBlendingEnabled);
	Cvar_Register(&g_cvTexturesEnabled);
	Cvar_Register(&g_cvWireframeEnabled);
	Cvar_Register(&g_cvBoundingBoxesEnabled);
	Cvar_Register(&g_cvFpsEnabled);
	Cvar_Register(&g_cvVsyncEnabled);
	Cvar_Register(&g_cvSpaceSize);

	Cvar_Register(&g_cvJoystickEnabled);
	Cvar_Register(&g_cvForceFeedbackEnabled);
	
	Cvar_Register(&g_cvSoundEnabled);
	Cvar_Register(&g_cvVolume);

	Cvar_Register(&g_cvInfiniteLives);
	Cvar_Register(&g_cvInfiniteShields);
	Cvar_Register(&g_cvInvincibility);
}

void InitActors() {

		// Initialize actors
	for (int i = 0; i < MAX_ACTORS; i++) 
	{
		actors[i].active			= FALSE;
		actors[i].type				= ACTOR_NONE;
		actors[i].displaylistid = NULL;
		actors[i].x					= 0;
		actors[i].y					= 0;
		actors[i].z					= WORLD_DEPTH;
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
		actors[i].Hyperspacing	= FALSE;
		actors[i].HyperspaceInterval = 0;
		actors[i].HyperspaceDelay = 0;
	}
	
	
	// Setup player to point to the point where player1 is 
	// in the actors list
	player = &actors[IDX_PLAYER1];

	// Setup shields
	actors[IDX_SHIELDS].active = TRUE;
	actors[IDX_SHIELDS].type = ACTOR_SHIELDS;
	actors[IDX_SHIELDS].displaylistid = shields_list;
	actors[IDX_SHIELDS].x		= 0;
	actors[IDX_SHIELDS].y		= 0;
	actors[IDX_SHIELDS].z		= WORLD_DEPTH;
	actors[IDX_SHIELDS].rx		= 0;
	actors[IDX_SHIELDS].ry		= 0;
	actors[IDX_SHIELDS].rz		= 0;
	actors[IDX_SHIELDS].vx		= 0;
	actors[IDX_SHIELDS].vy		= 0;
	actors[IDX_SHIELDS].vz		= 0;
	actors[IDX_SHIELDS].color.r		= (char)255;
	actors[IDX_SHIELDS].color.g		= (char)255;
	actors[IDX_SHIELDS].color.b		= (char)255;
	actors[IDX_SHIELDS].LifeEndTime	= 0;
	actors[IDX_SHIELDS].size			= 0;
	actors[IDX_SHIELDS].ShieldsUp		= FALSE;
	actors[IDX_SHIELDS].ShieldsPower	= 0;
	actors[IDX_SHIELDS].xcolldist = 0.5f;
	actors[IDX_SHIELDS].ycolldist = 0.5f;
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

	MSG	msg;							// Windows Message Structure

	int		i				= 0;		// i,j,k are counter variables
	int		j				= 0;
	int		k				= 0;
	
	BOOL	bPlayerThrust	= FALSE; // Variable replaces the use of velocity
											// to flag when player is thrusting. 
											// Based on the amount of time between
											// frames, velocity would sometimes be zero.

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
	BOOL	bPlayThump			= FALSE;	// Toggles between hi and lo thump
	float	fNextThumpTime		= 0;		// Ticks when next thump will occur

	// FPS variables
	long	lFramesRendered			= 0;
	float	fProgramStartTime			= 0;

	// DirectInput variables
	DWORD	DirectInput					= 0;
	DWORD	LastDirectInput			= 0;

	// Automatic framerate adjust variables
	const float DEFAULT_SWAPBUFFER_TIMING_ADJUST = 0.003f;
	float fTime						=0;
	float fLastSwapBufferTime = 0;
	float fSwapBufferTimingAdjust = DEFAULT_SWAPBUFFER_TIMING_ADJUST;
	int	nConsecutiveMissedSwapBuffers = 0;
	float fLastDrawGLSceneTime = 0;
	int	nConsecutiveExcessiveSwapBufferWait = 0;

	float fLastUpdateTime = 0;

	int nSpaceSize = 0;

	char sKeyBuffer[16]		= "";

	// Setup an array to keep track of when 
	// the last 10 hyperspaces took place
	// so that player can be penalized for
	// misuse of the hyperspace.  :)
	int	nHyperspaceUseCount = 0;
	float	afPreviousHyperspaceTicks[10] = {0,0,0,0,0,0,0,0,0,0};
	
	// Init timers
	TimerInit();

	// Initialize timing variables
	g_fNow = TimerGetTime();
	fProgramStartTime = g_fNow;

	// Display version info on startup 
	UpdateMessage(VERSION);
	g_lMessageEndTime = (long)g_fNow + 5;

	// Initialize Cvars
	InitCvars();

	// Load the game's configuration settings, which are 
	// stored as cvars.
	LoadCfg();

	// Prompt the user to select screen mode.  This is only done
	// once, the first time the program is run.  After that, the
	// program defaults to the console variable value.
	if (Cvar_GetValue("Fullscreen") == 2) 
	{
		if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION) == IDNO)
			Cvar_SetValue("Fullscreen", FALSE);
		else
			Cvar_SetValue("Fullscreen", TRUE);
	}

	// Setup graphics, controls and sounds
	if (!InitializeGame())
		return 0;

	// Release mouse until game actually starts
	UnacquireMouse();
	
	// Initialize Sin and Cos lookup tables
	InitLookupTables();

	// New random number seed
	srand( (unsigned)time( NULL ) );

	// Load the highscores
	LoadHighScores("disasteroids3d.dat");

	// Initialze stars
	for (i = 0; i < NUM_STARS; i++)
	{
		stars[i].x = randFloat(-STAR_WORLD_HALFWIDTH, STAR_WORLD_HALFWIDTH);
		stars[i].y = randFloat(-STAR_WORLD_HALFHEIGHT, STAR_WORLD_HALFHEIGHT);
		stars[i].z = randFloat(MIN_STAR_Z, MAX_STAR_Z);
		stars[i].size = randFloat(MIN_STAR_SIZE, MAX_STAR_SIZE);
	}

	// To simplify this routine I moved the actor
	// initialization into its own routine
	InitActors();



	// Main program loop
	while(!g_bExitApp)
	{

		// Update time
		if (!g_bGamePaused)
		{

			g_fNow = TimerGetTime() - g_fNowOffset;
			fDeltaTime = g_fNow - fLastLoopTime;

			if (fDeltaTime > 0)
				g_nFps = int(1.0f / fDeltaTime);

			fLastLoopTime = g_fNow;

		}

		// Compute average FPS
		g_fGameLength = g_fNow - fProgramStartTime;


		// Is There A Message Waiting?
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				g_bExitApp = TRUE;					// If So g_bExitApp=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);			// Translate The Message
				DispatchMessage(&msg);			// Dispatch The Message
			}
		}

		// Don't hog processor when game isn't active
		if (!g_bActive)
		{
			WaitMessage();
		}

		// Normal loop
		else
		{
			// Swap buffers -- control max frame rate when
			// vsync is enabled
			if (g_cvVsyncEnabled.value) 
			{

				fTime = TimerGetTime();
				if (fTime - fLastSwapBufferTime >= g_fSwapBufferInterval - fSwapBufferTimingAdjust)
				{

					// Swap the buffers
					SwapBuffers(hDC);

					// Check to make sure that the SwapBuffers() call
					// wasn't too early and increment the consec missed
					// counter if so
					if (TimerGetTime() - fTime >= g_fSwapBufferInterval)
						nConsecutiveMissedSwapBuffers ++;
					else
						nConsecutiveMissedSwapBuffers = 0;

					// Adjust timing if five swap buffers are missed in a row
					if (nConsecutiveMissedSwapBuffers >= 5)
					{
						fSwapBufferTimingAdjust += 0.002f;
						nConsecutiveMissedSwapBuffers = 0;
					}

					// Make sure that SwapBufferTimingAdjust doesn't go screwy.
					// If the value is higher than fSwapBufferInterval, it's time
					// to reset the frame rate adjust system and try again
					if (fSwapBufferTimingAdjust >= g_fSwapBufferInterval * 0.75f)
					{
						g_fSwapBufferInterval = GetRefreshTimingRate();
						fSwapBufferTimingAdjust = DEFAULT_SWAPBUFFER_TIMING_ADJUST;
					}

					// Update FPS 
					if (g_cvFpsEnabled.value)
						UpdateMessage("FPS: %4.0f  Adjust=%f", 1 / (TimerGetTime() - fLastSwapBufferTime), fSwapBufferTimingAdjust);

					// Reset the buffer swap timer
					fLastSwapBufferTime = TimerGetTime();

				}
				
				//	The scene is drawn twice for every time
				// SwapBuffer is called.  This is done to keep
				// the number of tossed away frames to a minimum
				if (TimerGetTime() - fLastDrawGLSceneTime >= g_fSwapBufferInterval * 0.5f)
				{
					
					// Update last scene time
					fLastDrawGLSceneTime = TimerGetTime();

					// Draw the scene
					if (g_bActive)
					{
						if (!DrawGLScene()) 
						{
							g_bExitApp = TRUE;
						}
					}
				}

			} // if (g_cvVsyncEnabled.value)


			// Run full speed when the vsync is disabled
			else
			{

				// Only draw the scene when the window is active
				if (g_bActive)
				{
					if (!DrawGLScene())
					{
						g_bExitApp = TRUE;
					}
					else
					{
						SwapBuffers(hDC);
					}
				}

				// Update FPS 
				if ((g_cvFpsEnabled.value) && (fDeltaTime))
					UpdateMessage("FPS: %4.0f", 1 / fDeltaTime);

			}


			// Get joystick and mouse inputs (if enabled)
			if (g_bDirectInputEnabled == TRUE)
			{
				DirectInput = GetDeviceInput();
			}


			// Don't thump if the game's over
			if (!g_bGameOver) {
			
				// Setup the first thump
				if (!fNextThumpTime)
					fNextThumpTime = g_fNow + 1.0f;
				
				// Play the thump
				if ((g_fNow > fNextThumpTime) && (g_cvSoundEnabled.value)) {
					if (bPlayThump)
						DSUtil_PlaySound( g_pThumpLoSound, 0 );
					else
						DSUtil_PlaySound( g_pThumpHiSound, 0 );

					// Alternate thumps
					bPlayThump = !bPlayThump;
				
					// Set next thump to play in 1 second
					fNextThumpTime = g_fNow + 1.0f;

					// Make the beats speed up over time
					switch ((int)(g_fNow - fLevelStartTime) / 15) {
						case 0:	
							fNextThumpTime = g_fNow + 1.0f;
							break;
						
						case 1:
							fNextThumpTime = g_fNow + 0.75f;
							break;

						case 2:
							fNextThumpTime = g_fNow + 0.5f;
							break;

						default:
							fNextThumpTime = g_fNow + 0.25f;
							break;
					}
				}
			}
	


			// Process keystrokes



			// Highscore initials entry
			if (g_bEnteringHighScoreInitials)
			{
				// Check for backspace -- clears initials
				if (KeyDown(VK_BACK))
				{
					if (g_nHighScoreInitialsIdx > 0)
					{
						g_nHighScoreInitialsIdx--;
						playerhighscore->Initials[g_nHighScoreInitialsIdx] = ' ';

						PlayMenuBeepSound();
					}

				}
				// Check for enter
				else if (KeyDown(VK_RETURN))
				{
					g_bEnteringHighScoreInitials = FALSE;
					playerhighscore = NULL;

					PlayMenuExplosionSound();
				}
				// Check for legal initial chars
				else if (g_nHighScoreInitialsIdx <= 2)
				{			
					for (i = 0; i < 37; i++) {
						if (KeyDown(g_cLegalHighScoreChars[i]))
						{
							playerhighscore->Initials[g_nHighScoreInitialsIdx] = g_cLegalHighScoreChars[i];
							g_nHighScoreInitialsIdx++;
	
							PlayMenuBeepSound();

							break;
						}
					}
				}
			
			}

			// These keys don't work when the highscore is 
			// being entered
			else
			{

				// Certain keys only work between games
				if (g_bGameOver)
				{

					if (((keys[(int)g_cvKeyStartGame[0].value]) || (keys[(int)g_cvKeyStartGame[1].value]) || (g_bStartGame)) && !g_bShowMenu)
					{

						// Reset g_bStartGame
						g_bStartGame = FALSE;
					
						// Game's no longer over
						g_bGameOver = FALSE;

						// Restart game
						g_nLevelNum = 0;					// Reset level number
						fPlayerRespawnTime = 1;			// Respawn player ASAP
						player->Score = 0;				// Reset score

						// Store the current space size selection so that the 
						// highscore table is updated correctly at the end of 
						// the game
						nSpaceSize = int(g_cvSpaceSize.value);

						// Setup world and screen size 
						float fSpaceSizeMultiplier = 0;
						switch (nSpaceSize)
						{
						case 0:
							fSpaceSizeMultiplier = 0.5f;
							break;
						case 1:
							fSpaceSizeMultiplier = 1.0f;
							break;
						case 2:
							fSpaceSizeMultiplier = 1.3f;
							break;
						}
						
						WORLD_WIDTH			= 22	* fSpaceSizeMultiplier;
						WORLD_HALFWIDTH	= 11	* fSpaceSizeMultiplier;
						WORLD_HEIGHT		= 16	* fSpaceSizeMultiplier;
						WORLD_HALFHEIGHT	= 8 	* fSpaceSizeMultiplier;
						WORLD_DEPTH			= -20 * fSpaceSizeMultiplier;

						SCREEN_WIDTH		= 22	* fSpaceSizeMultiplier;
						SCREEN_HALFWIDTH	= 11	* fSpaceSizeMultiplier;
						SCREEN_HEIGHT		= 16	* fSpaceSizeMultiplier;
						SCREEN_HALFHEIGHT	= 8	* fSpaceSizeMultiplier;
						SCREEN_DEPTH		= -20 * fSpaceSizeMultiplier;

						// Reposition actors' depths
						for (i = 0; i < MAX_ACTORS; i++) {
							actors[i].z	= WORLD_DEPTH;
						}

						// Set current difficulty so that it can't 
						// be changed mid-game
						g_nDifficulty = (int)g_cvDifficulty.value;

						// Reset player lives
						if (g_nDifficulty == EASY_DIFFICULTY)
							player->Lives = 5;
						else
							player->Lives = 3;

						player->NextBonus = 10000;		// Reset next bonus

						// Kill rocks
						for (i = IDX_FIRST_ROCK; i < IDX_LAST_ROCK; i++) 
						{
							actors[i].active = FALSE;
						}

						// Kill shots
						for (i = IDX_FIRST_SHOT; i < IDX_LAST_SHOT; i++)
							actors[i].active = FALSE;

						// Kill saucer shots
						for (i = IDX_FIRST_SFIRE; i < IDX_LAST_SFIRE; i++)
							actors[i].active = FALSE;

						// Kill saucer
						actors[IDX_LSAUCER].active = FALSE;

						// Reset rocks ASAP
						fTimeForNewRocks = 1;

						// Acquire mouse if mouse mode is enabled
						SetMouse((BOOL)g_cvMouseEnabled.value);
												
					}
			
				} // if (g_bGameOver)

				// Certain keys only work during games
				else
				{
					if (KeyDown(int(g_cvKeyPause[0].value)) || KeyDown(int(g_cvKeyPause[1].value)))
					{
						if (g_bGamePaused)
							PauseGame(FALSE);
						else
							PauseGame(TRUE);
					}
						
				}


				// ESC key shows game settings menu
				if (!g_bShowMenu)
				{
					if ((KeyDown(VK_ESCAPE)) && (g_bShowMenu == FALSE))
					{

						// Set the show menu flag
						g_bShowMenu = TRUE;

						// Pause the game if the menu is being
						// displayed mid-game
						if (!g_bGameOver)
							PauseGame(TRUE);

						// Initialize variables
						Menu_Init();
					}

					// *SHH* Activate the cheat menu
					if (	(sKeyBuffer[0] == 0x4d) &&
							(sKeyBuffer[1] == 0x4f) &&
							(sKeyBuffer[2] == 0x48) &&
							(sKeyBuffer[3] == 0x54) &&
							(sKeyBuffer[4] == 0x52) &&
							(sKeyBuffer[5] == 0x45) &&
							(sKeyBuffer[6] == 0x50) &&
							(sKeyBuffer[7] == 0x55) &&
							(sKeyBuffer[8] == 0x53))
					{
						// Set the show menu flag
						g_bShowMenu = TRUE;

						// Pause game
						if (!g_bGameOver)
							PauseGame(TRUE);

						// Init variables
						Menu_ShowCheatMenu();
					}

				}
				else
				{
					// When menu is exited, the function returns FALSE.
					// The game should be resumed and g_bShowMenu should
					// be reset
					if (Menu_Update() == FALSE)
					{
						// Reset the show menu flag
						g_bShowMenu = FALSE;

						// Resume game
						PauseGame(FALSE);

					}
				}


				// Don't allow player to hit movement keys while the 
				// game is paused
				if (!g_bGamePaused)
				{
					
					// Try cheesy fix to time problem when rotating
					if (fDeltaTime)
					{

						float fPlayerRxChange = 0;

						// Player turn left
						if ((keys[int(g_cvKeyRotateLeft[0].value)]) || (keys[int(g_cvKeyRotateLeft[1].value)]) || (DirectInput & JOYSTICK_LEFT))
						{
							player->rz += 1200.0f * fDeltaTime * g_cvShipRotateAdjust.value;
							fPlayerRxChange = 50.0f * fDeltaTime;
						}

						// Player turn right
						if ((keys[int(g_cvKeyRotateRight[0].value)]) || (keys[int(g_cvKeyRotateRight[1].value)]) || (DirectInput & JOYSTICK_RIGHT))
						{
							player->rz -= 1200.0f * fDeltaTime * g_cvShipRotateAdjust.value;
							fPlayerRxChange = -50.0f * fDeltaTime;
						}

						// Move player based on mouse position
						if (g_cvMouseEnabled.value != 0)
						{
							if (g_dims.lX)
							{
								player->rz += float(-g_dims.lX) * 0.5f * g_cvMouseSensitivity.value;
								fPlayerRxChange = -g_dims.lX * 3.0f * fDeltaTime;
							}
						}


						// If fPlayerRyChange is 0, set it to a value 
						// that will return ry to zero
						if (fPlayerRxChange == 0)
						{
							if (player->rx < 0) 
								fPlayerRxChange += 250.0f * fDeltaTime;
							else if (player->rx > 0)
								fPlayerRxChange -= 250.0f * fDeltaTime;
						}
						
						// Adjust rx and make sure it stays in bounds
						player->rx += fPlayerRxChange;
						if (player->rx > 45.0f)
							player->rx = 45.0f;
						else if (player->rx < -45.0f)
							player->rx = -45.0f;

						// Reset RxChange
						fPlayerRxChange = 0;

					}

					// Player thrust
					bPlayerThrust = ((keys[int(g_cvKeyThrust[0].value)]) || (keys[int(g_cvKeyThrust[1].value)]) || (DirectInput & JOYSTICK_UP));

					// Activate infinite shields
					if (g_cvInfiniteShields.value)
					{
							player->ShieldsPower = 255;
					}

					// Activate shields when down arrow's down
					if ((keys[int(g_cvKeyShields[0].value)]) || (keys[int(g_cvKeyShields[1].value)]) || (DirectInput & JOYSTICK_SHIELDS))
					{
						if (player->active) 
						{
							player->ShieldsUp = TRUE;
							actors[IDX_SHIELDS].active = TRUE;
						}
					}
					else 
					{
						player->ShieldsUp = FALSE;
						actors[IDX_SHIELDS].active = FALSE;
					}


					// Hyperspace
					if (player->Hyperspacing == FALSE) 
					{

						// Check the hyperspace keys
						if (((keys[int(g_cvKeyHyperspace[0].value)]) || (keys[int(g_cvKeyHyperspace[1].value)]) || (DirectInput & JOYSTICK_HYPERSPACE)) && (player->active)) 
						{
							// Code to keep track of Hyperspace Abusers
							//
							// Shift hyperspace time array down one element
							// and count the number of times hyperspace has 
							// been used in the last ten seconds
							nHyperspaceUseCount = 0;
							for (i = 9; i >= 0; i--)
							{
								afPreviousHyperspaceTicks[i + 1] = afPreviousHyperspaceTicks[i];
								
								// Count uses of the hyperspace key in the last 10 seconds
								if ((g_fNow - afPreviousHyperspaceTicks[i] <= 10) && (afPreviousHyperspaceTicks[i] != 0))
								{
									nHyperspaceUseCount++;
								}
							}
							afPreviousHyperspaceTicks[0] = g_fNow;

							// Debug messages
							// UpdateMessage("HyperspaceUseCount = %d", nHyperspaceUseCount);
							// UpdateMessage("Now %f Last %f Diff %f", g_fNow, afPreviousHyperspaceTicks[1], g_fNow - afPreviousHyperspaceTicks[1]);

							// Set player flags to tell game that player
							// is hyperspacing and setup when he is to reappear
							player->Hyperspacing = TRUE;
							player->HyperspaceDelay = 0.5f;
							player->active = FALSE;

							// Spawn debris
							CreateDebris(player, 10);

						}
					}
					else 
					{
						// The Hyperspace duration is up -- spawn player
						if (player->HyperspaceDelay <= 0) 
						{
							// UpdateMessage("Hyperspace over - spawning player");
						
							// Hyperspace player
							if (SpawnPlayer(TRUE)) 
							{
								
								// Punish Hyperspace Abusers
								if ((nHyperspaceUseCount > randInt(0, 20)) && (nHyperspaceUseCount >= 3))
								{
									
									// Display message -- muhahahaha
									UpdateMessage("BOOM!  HYPERSPACE ABUSER!!!!");

									// Find rock and bash user into it -- BOOM
									for (i = IDX_FIRST_ROCK; i <= IDX_LAST_ROCK; i++)
									{
										if (actors[i].active) {
											player->x = actors[i].x;
											player->y = actors[i].y;
											break;
										}
									}
								}

								// Clear hyperspace flags
								player->Hyperspacing = FALSE;
								player->HyperspaceInterval = 0.05f;
							}
						}
						else 
						{
							player->HyperspaceDelay -= fDeltaTime;
						}
					}


					// Spawn shots 
					if (((keys[int(g_cvKeyFire[0].value)]) || (keys[int(g_cvKeyFire[1].value)]) || (DirectInput & JOYSTICK_FIRE)) && player->active)  
					{
						// Make sure enough time has passed between shots
						if (g_fNow > player->ShotDelay) 
						{

							// Find free actor
							i = FindFreeActor(IDX_FIRST_SHOT, IDX_LAST_SHOT);

							// A free shot space is available
							if (i) {

								// Init actor by setting all of its
								// properties equal to the player's
								actors[i] = actors[IDX_PLAYER1];

								actors[i].active = TRUE;
								actors[i].type = ACTOR_SHOT;
								
								// Make the shots spawn from the tip
								// of the ship instead of the center
								actors[i].x += (float)CosLookup(player->rz) * 0.5f;
								actors[i].y += (float)SinLookup(player->rz) * 0.5f;

								actors[i].vx = 20.1f;
								actors[i].vy = 20.1f;

								actors[i].vz = 0.0f;
								actors[i].displaylistid = shot_list;
								actors[i].LifeEndTime = g_fNow + 0.7f;
								
								actors[i].xcolldist = 0.2f; 
								actors[i].ycolldist = 0.2f;

								// Delay period between shots
								player->ShotDelay = g_fNow + 0.3f;

								// Play sound
								if (g_cvSoundEnabled.value)
								{
									// DSUtil_PlaySound(g_pBulletFiringSound, 0); 
									DSUtil_PlayPannedSound(g_pBulletFiringSound, actors[i].x / WORLD_HALFWIDTH * cStereoSeperation);
								}
							}
						}
					}

					// Let the player fire more quickly when 
					// repeatedly hitting the fire button 
					// rather than holding it down
					if (!keys[int(g_cvKeyFire[0].value)] && !keys[int(g_cvKeyFire[1].value)] && !(DirectInput & JOYSTICK_FIRE))
						player->ShotDelay -= 400;
				
				} // if (!g_bGamePaused)

			} // if (g_bEnteringHighScoreInitials)

			
			// Copy keys into lastkeys
			for (i = 0; i < 255; i++)
			{
				
				// Capture pressed keys into sKeyBuffer[]
				if ((lastkeys[i] != keys[i]) && (keys[i]))
				{
					for (j = 14; j >= 0; j--)
					{
						sKeyBuffer[j] = sKeyBuffer[j -1];
					}
					sKeyBuffer[15] = 0;
					sKeyBuffer[0] = i;
				}

				// Update lastkeys[]
				lastkeys[i] = keys[i];
			}


			// Init flag to try and find when all of the 
			// rocks have been shot
			bAllEnemiesInactive = TRUE;

			// Update actors (when the game isn't paused)
			if (!g_bGamePaused)
			{

				// I'm doing the player seperate from the rest of 
				// the actors so that I can update its position
				// first then move all of the other actors based
				// on its position.  This is necessary for the 
				// player-centric play mode
				if (player->active) {

					// Update all of the actors' LifePercent
					// if their LifeStartTime is given
					if (player->LifeStartTime > 0) 
						player->LifePercent = (float)((float)(g_fNow - player->LifeStartTime) / (float)(player->LifeEndTime - player->LifeStartTime));
					

					// Update player bonus
					if (player->Score >= player->NextBonus)
					{
						player->NextBonus += 10000;
						player->Lives++;

						if (g_cvSoundEnabled.value)
						{
							DSUtil_PlaySound(g_pLifeSound, 0);
						}
					}

					// Make sure the player's invincible when necessary
					if (player->InvincibleEndTime != 0)
					{
						if (player->InvincibleEndTime >= g_fNow)
						{
							player->ShieldsPower = 255;
							player->ShieldsUp = TRUE;
							actors[IDX_SHIELDS].active = TRUE;
						}
						else
						{
							actors[IDX_SHIELDS].active = FALSE;
							player->ShieldsPower = 255;
							player->InvincibleEndTime = 0;
						}
					}

					// Apply Player velocity 
					if (bPlayerThrust) 
					{


	#define use_old_thrust 0
					
	#if use_old_thrust
						float velocity = 0.05f * fDeltaTime;
	#else
						float velocity = 0.075f * fDeltaTime;
	#endif
						
						// Set vx and vy
						player->vx += (float)CosLookup(player->rz) * velocity;
						player->vy += (float)SinLookup(player->rz) * velocity;

						// Zero velocity for next loop
						velocity = 0;

						// Spawn fire
						k = FindFreeActor(IDX_FIRST_PARTICLE, IDX_LAST_PARTICLE);
						if (k)
						{
							actors[k] = *player;
							actors[k].type = ACTOR_FLAMES;
							actors[k].displaylistid = smoke_list;
							actors[k].rz += 180 + randInt(-15,15);
							actors[k].x += (float)CosLookup(actors[k].rz) * 0.55f;
							actors[k].y += (float)SinLookup(actors[k].rz) * 0.55f;
							actors[k].vx = 0.0005f; 
							actors[k].vy = 0.0005f; 
							actors[k].vz = 0;
							actors[k].LifeStartTime = g_fNow;
							actors[k].LifeEndTime = g_fNow + randFloat(0.06f, 0.180f);
							actors[k].size = 0.6f;
						}
						
						// Spawn smoke
						if (fThrustDebrisTime < g_fNow)
						{
						
							k = FindFreeActor(IDX_FIRST_PARTICLE, IDX_LAST_PARTICLE);
							if (k) 
							{
								// Don't allow more thrust for ... ms
								fThrustDebrisTime = g_fNow + randFloat(0.005f, 0.025f);

								// Base smoke on player
								actors[k] = *player;
								actors[k].type = ACTOR_PARTICLE;
								actors[k].displaylistid = smoke_list;
								actors[k].rz = actors[k].rz + 180 + randFloat(-15, 15);
								actors[k].x += (float)CosLookup(actors[k].rz) * 0.5f;
								actors[k].y += (float)SinLookup(actors[k].rz) * 0.5f;
								actors[k].vx = (GLfloat)fabs(actors[k].vx * 0.25f);
								actors[k].vy = (GLfloat)fabs(actors[k].vy * 0.25f);
								actors[k].vz = 0.01f;
								actors[k].color.r = actors[k].color.g = actors[k].color.b = randInt(16,64);
								actors[k].LifeStartTime = g_fNow;
								actors[k].LifeEndTime = g_fNow + randFloat(0, 1.25f);
								actors[k].size = 1;
							}
						}

						// Play thrust sound
						if (g_cvSoundEnabled.value)
							if (g_bThrustSoundOn == FALSE)
								DSUtil_PlaySound( g_pShipThrustSound, DSBPLAY_LOOPING );
						g_bThrustSoundOn = TRUE;

					} 
					else {

	#if use_old_thrust 
						
						// Decelerate player ship
						//
						// Thanks to Marty R. for sending me this...
						//
						// velocity = velocity - (velocity * drag * elapsedTime)
						//
						// ... equation for controlling drift speed
						//
						player->vx = player->vx - (player->vx * 0.3f * fDeltaTime);
						player->vy = player->vy - (player->vy * 0.3f * fDeltaTime);
	#endif

						// Stop sound of player thrust
						if (g_cvSoundEnabled.value)
						{
							DSUtil_StopSound(g_pShipThrustSound);
						}
						
						// Flag thrust sound is now off
						g_bThrustSoundOn = FALSE;

					}

	#if !use_old_thrust
					player->vx -= float(fabs(player->vx) * player->vx * 0.001f + player->vx * 0.5f) * fDeltaTime;
					player->vy -= float(fabs(player->vy) * player->vy * 0.001f + player->vy * 0.5f) * fDeltaTime;
	#endif

					/*
					// This displays the player velocity for debug
					UpdateMessage("Vel x=%f y=%f", player->vx, player->vy);
					*/

					// Update player position
					player->x += player->vx * 200 * fDeltaTime;
					player->y += player->vy * 200 * fDeltaTime;

					// Handle screen wrap
					if (player->x > WORLD_HALFWIDTH) 
						player->x -= WORLD_WIDTH;
					else if (player->x < -WORLD_HALFWIDTH)
						player->x += WORLD_WIDTH;
					if (player->y > WORLD_HALFHEIGHT)
						player->y -= WORLD_HEIGHT;
					else if (player->y < -WORLD_HALFHEIGHT)
						player->y += WORLD_HEIGHT;
				}
			
				
				// Update each actor in the actors[] array
				for (i = 0; i < MAX_ACTORS; i++)
				{
					if ((actors[i].active) && (i != IDX_PLAYER1) && (i != IDX_PLAYER2)){

						// Update all of the actors' LifePercent
						// if their LifeStartTime is given
						if (actors[i].LifeStartTime > 0) 
						{
							actors[i].LifePercent = (float)((float)(g_fNow - actors[i].LifeStartTime) / (float)(actors[i].LifeEndTime - actors[i].LifeStartTime));
						}


						// Player is center of universe
						// if (g_bPreCopernicusMode)
						if (g_cvCopernicusMode.value)
						{
							actors[i].x -= player->x;
							actors[i].y -= player->y;
						}


						switch (actors[i].type) {

							case ACTOR_FLAMINGPARTICLE:
							case ACTOR_PARTICLE:
							case ACTOR_DEBRIS:
							case ACTOR_FLAMES:

								// Particles and debris MUST have LifeEndTime defined
								// or they won't make it past this line
								if (actors[i].LifeEndTime <= g_fNow)
								{
									actors[i].active = FALSE;
								}
								else
								{
									actors[i].x += (float)CosLookup(actors[i].rz) * (actors[i].vx + 2.0f) * fDeltaTime;
									actors[i].y += (float)SinLookup(actors[i].rz) * (actors[i].vy + 2.0f) * fDeltaTime;
									actors[i].rx += 100.0f * fDeltaTime;
									actors[i].ry += 100.0f * fDeltaTime;
								}

								// Debris can have smoke trails
								if (actors[i].type == ACTOR_DEBRIS)
								{
									
									// Spawn sparkling debris - 
									//		This is new code.  The old code was 
									//		written that at each cycle there was a 
									//		5% chance that the debris would spawn
									//		a particle.  Well now that the cycle runs
									//		10,000 times a frame, that was happening
									//		far more frequently than intended.  The
									//		new method spawns new debris after a 
									//		specified interval.  This is a concept that
									//		I was going to implement in my next game,
									//		but this was a good place to try it out.
									//		It's different from the rest of the timing
									//		code in the game in that it doesn't rely
									//		on g_fNow.  Implementing the during-game
									//		pause was difficult becuase every timing
									//		variable relied on g_fNow + some interval,
									//		and when the game pauses, g_fNow must be
									//		frozen (see pause code for more info).  
									//		Trust me, this is better ;)
									actors[i].ParticleSpawnDelay -= fDeltaTime;
									if (actors[i].ParticleSpawnDelay <= 0)
									{
										k = FindFreeActor(IDX_FIRST_PARTICLE, IDX_LAST_PARTICLE);
										if (k)
										{
											actors[k]			= actors[i];
											actors[k].active	= TRUE;
											actors[k].displaylistid = smoke_list;
											actors[k].type		= ACTOR_FLAMINGPARTICLE;
											actors[k].vx		*= 0.5;
											actors[k].vy		*= 0.5;
											
											actors[k].color.r	= randInt(48, 128);
											actors[k].color.g	= randInt(48, 128);
											actors[k].color.b	= 64;

											actors[k].LifeStartTime = g_fNow;
											actors[k].LifeEndTime	= g_fNow + randFloat(0.100f, 0.150f); // randFloat(0.050f, 0.100f);
										}

										// Reset spawn delay
										actors[i].ParticleSpawnDelay = randFloat(0.01f, 0.04f);
									}
								}

								break;

							case ACTOR_LSAUCER:
								
								// Set flag to indicate that at least one 
								// enemy is alive
								bAllEnemiesInactive = FALSE;

								// Change saucer direction periodically
								if (actors[i].ChangeDirectionDelay <= g_fNow)
								{
									actors[i].ChangeDirectionDelay = g_fNow + randFloat(0.500f, 1.500f);
									actors[i].vy = -actors[i].vy;
								}

								// Update saucer position
								actors[i].x += actors[i].vx * fDeltaTime;
								actors[i].y += actors[i].vy * fDeltaTime;
								actors[i].rz += 500.0f * fDeltaTime;

								// Spawn shots in even intervals
								if (actors[i].ShotDelay <= g_fNow)
								{
									
									// Setup next shot
									actors[i].ShotDelay = g_fNow + 1;

									// Spawn shot

									// Find free actor
									j = FindFreeActor(IDX_FIRST_SFIRE, IDX_LAST_SFIRE);

									// A free shot space is available
									if (j) {
										actors[j].active = TRUE;
										actors[j].type	= ACTOR_SFIRE;
										actors[j].displaylistid = sfire_list;
										actors[j].x		= actors[i].x;
										actors[j].y		= actors[i].y;
										actors[j].z		= actors[i].z;
										actors[j].rx	= actors[i].rx;
										actors[j].ry	= actors[i].ry;
										actors[j].rz	= randFloat(0, 360);
										
										// Set saucer's shot speed based on difficulty
										if (g_nDifficulty == EASY_DIFFICULTY)
										{
											actors[j].vx	= 15.0f;
											actors[j].vy	= 15.0f;
											actors[j].vz	= 0.1f;
										}
										else
										{
											actors[j].vx	= 20.1f;
											actors[j].vy	= 20.1f;
											actors[j].vz	= 0.1f;
										}
										actors[j].LifeEndTime = g_fNow + 0.500f;

										actors[j].xcolldist = 0.2f;
										actors[j].ycolldist = 0.2f;

										// Play fire sound
										if (g_cvSoundEnabled.value)
										{
											// DSUtil_PlaySound(g_pSaucerFireSound, 0);
											DSUtil_PlayPannedSound(g_pSaucerFireSound, actors[i].x / WORLD_HALFWIDTH * cStereoSeperation);
											
										}
									}
								}

								// If the saucer goes off screen, kill it
								if ((actors[i].x < -WORLD_HALFWIDTH) || (actors[i].x >  WORLD_HALFWIDTH))
								{
									
									// Kill the current saucer
									DestroySaucer(i);

									// Setup next saucer delay
									fTimeForNewLSaucer = g_fNow + randFloat(2.5f, 5.0f);

								}

								break;


							case ACTOR_ROCK:

								// Set flag to indicate that at least one
								// rock is still active ;)
								bAllEnemiesInactive = FALSE;

								// Update rock position
								actors[i].x += (float)CosLookup(actors[i].rz) * actors[i].vx * fDeltaTime;
								actors[i].y += (float)SinLookup(actors[i].rz) * actors[i].vy * fDeltaTime;
								actors[i].rx += 100.0f * fDeltaTime;
								actors[i].ry += 100.0f * fDeltaTime;

								if (actors[i].size < 0) 
									actors[i].active = FALSE;
								break;


							case ACTOR_SFIRE:
							case ACTOR_SHOT:
								if (actors[i].LifeEndTime <= g_fNow)
								{
									actors[i].active = FALSE;
								}
								else 
								{
									actors[i].x += (float)CosLookup(actors[i].rz) * actors[i].vx * fDeltaTime;
									actors[i].y += (float)SinLookup(actors[i].rz) * actors[i].vy * fDeltaTime;
									actors[i].rx += 1000.0f * fDeltaTime;

								}
								break;

							
							case ACTOR_SHIELDS:
								if ((player->ShieldsUp) && (player->ShieldsPower > 0)) {
									actors[i].active = TRUE;
									actors[i].x = player->x;
									actors[i].y = player->y;
									actors[i].z = player->z;
									actors[i].color.r = 255 - (int)player->ShieldsPower;
									actors[i].color.g = (int)player->ShieldsPower;
									actors[i].color.b = 0;
									actors[i].rx += 100.0f * fDeltaTime;
									actors[i].rz += 100.0f * fDeltaTime;
									
									if (!g_cvInfiniteShields.value)
										player->ShieldsPower -= 100.0f * fDeltaTime;


								}
								else {
									actors[i].active = FALSE;
								}

								break;

							case ACTOR_BLAST:
								if (actors[i].LifeEndTime <= g_fNow)
								{
									actors[i].active = FALSE;
								}
								break;

							default:
								break;
						}
						

						// Screen wrap 
						//
						// Don't wrap DEBRIS or PARTICLES
						if ((actors[i].type == ACTOR_DEBRIS)  || (actors[i].type == ACTOR_PARTICLE))
						{
							if ((actors[i].x > SCREEN_HALFWIDTH) || 
								(actors[i].x < -SCREEN_HALFWIDTH) ||
								(actors[i].y >  SCREEN_HALFHEIGHT) ||
								(actors[i].y < -SCREEN_HALFHEIGHT))
									actors[i].active = FALSE;
						}
						// Wrap everything else
						else 
						{
							if (actors[i].x > WORLD_HALFWIDTH) 
								actors[i].x -= WORLD_WIDTH;
							else if (actors[i].x < -WORLD_HALFWIDTH)
								actors[i].x += WORLD_WIDTH;
							if (actors[i].y > WORLD_HALFHEIGHT)
								actors[i].y -= WORLD_HEIGHT;
							else if (actors[i].y < -WORLD_HALFHEIGHT)
								actors[i].y += WORLD_HEIGHT;
						}
					}
				} // for (i = 0; i < MAX_ACTORS; i++)


				// Player is center of universe
				// if (g_bPreCopernicusMode)
				if (g_cvCopernicusMode.value)
				{
					
					// Move the background's x and y
					// based on the player's position
					g_fBackgroundX -= player->x;
					g_fBackgroundY -= player->y;
					
					// Update the stars only if 
					// they're visible
					if (g_bStarsActive) 
					{
						for (i = 0; i < NUM_STARS; i++)
						{
							// Move stars
							stars[i].x -= player->x;
							stars[i].y -= player->y;

							// Wrap stars at star's world border
							if (stars[i].x > STAR_WORLD_HALFWIDTH)
								stars[i].x -= STAR_WORLD_WIDTH;
							if (stars[i].x < -STAR_WORLD_HALFWIDTH)
								stars[i].x += STAR_WORLD_WIDTH;
							if (stars[i].y > STAR_WORLD_HALFHEIGHT)
								stars[i].y -= STAR_WORLD_HEIGHT;
							if (stars[i].y < -STAR_WORLD_HALFHEIGHT)
								stars[i].y += STAR_WORLD_HEIGHT;
						}
					}
				
					// Set player at center of screen
					player->x = 0;
					player->y = 0;
				}

				// Check for player collisions between the various
				// objects in the game that can collide
				CollisionDetection(&actors[IDX_SHIELDS], IDX_FIRST_ROCK, IDX_LAST_ROCK);
				CollisionDetection(player, IDX_FIRST_ROCK, IDX_LAST_ROCK);
				CollisionDetection(player, IDX_FIRST_SFIRE, IDX_LAST_SFIRE);
				CollisionDetection(player, IDX_LSAUCER, IDX_LSAUCER);
				CollisionDetection(&actors[IDX_LSAUCER], IDX_FIRST_SHOT, IDX_LAST_SHOT);
				for (i = IDX_FIRST_ROCK; i<= IDX_LAST_ROCK; i++)
				{
					CollisionDetection(&actors[i], IDX_FIRST_SHOT, IDX_LAST_SHOT);
					CollisionDetection(&actors[i], IDX_FIRST_SFIRE, IDX_LAST_SFIRE);
					CollisionDetection(&actors[i], IDX_LSAUCER, IDX_LSAUCER);
				}


				// Respawn player after death
				if ((player->active == FALSE) && (!player->Hyperspacing)) {

					if (fPlayerRespawnTime == 0) {

						// Decrease lives
						if (!g_cvInfiniteLives.value)
							player->Lives--;

						// Delay 2 seconds before respawning player
						fPlayerRespawnTime = g_fNow + 2;

						// Stop thumping
						fNextThumpTime = 2000000000;

						// Stop thruster sound
						if (g_cvSoundEnabled.value)
						{
							DSUtil_StopSound(g_pShipThrustSound);
						}
						g_bThrustSoundOn = FALSE;

						// If the player's out of lives,
						// game's over, man
						if (player->Lives <= 0)
						{
							// Set game over flag
							g_bGameOver = TRUE;

							// Stop looping sounds
							KillLoopingSounds();

							// Reset start game flag
							g_bStartGame = FALSE;

							// Check to see if current score is a high score
							for (i = 0; i < MAX_HIGHSCORES; i++)
							{
								if (player->Score > highscores[i].Score)
								{

									// Push all the other high scores
									// down one rank and elim the last
									for (j = MAX_HIGHSCORES - 2; j >= i; j--)
										highscores[j + 1] = highscores[j];

									// Found one!  Initialize variables and exit for loop
									g_bEnteringHighScoreInitials = TRUE;
									g_nHighScoreInitialsIdx = 0;

									highscores[i].Score = player->Score;
									highscores[i].Level = g_nLevelNum;
									
									switch(nSpaceSize) {
									case 0:	
										highscores[i].SpaceSize = 'S';
										break;
									case 1:
										highscores[i].SpaceSize = 'M';
										break;
									case 2:
										highscores[i].SpaceSize = 'L';
										break;
									}

									switch(g_nDifficulty) {
									case 0:
										highscores[i].Difficulty = 'E';
										break;
									case 1:
										highscores[i].Difficulty = 'N';
										break;
									case 2:
										highscores[i].Difficulty = 'H';
										break;
									}

									sprintf(highscores[i].Initials, "   ");

									playerhighscore = &highscores[i];

									break;
								}
							}
						}
					}

					if ((fPlayerRespawnTime != 0) && (fPlayerRespawnTime < g_fNow) && (player->Lives > 0)) {

						// This function resets player at center
						// of screen as long as it doesn't spell
						// instant death.
						if (SpawnPlayer())
						{
							// Restart thumpss
							bPlayThump = !bPlayThump;
							fNextThumpTime = 1;

							// Reset Timeforplayerrespawn for next time player dies
							fPlayerRespawnTime = 0;

							// Reset saucer next time
							fTimeForNewLSaucer = 0;
						}					

					}

				}

				// Setup and respawn saucer
				if (!g_bGameOver)
				{

					// Reset timeout if it's zero
					if ((!fTimeForNewLSaucer) && (!actors[IDX_LSAUCER].active))
					{
						fTimeForNewLSaucer = g_fNow + randFloat(2.5f, 5.0f);
					}

					// On easy, the saucer doesn't spawn until level five
					if (!((g_nDifficulty == EASY_DIFFICULTY) && (g_nLevelNum < 5)))
					{
						// If the interval between saucers has passed, then 
						// spawn the saucer
						if ((fTimeForNewLSaucer) && (fTimeForNewLSaucer < g_fNow))
						{
							if (actors[IDX_LSAUCER].active == FALSE) 
							{
								if (CreateSaucer(IDX_LSAUCER))
									fTimeForNewLSaucer = 0;
							}
						}
					}
					else
						fTimeForNewLSaucer = 0;
				}
				

				// Spawn rocks with random positions and velocities
				// 
  				// In the future, this should happen after a delay
				// instead of right away
				if (bAllEnemiesInactive) {

					// When the board's done, set the delay to 
					// repopulate board and increment the board
					// number
					if (fTimeForNewRocks == 0) {
						
						fTimeForNewRocks = g_fNow + 2;

						// Stop thumping between levels
						fNextThumpTime = 2000000000;

						// Stop UFOs between levels
						fTimeForNewLSaucer = 2000000000;

					}
					
					// Check time...
					if ((fTimeForNewRocks != 0) && (fTimeForNewRocks <= g_fNow)) {

						// Increment the level number
						g_nLevelNum++;

						// Reset player's shields on easy difficulty
						if (g_nDifficulty == EASY_DIFFICULTY)
						{
							player->ShieldsPower = 255;
						}
						// Recharge them 20% of the level number on 
						// normal and hard difficulties
						else
						{
							// Recharge shields 20% of level number
							if (g_nLevelNum < 100)
							{
								float fPct = g_nLevelNum * 0.02f;
								i = int(255 * fPct);
								// UpdateMessage("Recharging %f%% == %d", fPct, i);
								if (player->ShieldsPower + i > 255)
								{
									player->ShieldsPower = 255;
								}
								else
								{
									player->ShieldsPower += i;
								}
							}
							else
							{
								player->ShieldsPower = 255;
							}
						}


						// Reset timefornewrocks for next time...
						fTimeForNewRocks = 0;

						// Set lLevelStartTime as g_lNow
						fLevelStartTime = g_fNow;

						// Reset thumping time
						fNextThumpTime = 0;

						// Reset UFO
						fTimeForNewLSaucer = g_fNow + randFloat(2.5f, 5.0f); 

						// Spawn rocks
						
						// Compute number of rocks to use for current level
						// and difficulty
						int ubound = 0;
						if (g_nDifficulty == HARD_DIFFICULTY)
						{
							ubound = IDX_FIRST_ROCK  + ((g_nLevelNum + 2) * 2);
						}
						else
						{
							ubound = IDX_FIRST_ROCK  + g_nLevelNum + 2;
						}
						
						if (ubound > IDX_LAST_ROCK)
						{
							ubound = IDX_LAST_ROCK;
						}

						for (i = IDX_FIRST_ROCK; i < ubound; i++)
						{
							actors[i].active = TRUE;
							actors[i].type = ACTOR_ROCK;

							// This will stick rocks close to the 
							// edges of the screen (in the x direction)
							actors[i].x = randFloat(0.0f, 2.0f + g_nLevelNum) - 1.0f - (g_nLevelNum * 0.5f) - WORLD_HALFWIDTH;
							actors[i].y = randFloat(0.0f, (float)WORLD_HEIGHT);
							actors[i].z = WORLD_DEPTH;
							
							actors[i].rx = randFloat(-360.0f, 360.0f);
							actors[i].ry = randFloat(-360.0f, 360.0f);
							actors[i].rz = randFloat(-360.0f, 360.0f);

							
							// Set rocks' speed based on difficulty
							switch (g_nDifficulty)
							{
							case EASY_DIFFICULTY:
								actors[i].vx = 1.5f; 
								actors[i].vy = 1.5f; 
								actors[i].vz = 1.5f; 
								break;

							case NORMAL_DIFFICULTY:
								actors[i].vx = 1.5f + 0.025f * g_nLevelNum; 
								actors[i].vy = 1.5f + 0.025f * g_nLevelNum; 
								actors[i].vz = 1.5f + 0.025f * g_nLevelNum; 
								break;

							case HARD_DIFFICULTY:
								actors[i].vx = 2.0f + 0.025f * g_nLevelNum;
								actors[i].vy = 2.0f + 0.025f * g_nLevelNum;
								actors[i].vz = 2.0f + 0.025f * g_nLevelNum;
								break;
							}

							// Set color of rock
							actors[i].color.r = randInt(96, 160);
							actors[i].color.g = randInt(96, 160);
							actors[i].color.b = randInt(96, 160);


							// Make sure rock isn't too dark
							AdjustRockColor(i);

							actors[i].displaylistid = rock_list;
							actors[i].size = 2;

							actors[i].xcolldist = 1.1f;
							actors[i].ycolldist = 1.1f;
						}
					}
				} // if (bAllEnemiesInactive) 

				// Update the background rotation (even during pause)
				g_fBackgroundRot += 0.5f * fDeltaTime;

			} // if (!g_bGamePaused)

		} // Message loop end

	} // !g_bExitApp


	
	//------------------------------------------------------
	// Shutdown
	//------------------------------------------------------

	// Save high score records
	SaveHighScores("disasteroids3d.dat");

	// Save Cvars
	Cvar_SaveAll(csConfigFilename);

	// Kill data, sounds, control, window
	TerminateGame();


	// Exit The Program
	return (msg.wParam);

}

