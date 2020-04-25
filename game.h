#ifndef GAME_H
#define GAME_H

#include <windows.h>			// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <math.h>				// Header File For Math Library
#include <GL\gl.h>			// Header File For The OpenGL32 Library
#include <GL\glu.h>			// Header File For The GLu32 Library
#include "glext.h"

#include "3dglasses.h"
#include "renderflags.h"
#include "cvars.h"
#include "menus.h"
#include "tokenize.h"
#include "timer.h"
#include "mp3control.h"
#include "console.h"
#include "command.h"
#include "commands.h"

	extern HWND		hWnd;

	void AdjustRockColor(const int&);

	extern BYTE keys[];
	extern BYTE lastkeys[];

	extern BOOL g_bExitApp;
	extern BOOL g_bShowMenu;
	extern BOOL g_bStartGame;
	extern BOOL g_bCheatsEnabled;

	extern float g_fNow;

	extern cvar_t g_cvResolution;
	extern cvar_t g_cvColorDepth;
	extern cvar_t g_cvGammaGamma;
	extern cvar_t g_cvGammaBrightness;
	extern cvar_t g_cvGammaContrast;

	extern cvar_t g_cvFullscreen;
	extern cvar_t g_cvCopernicusMode;
	extern cvar_t g_cvSpaceMode;
	extern cvar_t g_cvDifficulty;
	extern cvar_t g_cvSpaceSize;
	
	extern cvar_t g_cvInfiniteLives;
	extern cvar_t g_cvInfiniteShields;
	extern cvar_t g_cvInvincibility;

	extern cvar_t g_cvDeveloper;
	extern cvar_t g_cvLightingEnabled;
	extern cvar_t g_cvBlendingEnabled;
	extern cvar_t g_cvTexturesEnabled;
	extern cvar_t g_cvWireframeEnabled;
	extern cvar_t g_cvBoundingBoxesEnabled;
	extern cvar_t g_cvFpsEnabled;
	extern cvar_t g_cvVsyncEnabled;

	BOOL SetVsync(const BOOL);

	extern cvar_t g_cvJoystickEnabled;
	extern cvar_t g_cvForceFeedbackEnabled;
	extern cvar_t g_cvJoystickDeadzone;
	extern BOOL g_bDirectInputEnabled;
	extern BOOL g_bForceFeedbackCapable;
	
	extern cvar_t g_cvSoundEnabled;
	extern cvar_t g_cvVolume;

	extern cvar_t g_cvShipRotateAdjust;
	
	extern cvar_t g_cvMouseSensitivity;
	extern cvar_t g_cvMouseEnabled;

	void SetSound(const BOOL);
	void SetMouse(const BOOL);
	void SetVolume(float NewValue);
	void SetGamma(void);
	
	void UnacquireMouse();
	
	#define KeyDown(x) ((keys[x] && !lastkeys[x]) && keys[x])
	#define KeyUp(x) ((keys[x] && !lastkeys[x]) && !keys[x])
	
	extern const int NUM_KEY_BINDS;
	extern char g_asKeyBindsText[][40];
	extern cvar_t *g_acvKeyBinds[];

	// Function definitions
	GLvoid glPrint(int, const GLfloat&, const GLfloat&, const char*);
	GLvoid glPrintVar(int, const GLfloat&, const GLfloat&, const char*, ...);
	GLvoid glPrintCentered(int, const GLfloat&, const char*);
	GLvoid glPrintCenteredVar(int, const GLfloat&, const char*, ...);
	void UpdateMessage(const char*, ...);
	void PauseGame(const BOOL&);
	BOOL InitializeGame(void);
	void TerminateGame(void);
	
	float GetRefreshTimingRate(void);

	// Sound stuff (for menus)
	void PlayMenuBeepSound();
	void PlayMenuLowSound();
	void PlayMenuExplosionSound();

	// 3d glasses stuff
	extern cvar_t g_cv3dglasses;
	int GetScreenWidth(void);
	int GetScreenHeight(void);

#endif

