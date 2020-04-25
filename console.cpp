// console.cpp

#include "console.h"

BOOL g_con_active = FALSE;

#define MAX_LEN 60
#define MAX_COMMAND_HISTORY 10

#define MAX_NOTIFY_LINES 10
#define MAX_CONSOLE_LINES 20

// CVARs dealing with console system
cvar_t con_maxlines = {"con_maxlines", "4096", TRUE};
cvar_t con_notifyduration = {"con_notifyduration", "5", TRUE};

// 
int  nBufferPos = 0;
char cmd_buffer[MAX_LEN];
int  nHistoryIdx = -1;
int  nHistoryCount = 0;
char cmd_history[MAX_LEN][MAX_COMMAND_HISTORY];
const char cLegalChars[]	= " .0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ=-._\b\t\r";

// cvar storage struct definition
typedef struct console_s {
	char	string[MAX_LEN];
	float  display_end_time;
	struct console_s*	next;
	struct console_s* prev;
} console_t;

// Pointers to console list
console_t *con_top = NULL;
console_t *con_last = NULL;
int con_nLineCount = 0;

const char scancode2ascii[] = {	
	0,		0,		0,		0,		0,		0,		0,		0,		'\b',	0,		//   9
	0,		0,		0,		'\r',	0,		0,		0,		0,		0,		0,		//  19
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		//  29
	0,		0,		' ',	0,		0,		0,		0,		0,		0,		0,		//  39
	0,		0,		0,		0,		0,		0,		0,		0,		'0',	'1',	//  49
	'2',	'3',	'4',	'5',	'6',	'7',	'8',	'9',	0,		0,		//  59
	0,		0,		0,		0,		0,		'a',	'b',	'c',	'd',	'e',	//	 69
	'f',	'g',	'h',  'i',	'j',	'k',	'l',	'm',	'n',	'o',	//	 79
	'p',	'q',  'r',	's',	't',	'u',	'v',	'w',	'x',	'y',	//  89
	'z',	0,		0,		0,		0,		0,		0,		0,		0,		0,		//	 99

	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 109
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 119
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 129
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 139
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 149
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 159
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 169
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 179
	0,		0,		0,		0,		0,		0,		';',	'=',	',',	'-',	// 189
	'.',	'/',	0,		0,		0,		0,		0,		0,		0,		0,		// 199
	
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 209
	0,		0,		0,		0,		0,		0,		0,		0,		0,		'[',	// 219
	92,	']',	96,	0,		0,		0,		0,		0,		0,		0,		// 229
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 239
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 249
	0,		0,		0,		0,		0,		0,										// 255

// shifted

	0,		0,		0,		0,		0,		0,		0,		0,		'\b',	0,		//   9
	'\r',	0,		0,		0,		0,		0,		0,		0,		0,		0,		//  19
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		//  29
	0,		0,		' ',	0,		0,		0,		0,		0,		0,		0,		//  39
	0,		0,		0,		0,		0,		0,		0,		0,		')',	'!',	//  49
	'@',	'#',	'$',	'%',	'^',	'&',	'*',	'(',	0,		0,		//  59
	0,		0,		0,		0,		0,		'A',	'B',	'C',	'D',	'E',	//	 69
	'F',	'G',	'H',  'I',	'J',	'K',	'L',	'M',	'N',	'O',	//	 79
	'P',	'Q',  'R',	'S',	'T',	'U',	'V',	'W',	'X',	'Y',	//  89
	'Z',	0,		0,		0,		0,		0,		0,		0,		0,		0,		//	 99

	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 109
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 119
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 129
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 139
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 149
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 159
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 169
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 179
	0,		0,		0,		0,		0,		0,		':',	'+',	'<',	'_',	// 189
	'>',	'?',	0,		0,		0,		0,		0,		0,		0,		0,		// 199
	
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 209
	0,		0,		0,		0,		0,		0,		0,		0,		0,		'{',	// 219
	'|',	'}',	'"',	0,		0,		0,		0,		0,		0,		0,		// 229
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 239
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		// 249
	0,		0,		0,		0,		0,		0,										// 255
		
};


/* Private functions 

  These functions are not accessable outside this code module

*/

