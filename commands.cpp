// commands.c

/*
	These are the commands which are available to the console and 
	game scripts
*/

#include "game.h"
#include "commands.h"

/* function prototypes
*/
void dis_QuitGame(void);
void dis_ConDump(void);
void dis_Echo(void);

/* Command declarations
*/
command_t cmd_QuitGame		= { "quit",			&dis_QuitGame };
command_t cmd_Beep			= { "beep",			&PlayMenuBeepSound };
command_t cmd_ConsoleDump	= { "con_dump",	&dis_ConDump };
command_t cmd_Echo			= { "echo",			&dis_Echo };


/* Command functions
*/
void dis_QuitGame()
{
	g_bExitApp = TRUE;
}

void dis_ConDump()
{
	if (g_TokenCount != 2)
	{
		con_printf("con_dump <filename>");
	}
	else
	{
		if (con_dump(g_Tokens[1]))
			con_printfvar("Console saved to '%s'", g_Tokens[1]);
		else
			con_printfvar("ERROR: con_dump('%s')", g_Tokens[1]);
	}

	return;
}

void dis_Echo()
{

	char text[80];

	if (g_TokenCount < 2)
	{
		con_printf("echo <text>");
	}
	else
	{
		// Create text to print
		for (int i = 1; i < g_TokenCount; i++)
		{
			strcat(text, g_Tokens[i]);
			strcat(text, " ");
		}

		con_printfvar("%s", text);
	}
			
}


/* This routine must be called during applcation initialization
	to setup the commands
*/
void InitializeCommands()
{
	// Register the commands
	cmd_Register(&cmd_QuitGame);
	cmd_Register(&cmd_Beep);
	cmd_Register(&cmd_ConsoleDump);
	cmd_Register(&cmd_Echo);
}
