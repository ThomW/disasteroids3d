
#include "menus.h"
#include "game.h"

const int EXIT_LISTINDEX = 9999;

const	int MENU_PREVMENU					= -1;
const int MENU_BLANK						= 0;

const int MENU_MAIN						= 100;
const int MENU_STARTGAME				= 101;
const int MENU_QUITGAME					= 199;

const int MENU_SETTINGS					= 200;

const int MENU_CONTROLLER				= 300;
const int MENU_KEYBINDS					= 301;
const int MENU_JOYSTICKTOGGLE			= 302;
const int MENU_FORCEFEEDBACKTOGGLE	= 303;
const int MENU_ROTATE_ADJUST			= 304;
const int MENU_MOUSE_ENABLED			= 305;
const int MENU_MOUSE_ADJUST			= 306;
const int MENU_JOYSTICKDEADZONE		= 307;

const int MENU_SOUND						= 350;
const int MENU_SOUNDTOGGLE				= 351;
const int MENU_VOLUME					= 352;

const int MENU_DISPLAY					= 400;

const int MENU_DEVELOPER				= 500;
const int MENU_LIGHTINGTOGGLE			= 501;
const int MENU_BLENDINGTOGGLE			= 502;
const int MENU_TEXTURETOGGLE			= 503;
const int MENU_WIREFRAMETOGGLE		= 504;
const int MENU_BOUNDINGBOXTOGGLE		= 505;
const int MENU_SHOWFPSTOGGLE			= 506;

const int MENU_GAMESETTINGS			= 600;
const int MENU_SPACE						= 601;
const int MENU_DIFFICULTY				= 602;

const int MENU_COPERNICUSMODE			= 650;
const int MENU_SPACEMOTIONMENU		= 651;

const int MENU_DIFFICULTY1				= 675;
const int MENU_DIFFICULTY2				= 676;
const int MENU_DIFFICULTY3				= 677;

const int MENU_CHANGERESOLUTION		= 700;
const int MENU_RESOLUTION640X480		= 701;
const int MENU_RESOLUTION800X600		= 702;
const int MENU_RESOLUTION1024X768	= 703;
const int MENU_RESOLUTION1280X1024	= 704;
const int MENU_RESOULTION1600X1200	= 705;

const int MENU_CHANGECOLORDEPTH		= 710;
const int MENU_COLORDEPTH16			= 711;
const int MENU_COLORDEPTH32			= 712;

const int MENU_CHANGEGAMMA				= 720;
const int MENU_GAMMAGAMMA				= 721;
const int MENU_GAMMABRIGHTNESS		= 722;
const int MENU_GAMMACONTRAST			= 723;

const int MENU_TOGGLEFULLSCREEN		= 750;
const int MENU_TOGGLEVSYNC				= 751;
const int MENU_TOGGLE3D					= 752;

const int MENU_SPACEMODE				= 800;
const int MENU_SPACEMODE1				= 801;
const int MENU_SPACEMODE2				= 802;
const int MENU_SPACEMODE3				= 803;

const int MENU_SPACESIZE				= 900;
const int MENU_SPACESIZE1				= 901;
const int MENU_SPACESIZE2				= 902;
const int MENU_SPACESIZE3				= 903;


// Constants used to define the menu item type
const int MNUI_BUTTON			= 0;
const int MNUI_TOGGLE			= 1;
const int MNUI_OPTION			= 2;
const int MNUI_SLIDER			= 4;
const int MNUI_CONTROL_BITS	= 63;
const int MNUI_DISABLED			= 64;
const int MNUI_REVERSE_LOGIC	= 128;

int nSelectedItem = -1;


// Define menu data structures
struct udtMenuItems {
	char			Caption[25];
	int			ID;
	int			Type;
	cvar_t		*Cvar;
};
struct udtMenuDef {
	int	MenuID; 
	char	Caption[25];
	int	ListCount;
	udtMenuItems *MenuItems;
	cvar_t	*pListSelect;
	udtMenuDef* PrevMenu;
	int	ListItem;
};


udtMenuItems miMainMenu[] = {
	{"START GAME",			MENU_STARTGAME,		MNUI_BUTTON},
	{"SETTINGS",			MENU_SETTINGS,			MNUI_BUTTON},
	{"",						MENU_BLANK,				MNUI_BUTTON},		
	{"QUIT GAME",			MENU_QUITGAME,			MNUI_BUTTON}
};
udtMenuDef mdMainMenu= {
	MENU_MAIN,
	"MAIN MENU",
	sizeof(miMainMenu) / sizeof(miMainMenu[0]),
	&miMainMenu[0]
};


udtMenuItems miSettingsMenu[] = {
	{"GAME",					MENU_GAMESETTINGS,	MNUI_BUTTON},
	{"DISPLAY",				MENU_DISPLAY,			MNUI_BUTTON},
	{"SOUND",				MENU_SOUND,				MNUI_BUTTON},
	{"CONTROLLER",			MENU_CONTROLLER,		MNUI_BUTTON},
	{"DEVELOPER",			MENU_DEVELOPER,		MNUI_BUTTON},
	{"",						MENU_BLANK,				MNUI_BUTTON},		
	{"BACK",					MENU_PREVMENU,			MNUI_BUTTON}	
};
udtMenuDef mdSettingsMenu = {
	MENU_SETTINGS,
	"SETTINGS MENU",
	sizeof(miSettingsMenu) / sizeof(miSettingsMenu[0]),
	&miSettingsMenu[0]
};


