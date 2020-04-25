// command.c

/*

	This module controls commands that the player may issue 
	from scripts or from the console

*/

#include "console.h"
#include "command.h"


// Pointer to cvars list
command_t *cmds = NULL;


/* This function searches the list of commands for one that 
	matches the given name
*/
command_t *cmd_FindCommand(const char *name)
{
	command_t *ptr = NULL;

	for (ptr = cmds; ptr != NULL; ptr = ptr->next)
		if (!stricmp(name, ptr->name))
			return ptr;
			
	return NULL;
}


/*	This function gets called when a command should be executed
*/
BOOL cmd_Execute(const char *name)
{

	command_t *ptr = NULL;

	// Set ptr to the specified command
	ptr = cmd_FindCommand(name);

	// Pointer not found - return FALSE
	if (!ptr)
		return FALSE;

	// Execute pointer
	ptr->ptrFunction();

	// Return success
	return TRUE;

}


/*	Adds the specified command in the list of commands
*/
void cmd_Register(command_t *new_command)
{

	// Search for duplicate Cvar and exit
	// if one is found
	if (cmd_FindCommand(new_command->name))
	{
		con_printfvar("'%s' is already a command", new_command->name);
		return;
	}

	// Add the Cvar to the beginning of the cvar list
	new_command->next = cmds;
	cmds = new_command;

}