console_t *con_FindPrevious(console_t *con_item)
{

	console_t *ptr = NULL;

	// Find the console item with a next value that points
	// to this console item
	for (ptr = con_top; ptr != NULL; ptr = ptr->next)
		if (ptr->next == con_item)
			return(ptr);

	return NULL;

}


void con_Delete(console_t *con_item)
{
	console_t *ptr = NULL;
	
	// Special case if item being deleted is top
	if (con_item == con_top) {
		con_top = con_item->next;
		free(con_item);
		con_nLineCount--;
	}

	// Special case is item being deleted is bottom
	if (con_item == con_last) {
		ptr = con_FindPrevious(con_item);
		if (ptr)
		{
			ptr->next = NULL;
			con_last = ptr;
			free(con_item);
			con_nLineCount--;
		}
		else
		{
			con_last = NULL;
			free(con_item);
			con_nLineCount--;
		}
	}

	// Normal handler
	else {
		// Find the console item with a next value that points
		// to this console item
		ptr = con_FindPrevious(con_item);
		if (ptr != NULL) {
			ptr->next = con_item->next;
			free(con_item);
			con_nLineCount--;
		}
	}

}



// Items are added at the top of the list
void con_Add(const char *text)
{
	console_t *newitem = NULL;

	// allocate memory for new item
	// and make sure allocation went ok
	newitem = (console_t *)malloc(sizeof(console_t));
	if (newitem == NULL) {
		printf ("ERROR: Insufficient memory available\n");
		return;
	}

	// Insert the newest console item first in the console list
	newitem->next = con_top;
	newitem->display_end_time = g_fNow + con_notifyduration.value;
	con_top = newitem;

	// Set the console's line text
	strncpy(newitem->string, text, MAX_LEN);

	// Increment line count
	con_nLineCount++;

	// Make sure there aren't too many lines in the console.
	// If there are, delete the oldest item in the list
	if (con_nLineCount > con_maxlines.value)
		con_Delete(con_last);

}



void con_UpdateCmdHistory()
{
	int i;

	// Shift all the old commands up in the array
	for (i = MAX_COMMAND_HISTORY - 1; i >= 1;  i--)
		strcpy(cmd_history[i], cmd_history[i-1]);

	// Copy the new command into the command history array
	strcpy(cmd_history[0], cmd_buffer);
	
	// Keep track of how many items in the history array 
	// are populated
	nHistoryCount++;
	if (nHistoryCount > MAX_COMMAND_HISTORY)
		nHistoryCount = MAX_COMMAND_HISTORY;

}


/*
Cvar_DisplayValue

This function is here so that I don't have to make cvars.cpp dependant
on console.cpp.  
*/
BOOL Cvar_DisplayValue(const char *name)
{
	cvar_t *ptr = NULL;

	// Tokenize the string to see if this is a Cvar being assigned a new value
	if (g_TokenCount == 2)
	{
		// Try to locate the variable
		ptr = Cvar_FindVar(name);

		// Variable was found
		if (ptr)
		{
			// Set cvar to value
			Cvar_SetString(g_Tokens[0], g_Tokens[1]);

			// Print updated value
			con_printfvar("\"%s\" changed to \"%s\"", ptr->name, ptr->string);

			// Exit indicating CVAR found
			return TRUE;
		}
		else 
		{
			return FALSE;
		}
	}

	// Does 'name' contain a Cvar?
	ptr = Cvar_FindVar(name);
	if (!ptr)
		return FALSE;
	con_printfvar("\"%s\" = \"%s\"", ptr->name, ptr->string);
	return TRUE;

}


void ProcessCommand() 
{
	BOOL bDone = FALSE;

	// Make sure something was typed
	if (strlen(cmd_buffer))
	{
		// Print the command
		con_printfvar("%s", cmd_buffer);

		// Tokenize cmd_buffer
		Tokenize(cmd_buffer);

		/* This is for debugging the console 

		// Display tokens
		int i = 0;
		for (i = 0; i < g_TokenCount; i++)
			con_printfvar("%d %s", g_TokenCount, g_Tokens[i]);

		*/

		if (g_TokenCount) {

			// Attempt to excute the buffer as a command
			if (!bDone)
				bDone = cmd_Execute(g_Tokens[0]);

			// Search cvars for match
			if (!bDone)
				bDone = Cvar_DisplayValue(g_Tokens[0]);
		}
		
		// Display message saying that the entered value couldn't be found
		if (!bDone)
			con_printfvar("Couldn't find '%s'", cmd_buffer);

		// Add the latest command to the comand history array
		else
			con_UpdateCmdHistory();
	}

	// Reset the buffer
	nBufferPos = -1;
	nHistoryIdx = -1; 
	memset(cmd_buffer, 0, MAX_LEN);

}