udtMenuItems miSoundMenu[] = {
	{"SOUND",				MENU_SOUNDTOGGLE,		MNUI_TOGGLE, &g_cvSoundEnabled},
	{"VOLUME",				MENU_VOLUME,			MNUI_SLIDER, &g_cvVolume},
	{"",						MENU_BLANK,				MNUI_BUTTON},		
	{"BACK",					MENU_PREVMENU,			MNUI_BUTTON}	
};
udtMenuDef mdSoundMenu = {
	MENU_SOUND,
	"SOUND MENU",
	sizeof(miSoundMenu) / sizeof(miSoundMenu[0]),
	&miSoundMenu[0]
};	


udtMenuItems miSpaceMenu[] = {
	{"SPACE SIZE",			MENU_SPACESIZE,		MNUI_BUTTON},
	{"SPACE MOTION",		MENU_SPACEMODE,		MNUI_BUTTON},
	{"COPERNICUS MODE",	MENU_COPERNICUSMODE,	MNUI_TOGGLE, &g_cvCopernicusMode},
	{"",						MENU_BLANK,				MNUI_BUTTON},		
	{"BACK",					MENU_PREVMENU,			MNUI_BUTTON}	
};
udtMenuDef mdSpaceMenu = {
	MENU_SPACE,
	"SPACE MENU",
	sizeof(miSpaceMenu) / sizeof(miSpaceMenu[0]),
	&miSpaceMenu[0]
};


udtMenuItems miDeveloperMenu[] = {
	{"LIGHTING",		MENU_LIGHTINGTOGGLE,		MNUI_TOGGLE, &g_cvLightingEnabled},
	{"BLENDING",		MENU_BLENDINGTOGGLE,		MNUI_TOGGLE, &g_cvBlendingEnabled},
	{"TEXTURES",		MENU_TEXTURETOGGLE,		MNUI_TOGGLE, &g_cvTexturesEnabled},
	{"WIREFRAME",		MENU_WIREFRAMETOGGLE,	MNUI_TOGGLE, &g_cvWireframeEnabled},
	{"BOUNDING BOXES",MENU_BOUNDINGBOXTOGGLE,	MNUI_TOGGLE, &g_cvBoundingBoxesEnabled},
	{"FRAME RATE",		MENU_SHOWFPSTOGGLE,		MNUI_TOGGLE, &g_cvFpsEnabled},
	{"",					MENU_BLANK,					MNUI_BUTTON},
	{"BACK",				MENU_PREVMENU,				MNUI_BUTTON}
};
udtMenuDef mdDeveloperMenu = {
	MENU_DEVELOPER,
	"DEVELOPER SETTINGS",
	sizeof(miDeveloperMenu) / sizeof(miDeveloperMenu[0]),
	&miDeveloperMenu[0]
};


udtMenuItems miDisplayMenu[] = {
	{"CHANGE RESOLUTION",		MENU_CHANGERESOLUTION,	MNUI_BUTTON},
	{"CHANGE COLOR DEPTH",		MENU_CHANGECOLORDEPTH,	MNUI_BUTTON},
	{"CHANGE GAMMA SETTINGS",	MENU_CHANGEGAMMA,			MNUI_BUTTON},
	{"TOGGLE FULLSCREEN",		MENU_TOGGLEFULLSCREEN,	MNUI_BUTTON},
	{"VSYNC",						MENU_TOGGLEVSYNC,			MNUI_TOGGLE, &g_cvVsyncEnabled},
	{"",								MENU_BLANK,					MNUI_BUTTON},
//	{"3D RED/BLUE",				MENU_TOGGLE3D,				MNUI_TOGGLE, &g_cv3dglasses},
//	{"",								MENU_BLANK,					MNUI_BUTTON},
	{"BACK",							MENU_PREVMENU,				MNUI_BUTTON}
};
udtMenuDef mdDisplayMenu = {
	MENU_DISPLAY,
	"DISPLAY OPTIONS",
	sizeof(miDisplayMenu) / sizeof(miDisplayMenu[0]),
	&miDisplayMenu[0]
};


udtMenuItems miResolutionMenu[] = {
	{"640 X 480",	MENU_RESOLUTION640X480,		MNUI_OPTION},
	{"800 X 600",	MENU_RESOLUTION800X600,		MNUI_OPTION},
	{"1024 X 768",	MENU_RESOLUTION1024X768,	MNUI_OPTION},
	
	/*
	The next two options are remmed out to prevent people from
	potentially damaging their monitors by selecting video modes
	outside the range of their hardware.
	
	If you have the hardware to support it and the desire to see 
	the game at these ungodly resolutions, feel free to unrem
	the next two lines.  
	*/
	// {"1280 X 1024",MENU_RESOLUTION1280X1024,	MNUI_OPTION},
	// {"1600 X 1200",MENU_RESOLUTION1600X1200,	MNUI_OPTION},

	{"",				MENU_BLANK,						MNUI_BUTTON},
	{"BACK",			MENU_PREVMENU,					MNUI_BUTTON}
};
udtMenuDef mdResolutionMenu = {
	MENU_CHANGERESOLUTION,
	"DISPLAY RESOLUTION",
	sizeof(miResolutionMenu) / sizeof(miResolutionMenu[0]),
	&miResolutionMenu[0],
	&g_cvResolution
};


