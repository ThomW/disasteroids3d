/* 

	tokenize.h

	AUTHOR:	Thom Wetzel, Jr.
				www.LMNOpc.com

	PURPOSE:
		Header file for tokenize.cpp

*/

#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <memory.h>

	// All tokens are returned in this array
	#define MAX_TOKEN_COUNT		8
	#define MAX_TOKEN_SIZE		80
	
	extern char g_Tokens[MAX_TOKEN_COUNT][MAX_TOKEN_SIZE];
	extern int	g_TokenCount;

	// Function definitions
	int Tokenize(const char*);
	int GetCharType(const char&);

#endif