void con_handlekeys(const char inkey)
{

	// Process function based on key press
	switch (inkey)
	{
		// RETURN
		case '\r':
			ProcessCommand();
			break;
		
		// BACKSPACE
		case '\b':
			if (nBufferPos >= 0)
			{
				cmd_buffer[nBufferPos] = 0;
				nBufferPos--;
			}
			break;

		// TAB - autocomplete
		case '\t':
			break;


		// Normal keys
		default:

			// Add key to command buffer
			if (nBufferPos < MAX_LEN-1)
				nBufferPos++;
			cmd_buffer[nBufferPos] = inkey;

			break;
	}
}

/* Public functions */


/* This function must be called during program initialization to 
	initialize the console variables
*/
void con_init() 
{

	// Init keypress handling variables	
	nBufferPos = -1;
	memset(cmd_buffer,	0, MAX_LEN);
	memset(cmd_history,	0, MAX_LEN * MAX_COMMAND_HISTORY);

	// Register cvars
	Cvar_Register(&con_maxlines);
	Cvar_Register(&con_notifyduration);

}


/* This function should be called on program termination to 
	clean up console variables
*/
void con_kill() {

	// Delete all the console variables
	while (con_top != NULL)
		con_Delete(con_top);

}


/* This routine should be called at the end of the main rendering loop
*/
void con_draw(const float fNow) 
{
	console_t *ptr = NULL;
	int nLineCount = 0;

	// if the console is totally visible, draw it based on its size.
	if (g_con_active)
	{
		// Draw a blackbackground for console
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		renderdisable(render_lighting | render_depthtest | render_texture | render_blend);
		glColor4ub(0, 0, 0, 0);
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f( 3.0f, 3.0f, -4.0f);
			glVertex3f(-3.0f, 3.0f, -4.0f);
			glVertex3f( 3.0f, -0.1f, -4.0f);
			glVertex3f(-3.0f, -0.1f, -4.0f);
		glEnd();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		// Color for console text
		glColor4ub(128, 128, 128, 255);

		// Find the number of lines in the buffer.  Don't 
		// allow more than MAX_CONSOLE_LINES to be displayed
		//
		// This will have to change when I implement pgup/dwn
		for (ptr = con_top; ptr != NULL; ptr = ptr->next)
		{
			nLineCount++;
			if (nLineCount >= MAX_CONSOLE_LINES)
				break;
		}

		if (nLineCount)
		{
			// Make sure console printing starts all the way at the bottom
			if (nLineCount < MAX_CONSOLE_LINES)
				nLineCount = MAX_CONSOLE_LINES;

			// Draw the console text
			for (ptr = con_top; ptr != NULL; ptr = ptr->next) 
			{
				glPrint(0, -14.4f, 12.0f - nLineCount * 0.6f, ptr->string);
				nLineCount--;

				// Stop the loop when the console is full
				if (nLineCount <= 0)
					break;
			}
		}

		// Display console key buffer
		glColor4ub(128,128,0,255);
		glPrintVar(0, -15, -0.6f, ">%s", cmd_buffer);

		// Store current matrix
		glPushMatrix();

		// Draw the cursor
		glColor3f(0.3f,0.0f,0.6f);
		glTranslatef(-13.2f + (nBufferPos * 0.6f), -0.1f, -30.0f);
		renderenable(render_blend);
		renderdisable(render_lighting | render_depthtest | render_texture);
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f( 0.3f,  0.3f, 0);
			glVertex3f(-0.3f,  0.3f, 0);
			glVertex3f( 0.3f, -0.3f, 0);
			glVertex3f(-0.3f, -0.3f, 0);
		glEnd();
		glColor3f(1.0f,1.0f,1.0f);

		// Restore matrix
		glPopMatrix();

	}

	// if the console is NOT visible, only draw console items whose
	// display time hasn't expired
	else
	{

		// Count the number of lines that are needed
		for (ptr = con_top; ptr != NULL; ptr = ptr->next)
		{
			if (ptr->display_end_time >= fNow)
			{
				nLineCount++;

				// Limit the number of lines that can be dislayed
				if (nLineCount >= MAX_NOTIFY_LINES)
					break;
			}
			else
				break;
		}

		if (nLineCount)
		{

			// DEBUG: Display console variable summary
			// glPrintCenteredVar(0, 0, "ttl_con_lines=%d l=%d  Dev=%f", con_nLineCount, nLineCount, g_cvDeveloper.value);

			// Draw the lines
			for (ptr = con_top; ptr != NULL; ptr = ptr->next) 
			{
				if (ptr->display_end_time >= fNow) 
				{
					glPrint(0, -15, -10 + nLineCount * 0.6f, ptr->string);
					nLineCount--;
				}
				else
					break;

				if (nLineCount <= 0)
					break;
			}
		}
	}
}