udtMenuItems miColorDepthMenu[] = {
	{"16 BIT COLOR",	MENU_COLORDEPTH16,		MNUI_OPTION},
	{"32 BIT COLOR",	MENU_COLORDEPTH32,		MNUI_OPTION},
	{"",					MENU_BLANK,					MNUI_BUTTON},
	{"BACK",				MENU_PREVMENU,				MNUI_BUTTON}
};
udtMenuDef mdColorDepthMenu = {
	MENU_CHANGECOLORDEPTH,
	"DISPLAY COLOR DEPTH",
	sizeof(miColorDepthMenu) / sizeof(miColorDepthMenu[0]),
	&miColorDepthMenu[0],
	&g_cvColorDepth
};


udtMenuItems miGammaMenu[] = {
	{"GAMMA",				MENU_GAMMAGAMMA,			MNUI_SLIDER, &g_cvGammaGamma},
	{"BRIGHTNESS",			MENU_GAMMABRIGHTNESS,	MNUI_SLIDER, &g_cvGammaBrightness},
	{"CONTRAST",			MENU_GAMMACONTRAST,		MNUI_SLIDER, &g_cvGammaContrast},
	{"",						MENU_BLANK,				MNUI_BUTTON},		
	{"BACK",					MENU_PREVMENU,			MNUI_BUTTON}	
};
udtMenuDef mdGammaMenu = {
	MENU_CHANGEGAMMA,
	"GAMMA MENU",
	sizeof(miGammaMenu) / sizeof(miGammaMenu[0]),
	&miGammaMenu[0]
};	


udtMenuItems miControllerMenu[] = {
	{"JOYSTICK",		MENU_JOYSTICKTOGGLE,			MNUI_TOGGLE, &g_cvJoystickEnabled},
	{"FORCE-FEEDBACK",MENU_FORCEFEEDBACKTOGGLE,	MNUI_TOGGLE, &g_cvForceFeedbackEnabled},
//	{"DEADZONE",		MENU_JOYSTICKDEADZONE,		MNUI_SLIDER, &g_cvJoystickDeadzone},
	{"",					MENU_BLANK,						MNUI_BUTTON},
	{"ROTATE SPEED",	MENU_ROTATE_ADJUST,			MNUI_SLIDER, &g_cvShipRotateAdjust},
	{"",					MENU_BLANK,						MNUI_BUTTON},
	{"MOUSE",			MENU_MOUSE_ENABLED,			MNUI_TOGGLE, &g_cvMouseEnabled},
	{"MOUSE RATE",		MENU_MOUSE_ADJUST,			MNUI_SLIDER, &g_cvMouseSensitivity},
	{"",					MENU_BLANK,						MNUI_BUTTON},
	{"ASSIGN KEYS",	MENU_KEYBINDS,					MNUI_BUTTON},
	{"",					MENU_BLANK,						MNUI_BUTTON},
	{"BACK",				MENU_PREVMENU,					MNUI_BUTTON}
};
udtMenuDef mdControllerMenu = {
	MENU_CONTROLLER,
	"CONTROLLER SETTINGS",
	sizeof(miControllerMenu) / sizeof(miControllerMenu[0]),
	&miControllerMenu[0]
};


udtMenuItems miGameSettingsMenu[] = {
	{"DIFFICULTY",		MENU_DIFFICULTY,				MNUI_BUTTON},
	{"SPACE",			MENU_SPACE,						MNUI_BUTTON},
	{"",					MENU_BLANK,						MNUI_BUTTON},
	{"BACK",				MENU_PREVMENU,					MNUI_BUTTON}
};

udtMenuDef mdGameSettingsMenu = {
	MENU_GAMESETTINGS,
	"GAME SETTINGS",
	sizeof(miGameSettingsMenu) / sizeof(miGameSettingsMenu[0]),
	&miGameSettingsMenu[0]
};


udtMenuItems miPreCopernicusSpaceModeMenu[] = {
	{" DEMANDING      ",	MENU_SPACEMODE1,		MNUI_OPTION},
	{" SPARKLING STARS",	MENU_SPACEMODE2,		MNUI_OPTION},
	{" STARS ONLY     ",	MENU_SPACEMODE3,		MNUI_OPTION},
	{"",						MENU_BLANK,				MNUI_BUTTON},
	{"BACK",					MENU_PREVMENU,			MNUI_BUTTON}
};
udtMenuDef mdPreCopernicusSpaceModeMenu = {
	MENU_SPACEMODE,
	"PRE-COPERNICUS SPACE",
	sizeof(miPreCopernicusSpaceModeMenu) / sizeof(miPreCopernicusSpaceModeMenu[0]),
	&miPreCopernicusSpaceModeMenu[0],
	&g_cvSpaceMode
};


udtMenuItems miPostCopernicusSpaceModeMenu[] = {
	{"STILL    ",		MENU_SPACEMODE1,		MNUI_OPTION},
	{"SPARKLING",		MENU_SPACEMODE2,		MNUI_OPTION},
	{"SICKENING",		MENU_SPACEMODE3,		MNUI_OPTION},
	{"",					MENU_BLANK,				MNUI_BUTTON},
	{"BACK",				MENU_PREVMENU,			MNUI_BUTTON}
};
udtMenuDef mdPostCopernicusSpaceModeMenu = {
	MENU_SPACEMODE,
	"POST-COPERNICUS SPACE",
	sizeof(miPostCopernicusSpaceModeMenu) / sizeof(miPostCopernicusSpaceModeMenu[0]),
	&miPostCopernicusSpaceModeMenu[0],
	&g_cvSpaceMode
};


