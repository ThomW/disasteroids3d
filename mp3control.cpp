/*
	mp3control.cpp

	Copyright 
*/

#include <windows.h>

#include "game.h"

HWND	hwndMp3 = NULL;

const MP3_NEXTTRACK		= 40048;
const MP3_PREVTRACK		= 40044;
const MP3_PAUSE			= 40046;
const MP3_VOLUMEUP		= 40058;
const MP3_VOLUMEDOWN		= 40059;

const MP3PLAYER_INIT = 0;
const MP3PLAYER_WINAMP = 1;
const MP3PLAYER_SONIQUE = 2;
const MP3PLAYER_NONE = 99;
int	nMp3Player = MP3PLAYER_INIT;


int FindMp3Player(void)
{

	hwndMp3 = FindWindow("Winamp v1.x", NULL);
	if (NULL != hwndMp3) 
		return MP3PLAYER_WINAMP;

	hwndMp3 = FindWindow("Sonique Window Class", NULL);
	if (NULL != hwndMp3)
		return MP3PLAYER_SONIQUE;

	return MP3PLAYER_NONE;

}


void mp3control(void)
{

	WPARAM cmd = 0;
	LPARAM keycode = 0;


	/* Exit right away if no mp3 player was found */
	if (MP3PLAYER_NONE == nMp3Player)
		return;


	/* 
	Check keys and see if any of our MP3 control keys
	were just pressed.  'cmd' is being set to Winamp's
	WM_COMMAND value which will be 'translated' into
	Sonique keystrokes further down... 
	*/

	if (KeyDown(221)) 
	{
		cmd = MP3_NEXTTRACK;
		UpdateMessage("MUSIC - NEXT TRACK");
	}
	else if (KeyDown(219)) 
	{
		cmd = MP3_PREVTRACK;
		UpdateMessage("MUSIC - PREVIOUS TRACK");
	}
	else if (KeyDown(220)) 
	{
		cmd = MP3_PAUSE;
		UpdateMessage("MUSIC - PAUSE");
	}
	else if (KeyDown(187)) 
	{
		cmd = MP3_VOLUMEUP;
		UpdateMessage("MUSIC - VOLUME UP");
	}
	else if (KeyDown(189)) 
	{	
		cmd = MP3_VOLUMEDOWN;
		UpdateMessage("MUSIC - VOLUME DOWN");
	}

	/*
	If cmd has been set to an mp3 command constant,
	use FindWindow to get WinAmp's hWnd.  If WinAmp
	is not running, FindWindow will return NULL.
	*/
	if (cmd)
	{

		/* Look for an Mp3 player if one was not previously found*/
		if (MP3PLAYER_INIT == nMp3Player)
			nMp3Player = FindMp3Player();

		/* Display error message and exit if an mp3 player was not found */
		if (MP3PLAYER_NONE == nMp3Player)
		{
			UpdateMessage("SUPPORTED MP3 PLAYER NOT FOUND");
			return;
		}
		

		/*
		Send WM_COMMAND to WinAmp
		*/
		if (MP3PLAYER_WINAMP == nMp3Player)
		{
			PostMessage(hwndMp3, WM_COMMAND, cmd, 0);
			return;
		}

		else if (MP3PLAYER_SONIQUE == nMp3Player)
		{
			/* 
			Convert WinAmp cmd value into a key code
			*/
			switch (cmd)
			{
				case MP3_NEXTTRACK:	keycode = 'B'; break;
				case MP3_PREVTRACK:	keycode = 'Z'; break;
				case MP3_PAUSE:		keycode = 'C'; break;
				case MP3_VOLUMEUP:	keycode = VK_UP; break;
				case MP3_VOLUMEDOWN:	keycode = VK_DOWN; break;
				default: return;
			}

			/*
			The WinAmp command was converted, send messages
			to the Sonique window
			*/
			if (keycode)
			{
				PostMessage(hwndMp3, WM_KEYDOWN, keycode, 1);
				PostMessage(hwndMp3, WM_KEYUP, keycode, 1);
				/* PostMessage(hwndMp3, WM_CHAR,	keycode, 1); */
				return;
			}
		}
	}
}

