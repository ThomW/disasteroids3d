// command.h

#ifndef COMMAND_H
#define COMMAND_H

#include <windows.h>

// command structure
typedef struct command_s {
	char					*name;
	void					(*ptrFunction)();
	struct				command_s*	next;
} command_t;


// public functions
void cmd_Register(command_t*);
BOOL cmd_Execute(const char*);


#endif