udtMenuItems miSpaceSizeMenu[] = {
	{"TOO SMALL",		MENU_SPACESIZE1,		MNUI_OPTION},
	{"NORMAL",			MENU_SPACESIZE2,		MNUI_OPTION},
	{"LARGE",			MENU_SPACESIZE3,		MNUI_OPTION},
	{"",					MENU_BLANK,				MNUI_BUTTON},
	{"BACK",				MENU_PREVMENU,			MNUI_BUTTON}
};
udtMenuDef mdSpaceSizeMenu = {
	MENU_SPACESIZE,
	"SPACE SIZE",
	sizeof(miSpaceSizeMenu) / sizeof(miSpaceSizeMenu[0]),
	&miSpaceSizeMenu[0],
	&g_cvSpaceSize
};


udtMenuItems miDifficultyMenu[] = {
	{"EASY",					MENU_DIFFICULTY1,		MNUI_OPTION},
	{"NORMAL",				MENU_DIFFICULTY2,		MNUI_OPTION},
	{"HARD",					MENU_DIFFICULTY3,		MNUI_OPTION},
	{"",						MENU_BLANK,				MNUI_BUTTON},
	{"BACK",					MENU_PREVMENU,			MNUI_BUTTON}
};
udtMenuDef mdDifficultyMenu = {
	MENU_DIFFICULTY,
	"DIFFICULTY",
	sizeof(miDifficultyMenu) / sizeof(miDifficultyMenu[0]),
	&miDifficultyMenu[0],
	&g_cvDifficulty
};

const int MENU_CHEATS					= 1000;
const int MENU_INFINITESHIELDS		= 1001;
const int MENU_INFINITELIVES			= 1002;
const int MENU_INVINCIBILITY			= 1003;
udtMenuItems miCheatMenu[] = {
	{"SHIELDS",					MENU_INFINITESHIELDS,			MNUI_TOGGLE, &g_cvInfiniteShields},
	{"LIVES",					MENU_INFINITELIVES,				MNUI_TOGGLE, &g_cvInfiniteLives},
	{"INVINCIBLE",				MENU_INVINCIBILITY,				MNUI_TOGGLE, &g_cvInvincibility},
	{"",							MENU_BLANK,							MNUI_BUTTON},
	{"EXIT MENU (CHEATER)",	MENU_STARTGAME,					MNUI_BUTTON}
};
udtMenuDef mdCheatMenu = {
	MENU_CHEATS,
	"CHEAT MENU",
	sizeof(miCheatMenu) / sizeof(miCheatMenu[0]),
	&miCheatMenu[0],
};


udtMenuDef mdKeyBindsMenu = {
	MENU_KEYBINDS,
	"KEYBOARD SETUP",
	0,
	NULL
};

udtMenuDef *CurrentMenu = &mdMainMenu;


// This routine redirects the pointer, but also resets 
// the selected list item
void ShowMenu(udtMenuDef *Menu)
{

	// Store CurrentMenu as Menu's PrevMenu
	Menu->PrevMenu = CurrentMenu;

	// Set the current menu 
	CurrentMenu = Menu;

	// Set the selected list item
	if (CurrentMenu->pListSelect == NULL) 
		CurrentMenu->ListItem = 0;
	else
		CurrentMenu->ListItem = (int)CurrentMenu->pListSelect->value;

	// UpdateMessage("Menu = %s", CurrentMenu->Caption);
}

void ShowLastMenu()
{
	
	// Return to PrevMenu
	if (CurrentMenu->PrevMenu != NULL)
	{
		CurrentMenu = CurrentMenu->PrevMenu;
	}
	
	// Fall back to the main menu if PrevMenu is NULL
	else
	{
		CurrentMenu = &mdMainMenu;
		CurrentMenu->ListItem = 0;
	}

}



void Menu_Init()
{
	CurrentMenu = &mdMainMenu;
	ShowMenu(&mdMainMenu);
}



void Menu_ShowCheatMenu()
{
	CurrentMenu = &mdCheatMenu;
	ShowMenu(&mdCheatMenu);
}