// Dump the contents of the console to a file
BOOL con_dump(const char *Filename)
{
	FILE *fp = NULL;
	console_t *ptr = NULL;
	
	if (!Filename)
		return FALSE;
	
	fp = fopen(Filename,"w");
	if (!fp)
		return FALSE;

	for (ptr = con_top; ptr != NULL; ptr = ptr->next)
		fprintf(fp, "%s\n", ptr->string);

	fclose(fp);
	
	return TRUE;
}


/* these messages are only logged when developer mode is active 
*/
void con_dprintf(const char *string, ...)
{
	if (g_cvDeveloper.value == 0)
		return;
	
	char text[MAX_LEN];
	va_list ap;

	// Parse The String For Variables And Convert 
	// Symbols To Actual Numbers
	va_start(ap, string);
		vsprintf(text, string, ap);
	va_end(ap);

	if (strlen(text))
		con_Add(text);

}


/* prints strings with no variables to the console
*/
void con_printf(const char *string) {
	con_Add(string);
}


/* This function is used by outside routines to print
	lines that are displayed as part of the console
*/
void con_printfvar(const char *string, ...) {

	char text[MAX_LEN];
	va_list ap;

	// Parse The String For Variables And Convert 
	// Symbols To Actual Numbers
	va_start(ap, string);
		vsprintf(text, string, ap);
	va_end(ap);

	if (strlen(text))
		con_Add(text);
}


/* This routine is the console's keystroke interpretter
*/
void con_update()
{

	int i = 0;
	char asc = 0;

	// Test console toggle key
	if (KeyDown(192))
	{
		// Toggle console
		g_con_active = !g_con_active;

		// Pause the game when the console is active
		PauseGame(g_con_active);
	}
	
	// If the console is not visible, there's no point
	// in checking for key hits
	if (!g_con_active)
		return;

	for (i = 0; i < 255; i++)
	{
		if (KeyDown(i))
		{
			// DEBUG - print pressed keys
			// con_printfvar("keydown=%d scancode2ascii[]='%c'", i, scancode2ascii[i]);

			// Shift key down
			if (keys[16])
				asc = scancode2ascii[i + 256];
			else
				asc = scancode2ascii[i];
			
			// If the scancode conversion array contains a key for the current
			// key, pass its ASCII value to the key handling routine
			if (asc)
				con_handlekeys(asc);
			
		}
	}

	// Special keys

	// History navigation
	if (KeyDown(VK_UP))
	{
		if (nHistoryIdx < nHistoryCount -1)
		{
			nHistoryIdx++;
			if (nHistoryIdx >= MAX_COMMAND_HISTORY) 
				nHistoryIdx = MAX_COMMAND_HISTORY - 1;
		}
		strcpy(cmd_buffer, cmd_history[nHistoryIdx]);
		nBufferPos = strlen(cmd_buffer) - 1;
	}
	if (KeyDown(VK_DOWN))
	{
		if (nHistoryCount > 0)
		{
			if (nHistoryIdx >= 0)
				nHistoryIdx--;
			
			if (nHistoryIdx >= 0)
				strcpy(cmd_buffer, cmd_history[nHistoryIdx]);
			else
				memset(cmd_buffer, 0, MAX_LEN);

			nBufferPos = strlen(cmd_buffer) - 1;

		}
	}

}