bool Menu_Update()
{

	BOOL bChanged = FALSE;
	int i = 0;

	
	// Branch off to seperate routine handlers
	// where applicable
	if (CurrentMenu->MenuID == MENU_KEYBINDS)
	{
		KeyBindMenu_Update();
	}	
	else
	{

		// Change list item
		if (KeyDown(VK_UP))
		{
			CurrentMenu->ListItem--;
						
			// Skip blanks and disabled items
			while ((strlen(CurrentMenu->MenuItems[CurrentMenu->ListItem].Caption) == 0) || ((CurrentMenu->MenuItems[CurrentMenu->ListItem].Type & MNUI_DISABLED) == MNUI_DISABLED))
			{
				CurrentMenu->ListItem--;
				if (CurrentMenu->ListItem < 0) break;
			}

			// Play sound
			PlayMenuBeepSound();

		}
		if (KeyDown(VK_DOWN))
		{
			CurrentMenu->ListItem++;

			// Skip blanks and disabled items
			while ((strlen(CurrentMenu->MenuItems[CurrentMenu->ListItem].Caption) == 0) || ((CurrentMenu->MenuItems[CurrentMenu->ListItem].Type & MNUI_DISABLED) == MNUI_DISABLED))
			{
				CurrentMenu->ListItem++;
				if (CurrentMenu->ListItem > CurrentMenu->ListCount) break;
			}

			// Play sound
			PlayMenuBeepSound();

		}

		// Wrap menu
		if (CurrentMenu->ListItem < 0)
			CurrentMenu->ListItem = CurrentMenu->ListCount - 1;
		else if (CurrentMenu->ListItem >= CurrentMenu->ListCount)
			CurrentMenu->ListItem = 0;

		// ESC key backs up one menu level
		if (KeyDown(VK_ESCAPE))
		{
			// Top level menu -- exit 
			if (CurrentMenu->PrevMenu == CurrentMenu)
				return FALSE;
			else
			{
				PlayMenuLowSound();

				ShowLastMenu();

			}
		}


		// Handle 'slider' values
		if ((CurrentMenu->MenuItems[CurrentMenu->ListItem].Type & MNUI_CONTROL_BITS) == MNUI_SLIDER)
		{
			if (CurrentMenu->MenuItems[CurrentMenu->ListItem].Cvar != NULL)
			{
				if (KeyDown(VK_RIGHT))
				{
					PlayMenuBeepSound();
					if (CurrentMenu->MenuItems[CurrentMenu->ListItem].Cvar->value + 0.1f <= 1.0f)
					{
						Cvar_SetValue(CurrentMenu->MenuItems[CurrentMenu->ListItem].Cvar->name, CurrentMenu->MenuItems[CurrentMenu->ListItem].Cvar->value + 0.1f);
						bChanged = TRUE;
					}
				}
				if (KeyDown(VK_LEFT))
				{
					PlayMenuLowSound();
					if (CurrentMenu->MenuItems[CurrentMenu->ListItem].Cvar->value - 0.1f >= 0.0f)
					{
						Cvar_SetValue(CurrentMenu->MenuItems[CurrentMenu->ListItem].Cvar->name, CurrentMenu->MenuItems[CurrentMenu->ListItem].Cvar->value - 0.1f);
						bChanged = TRUE;
					}
				}
			}
		}

		if (bChanged)
		{
			switch (CurrentMenu->MenuItems[CurrentMenu->ListItem].ID)
			{
			case MENU_VOLUME:
				SetVolume(g_cvVolume.value);
				break;

			case MENU_GAMMAGAMMA:
			case MENU_GAMMABRIGHTNESS:
			case MENU_GAMMACONTRAST:
				SetGamma();
				break;
			}
			
		}
		bChanged = FALSE;



		// Select a list item when the ENTER key is hit
		if (KeyDown(VK_RETURN))
		{

			// Play sound
			PlayMenuBeepSound();

			// Do action based on selected menu item's
			// ID number
			switch (CurrentMenu->MenuItems[CurrentMenu->ListItem].ID)
			{

			case MENU_QUITGAME:
				g_bExitApp = TRUE;
				break;

			case MENU_PREVMENU:
				ShowLastMenu();
				break;

			case MENU_MAIN:	
				ShowMenu(&mdMainMenu);
				break;

			case MENU_STARTGAME:
				g_bStartGame = TRUE;
				g_bShowMenu = FALSE;
				PauseGame(FALSE);
				break;

			case MENU_SETTINGS:	
				ShowMenu(&mdSettingsMenu);
				break;

			case MENU_GAMESETTINGS:
				ShowMenu(&mdGameSettingsMenu);
				break;

			case MENU_SPACE:
				ShowMenu(&mdSpaceMenu);
				break;

			case MENU_DIFFICULTY:
				ShowMenu(&mdDifficultyMenu);
				break;

			case MENU_DIFFICULTY1:
				Cvar_SetValue("Difficulty", 0);
				break;

			case MENU_DIFFICULTY2:
				Cvar_SetValue("Difficulty", 1);
				break;
			
			case MENU_DIFFICULTY3:
				Cvar_SetValue("Difficulty", 2);
				break;
			
			case MENU_SPACEMODE:
				if (g_cvCopernicusMode.value)
					ShowMenu(&mdPreCopernicusSpaceModeMenu);
				else
					ShowMenu(&mdPostCopernicusSpaceModeMenu);
					
				break;

			case MENU_SPACEMODE1:
				Cvar_SetValue("SpaceMode", 0);
				break;

			case MENU_SPACEMODE2:
				Cvar_SetValue("SpaceMode", 1);
				break;

			case MENU_SPACEMODE3:
				Cvar_SetValue("SpaceMode", 2);
				break;

			case MENU_SPACESIZE:
				ShowMenu(&mdSpaceSizeMenu);
				break;

			case MENU_SPACESIZE1:
				Cvar_SetValue("SpaceSize", 0);
				break;

			case MENU_SPACESIZE2:
				Cvar_SetValue("SpaceSize", 1);
				break;

			case MENU_SPACESIZE3:
				Cvar_SetValue("SpaceSize", 2);
				break;

			case MENU_DISPLAY:
				ShowMenu(&mdDisplayMenu);
				break;

			case MENU_SOUND:
				ShowMenu(&mdSoundMenu);
				break;

			case MENU_TOGGLEFULLSCREEN:
				TerminateGame();
				Cvar_SetValue("Fullscreen", !Cvar_GetValue("Fullscreen"));
				InitializeGame();
				break;

			case MENU_CHANGERESOLUTION:
				ShowMenu(&mdResolutionMenu);
				break;

			case MENU_CHANGECOLORDEPTH:
				ShowMenu(&mdColorDepthMenu);
				break;

			case MENU_CHANGEGAMMA:
				ShowMenu(&mdGammaMenu);
				break;

			case MENU_COPERNICUSMODE:
				if (Cvar_GetValue("CopernicusMode"))
				{
					Cvar_SetValue("CopernicusMode", 0);
					Cvar_SetValue("SpaceMode", 2);
				}
				else
				{
					Cvar_SetValue("CopernicusMode", -1);
					Cvar_SetValue("SpaceMode", 1);
				}

				break;
			
			case MENU_RESOLUTION640X480:
			case MENU_RESOLUTION800X600:
			case MENU_RESOLUTION1024X768:
				i = CurrentMenu->MenuItems[CurrentMenu->ListItem].ID - MENU_RESOLUTION640X480;
				if (g_cvResolution.value != i)
				{
					TerminateGame();
					Cvar_SetValue("Resolution", (float)i);
					InitializeGame();
				}
				break;

			case MENU_COLORDEPTH16:
			case MENU_COLORDEPTH32:
				i = CurrentMenu->MenuItems[CurrentMenu->ListItem].ID - MENU_COLORDEPTH16;
				if (g_cvColorDepth.value != i)
				{
					TerminateGame();
					Cvar_SetValue("ColorDepth", (float)i);
					InitializeGame();
				}
				break;

			case MENU_KEYBINDS:
				ShowMenu(&mdKeyBindsMenu);

				// Take over control of the mouse if it's 
				// currently enabled to get binds
				SetMouse(BOOL(Cvar_GetValue("MouseEnabled")));
				break;

			case MENU_CONTROLLER:
				ShowMenu(&mdControllerMenu);
				break;

			case MENU_DEVELOPER:
				ShowMenu(&mdDeveloperMenu);
				break;

			case MENU_MOUSE_ENABLED:
				SetMouse(!BOOL(Cvar_GetValue("MouseEnabled")));
				break;

			case MENU_INFINITELIVES:
			case MENU_INFINITESHIELDS:
			case MENU_INVINCIBILITY:
				g_bCheatsEnabled = TRUE;

			case MENU_JOYSTICKTOGGLE:
			case MENU_FORCEFEEDBACKTOGGLE:
			case MENU_LIGHTINGTOGGLE:
			case MENU_BLENDINGTOGGLE:
			case MENU_TEXTURETOGGLE:
			case MENU_WIREFRAMETOGGLE:
			case MENU_BOUNDINGBOXTOGGLE:
			case MENU_SHOWFPSTOGGLE:
			case MENU_TOGGLE3D:
				if (CurrentMenu->MenuItems[CurrentMenu->ListItem].Cvar != NULL)
					Cvar_SetValue(CurrentMenu->MenuItems[CurrentMenu->ListItem].Cvar->name, !BOOL(CurrentMenu->MenuItems[CurrentMenu->ListItem].Cvar->value));
				break;

			case MENU_TOGGLEVSYNC:
				SetVsync(BOOL(Cvar_GetValue("VsyncEnabled")));
				break;

			case MENU_SOUNDTOGGLE:
				SetSound(!BOOL(Cvar_GetValue("SoundEnabled")));

				// Play beep sound -- it was weird that the beep wasn't
				// played when the sound was reenabled...
				PlayMenuBeepSound();

				break;

			default:		
				UpdateMessage("Unknown menu ID %d", CurrentMenu->MenuItems[CurrentMenu->ListItem].ID);
				break;
			}
		}
	}

	return TRUE;
}


void Menu_DrawGL()
{

	int i;
	char cTransparancy = (char)255;
	char cSlider[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


	if (CurrentMenu->MenuID == MENU_KEYBINDS)
	{
		KeyBindMenu_DrawGL();
	}
	else
	{

		// This will allow me to draw transparent black
		// shapes on the screen.  Without this the box 
		// isn't drawn because black is the transparent color
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Overwrite the current display with an untextured black square
		renderdisable(render_lighting | render_depthtest | render_texture);
		renderenable(render_blend);
		glColor4ub(0, 0, 0, 180);
		const float fx = 0;
		const float fy = 2;
		const float fz = -30;
		const float fWidth = 10;
		float fHeight = (CurrentMenu->ListCount * 0.5f) + 2;
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(fx + fWidth, fy + fHeight, fz);
			glVertex3f(fx - fWidth, fy + fHeight, fz);
			glVertex3f(fx + fWidth, fy - fHeight, fz);
			glVertex3f(fx - fWidth, fy - fHeight, fz);
		glEnd();

		// Restore the blend func for translucency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		// Print menu title
		glColor4ub(96,96,180,255);
		glPrintCentered(0, float((CurrentMenu->ListCount * 0.5) + 2), CurrentMenu->Caption);

		// Setup selected item
		if (CurrentMenu->pListSelect == NULL) 
			nSelectedItem = -1;
		else
			nSelectedItem = (int)CurrentMenu->pListSelect->value;

		// Loop through the list items
		for (i = 0; i < CurrentMenu->ListCount; i++)
		{
		
			// Set transparency of disabled items
			if ((CurrentMenu->MenuItems[i].Type & MNUI_DISABLED) == MNUI_DISABLED)
				cTransparancy = (char)64;
			else
				cTransparancy = (char)255;

			// Set color based on the current item's state
			if (i == CurrentMenu->ListItem)
				glColor4ub(255,255,0,cTransparancy);
			else
				glColor4ub(255,255,255,cTransparancy);

			// Put little pointers next to selected item
			if (i == CurrentMenu->ListItem)
				glPrintCenteredVar(0, float(CurrentMenu->ListCount / 2 - i), "%c                        %c", 129, 128);


			// Draw current menu item
			switch (CurrentMenu->MenuItems[i].Type & MNUI_CONTROL_BITS)
			{

			// Buttons are the generic menu item type
			case MNUI_BUTTON:
				glPrintCenteredVar(0, float(CurrentMenu->ListCount / 2 - i), "%s", CurrentMenu->MenuItems[i].Caption);
				break;

			// Bullets are displayed next to items flagged as options
			case MNUI_OPTION:

				// Draw menu item text
				glPrintCenteredVar(0, float(CurrentMenu->ListCount / 2 - i), "%s", CurrentMenu->MenuItems[i].Caption);
				
				// Draw bullets 
				if (nSelectedItem != -1)
				{
					if (i == nSelectedItem)
						glPrintCenteredVar(0, float(CurrentMenu->ListCount / 2 - i), "%c                 ", 142);
					else
						glPrintCenteredVar(0, float(CurrentMenu->ListCount / 2 - i), "%c                 ", 141);
				}

				break;
		
			// ON/OFF are displayed next to toggle items
			case MNUI_TOGGLE:
				if (CurrentMenu->MenuItems[i].Cvar != NULL)
				{
					if ((CurrentMenu->MenuItems[i].Type & MNUI_REVERSE_LOGIC) == MNUI_REVERSE_LOGIC)
					{
						if ((CurrentMenu->MenuItems[i].Cvar->value != 0))
							glPrintCenteredVar(0, float(CurrentMenu->ListCount / 2 - i), "%-18sOFF", CurrentMenu->MenuItems[i].Caption);
						else
							glPrintCenteredVar(0, float(CurrentMenu->ListCount / 2 - i), "%-18s ON", CurrentMenu->MenuItems[i].Caption);
					}
					else
					{
						if ((CurrentMenu->MenuItems[i].Cvar->value != 0))
							glPrintCenteredVar(0, float(CurrentMenu->ListCount / 2 - i), "%-18s ON", CurrentMenu->MenuItems[i].Caption);
						else
							glPrintCenteredVar(0, float(CurrentMenu->ListCount / 2 - i), "%-18sOFF", CurrentMenu->MenuItems[i].Caption);
					}
				}
				break;

			// Adjustable values 0 - 100%
			case MNUI_SLIDER:
				if (CurrentMenu->MenuItems[i].Cvar != NULL)
				{
					
					// Sliders are drawn using modified chars in the font
					// texture.  I changed a couple of the useless characters
					// into graphics I can use for the slider bar.
					int nSliderValue = int((CurrentMenu->MenuItems[i].Cvar->value * 100.0f) + 0.5f) / 10;
					for (int j = 0; j < 10; j++)
					{
						if (nSliderValue > j)
							cSlider[j] = (char)135;
						else
							cSlider[j] = (char)134;
					}
					glPrintCenteredVar(0, float(CurrentMenu->ListCount / 2 - i), "%-10s %10s", CurrentMenu->MenuItems[i].Caption, &cSlider);
				}
				break;
			}
		}
	}
}




void KeyBindMenu_DrawGL()
{

	int i;
	
	// This will allow me to draw transparent black
	// shapes on the screen.  Without this the box 
	// isn't drawn because black is the transparent color
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Overwrite the current display with an untextured black square
	renderdisable(render_lighting | render_depthtest | render_texture);
	renderenable(render_blend);
	glColor4ub(0, 0, 0, 180);
	const float fx = 0;
	const float fy = 0;
	const float fz = -30;
	const float fWidth = 14;
	float fHeight = ((NUM_KEY_BINDS + 2) * 0.5f) + 2;

	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(fx + fWidth, fy + fHeight, fz);
		glVertex3f(fx - fWidth, fy + fHeight, fz);
		glVertex3f(fx + fWidth, fy - fHeight, fz);
		glVertex3f(fx - fWidth, fy - fHeight, fz);
	glEnd();

	// Restore the blend func for translucency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);


	// Loop through the list items
	for (i = 0; i < NUM_KEY_BINDS; i++)
	{
		// Selected item is red
		// Highlighted item is yellow
		// Others are white
		if (i == nSelectedItem)
			glColor4ub(255,0,0,255);
		else if (i == CurrentMenu->ListItem)
			glColor4ub(255,255,0,255);
		else
			glColor4ub(255,255,255,255);

		// Put little pointers next to selected item
		if (i == CurrentMenu->ListItem)
			glPrintCenteredVar(0, float(NUM_KEY_BINDS / 2 - i), "%c                                      %c", 129, 128); // glPrintCenteredVar(0, float(NUM_KEY_BINDS / 2 - i), "%c                            %c", 129, 128);

		// Display bind description and current key
		glPrintCenteredVar(0, float(NUM_KEY_BINDS / 2 - i), "%-15s %10s %10s", g_asKeyBindsText[i], cKeyNames[int(g_acvKeyBinds[i * 2]->value)], cKeyNames[int(g_acvKeyBinds[(i * 2) + 1]->value)]);
		
	}


	// Print Exit menu item
	if (CurrentMenu->ListItem == EXIT_LISTINDEX)
	{
		glColor4ub(255,255,0,255);
		glPrintCenteredVar(0, float(NUM_KEY_BINDS / 2 - NUM_KEY_BINDS - 1), "%c                            %c", 129, 128);
	}
	else
	{
		glColor4ub(255,255,255,255);
	}
	glPrintCentered(0, float(NUM_KEY_BINDS / 2 - NUM_KEY_BINDS - 1), "EXIT");


	// If a list item is selected, then print instructions
	if (nSelectedItem != -1)
	{
		glColor4ub(255,255,255,255);
		glPrintCentered(0, float(-NUM_KEY_BINDS), "PRESS NEW KEY OR ENTER TO CANCEL CHANGE");
	}
	
		
}



void KeyBindMenu_Update()
{
	int key = 0; 

	// Handle exit key
	if (CurrentMenu->ListItem == EXIT_LISTINDEX)
	{
		if (KeyDown(VK_RETURN) || KeyDown(VK_ESCAPE))
		{
			// Don't hog the mouse
			UnacquireMouse();

			PlayMenuBeepSound();
			ShowLastMenu();
		}
		else if (KeyDown(VK_UP))
		{
			PlayMenuBeepSound();
			CurrentMenu->ListItem = NUM_KEY_BINDS - 1;
		}
		else if (KeyDown(VK_DOWN))
		{
			PlayMenuBeepSound();
			CurrentMenu->ListItem = 0;
		}
	}
	else
	{

		// If an item is already selected, lock the selection
		if (nSelectedItem == -1)
		{
			
			// Handle ESC key -- return to previous menu
			if (KeyDown(VK_ESCAPE))
			{
				// Don't hog the mouse
				UnacquireMouse();

				PlayMenuLowSound();
				ShowLastMenu();
			}

			// Change list item
			if (KeyDown(VK_UP))
			{
				PlayMenuBeepSound();
				CurrentMenu->ListItem--;
			}
			if (KeyDown(VK_DOWN))
			{
				PlayMenuBeepSound();
				CurrentMenu->ListItem++;
			}

			// Wrap menu
			if (CurrentMenu->ListItem < 0)
			{
				CurrentMenu->ListItem = EXIT_LISTINDEX;
			}
			// Jump from last list item to exit item
			else if (CurrentMenu->ListItem >= NUM_KEY_BINDS)
			{
				CurrentMenu->ListItem = EXIT_LISTINDEX;
			}

		}

		// Select a list item when the ENTER key is hit
		if (KeyDown(VK_RETURN))
		{

			PlayMenuBeepSound();

			if (nSelectedItem == -1) 
				nSelectedItem = CurrentMenu->ListItem;
			else
				nSelectedItem = -1;
		}

		// When an item is selected, a keypress will
		// trigger that key's selection for the desired
		// control.  Of course, certain keys cannot
		// be selected.  (like ENTER)
		if ((nSelectedItem != -1) && (!KeyDown(VK_RETURN)))
		{
			for (key = 1; key < 256; key++)
			{
				if (KeyDown(key)) 
				{
					PlayMenuLowSound();
					
					// Find duplicate key binds
					KeyBindMenu_RemoveDuplicates(key);
					
					// Use the current key as that item's alt setting
					Cvar_SetValue(g_acvKeyBinds[(nSelectedItem * 2) + 1]->name, g_acvKeyBinds[nSelectedItem * 2]->value);

					// Set the primary key bind
					Cvar_SetValue(g_acvKeyBinds[nSelectedItem * 2]->name, (float)key);

					// Make sure primary and secondary keys aren't the same
					if (g_acvKeyBinds[nSelectedItem * 2]->value == g_acvKeyBinds[(nSelectedItem * 2) + 1]->value)
					{
						Cvar_SetValue(g_acvKeyBinds[(nSelectedItem * 2) + 1]->name, 0);
					}

					nSelectedItem = -1;
				}
			}
		}
	}
}

void KeyBindMenu_RemoveDuplicates(const int key)
{
	for (int i = 0; i < NUM_KEY_BINDS; i++)
	{
		// Don't check the current item -- the calling 
		// routine handles those
		if (i != nSelectedItem)
		{
			// Check the primary key
			if (g_acvKeyBinds[i * 2]->value == key)
			{
				Cvar_SetValue(g_acvKeyBinds[i * 2]->name, g_acvKeyBinds[(i * 2) + 1]->value);
				Cvar_SetValue(g_acvKeyBinds[(i * 2) + 1]->name, 0);
			}

			// Check secondary key
			if (g_acvKeyBinds[(i * 2) + 1]->value == key)
			{
				Cvar_SetValue(g_acvKeyBinds[(i * 2) + 1]->name, 0);
			}
		}
	}
}

