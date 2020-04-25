
/* 
=================================================================

	tokenize.cpp

	AUTHOR:	Thom Wetzel, Jr.
				www.LMNOpc.com

	PURPOSE:
		This routine accepts a string and seperates each word
		in the sentence into another element in the g_Tokens
		array.  The tokenizer's main purpose is for reading in
		lines from a game's configuration (.cfg) file.
				  
		The only 'cool' thing about this routine is that it 
		doesn't break up words in quotes so that a line like:
		
		  PlayerName "Your Name Here"

		is parsed correctly.

	REVISIONS:
		2000.10.27	Added single quote seperator
			I did this so that things like nicknames ie.
			'Thom "Savage" Wetzel' could be parsed correctly by
			using the single quotes as token delimiters instead 
			of the double quotes.  This will make the routine 
			a little more flexible, but still wouldn't allow a
			line like 
			
			  'Thom "Savage" Wetzel's Wild Ride'

			to be parsed as a single token.  It would appear as 
			
			Thom "Savage" Wetzel
			s Wild Ride

			which is totally screwy. 
			
			What do you expect for free? ;)

		2002.03.26
			Eliminated the single quote seperators.  It made certain
			words like it's not parse correctly.  In the future, the
			string printing routines should do something similar to 
			HTML where quotes are stored as &quot; or maybe %22

================================================================= 
*/


#include "tokenize.h"
#include <string.h>
#include <stdio.h>

char	g_Tokens[MAX_TOKEN_COUNT][MAX_TOKEN_SIZE];
int	g_TokenCount = 0;

// Constants returned from GetCharType
const int cCharTypeTokenText 			= 0;
const int cCharTypeTokenQuotes 		= 1;
const int cCharTypeTokenSeperator 	= 2;

#define TRUE		1
#define FALSE		0
#define NULL		0

int Tokenize1(const char *String)
{
	bool bInQuotes = FALSE;
	int nCharIndex = 0;
	char cStartingQuoteChar = NULL;

	// Reset token count
	g_TokenCount = 0;
	
	// Fill g_Tokens with nulls
	memset((void *)g_Tokens, 0, MAX_TOKEN_COUNT * MAX_TOKEN_SIZE * sizeof(char));

	// Loop through each character in the string
	// and parse the tokens into the Tokens array
	for (String; *String != 0 ; String++)
	{

		// Do action based on the type of character
		// at the string pointer's current position
		switch (GetCharType(*String))
		{
		
		case cCharTypeTokenText:
			g_Tokens[g_TokenCount][nCharIndex++] = *String;
			break;

//		case cCharTypeTokenQuotes:
//			bInQuotes = !bInQuotes;
//			break;
		case cCharTypeTokenQuotes:
			if (bInQuotes == FALSE)
			{
				cStartingQuoteChar = *String;
				bInQuotes = TRUE;
			}	
			else
			{
				if (*String == cStartingQuoteChar)
				{
					cStartingQuoteChar = NULL;
					bInQuotes = FALSE;
				}
				else
				{
					g_Tokens[g_TokenCount][nCharIndex++] = *String;
				}
			}
			break;


		case cCharTypeTokenSeperator:
			if (bInQuotes)
			{
				g_Tokens[g_TokenCount][nCharIndex++] = *String;
			}
			else
			{
				if (nCharIndex)
				{
					g_TokenCount++;
					nCharIndex = 0;
				}
			}
			break;
		}
	}

	// If the program is in mid-token, clean it up
	if (nCharIndex)
	{
		g_TokenCount++;
		nCharIndex = 0;
		bInQuotes = FALSE;
	}

	// Return the number of tokens found
	return g_TokenCount;

}


int Tokenize(const char *strText)
{
	bool bInQuotes = FALSE;
	int nCharIndex = 0;
	char cStartingQuoteChar = NULL;
	
	int nIdx = 0;
	int nLen = 0;

	// Reset token count
	g_TokenCount = 0;
	
	// Fill g_Tokens with nulls
	memset((void *)g_Tokens, 0, MAX_TOKEN_COUNT * MAX_TOKEN_SIZE * sizeof(char));

	//nLen = strlen(strText);
	nLen = strlen(strText);

	for (nIdx=0; nIdx < nLen; nIdx++)
		printf("%c", strText[nIdx]);
	printf("\n");

	// Loop through each character in the string
	// and parse the tokens into the Tokens array
	for (nIdx=0; nIdx < nLen; nIdx++)
	{

		// Do action based on the type of character
		// at the string pointer's current position
		switch (GetCharType(strText[nIdx]))
		{
		
		case cCharTypeTokenText:
			g_Tokens[g_TokenCount][nCharIndex++] = strText[nIdx];
			break;

//		case cCharTypeTokenQuotes:
//			bInQuotes = !bInQuotes;
//			break;
		case cCharTypeTokenQuotes:
			if (bInQuotes == FALSE)
			{
				cStartingQuoteChar = strText[nIdx];
				bInQuotes = TRUE;
			}	
			else
			{
				if (strText[nIdx] == cStartingQuoteChar)
				{
					cStartingQuoteChar = NULL;
					bInQuotes = FALSE;
				}
				else
				{
					g_Tokens[g_TokenCount][nCharIndex++] = strText[nIdx];
				}
			}
			break;


		case cCharTypeTokenSeperator:
			if (bInQuotes)
			{
				g_Tokens[g_TokenCount][nCharIndex++] = strText[nIdx];
			}
			else
			{
				if (nCharIndex)
				{
					g_TokenCount++;
					nCharIndex = 0;
				}
			}
			break;
		}
	}

	// If the program is in mid-token, clean it up
	if (nCharIndex)
	{
		g_TokenCount++;
		nCharIndex = 0;
		bInQuotes = FALSE;
	}

	// Return the number of tokens found
	return g_TokenCount;

}


int GetCharType(const char& C)
{
	switch (C)
	{
	case ' ':
	case '\n':
	case '\0':
		return cCharTypeTokenSeperator;
		break;

	case '\"':
		return cCharTypeTokenQuotes;
		break;

	default:
		return cCharTypeTokenText;
		break;
	}
}


// ========================================
// Sample application using Tokenize
// ========================================
#if 0

#include <string.h>
#include <stdio.h>

void main( void )
{

	//const char SourceString[] = "\"Token Test\"      \"a b c";
	const char SourceString[] = "\"Thom 'Savage' Wetzel's\" Wild Ride \"This is stupid\" MORE TESTING!!! AND MORE and even more...\0";
	// char FixedString[255];
	int nTokenCount = 0;
	int j = 0;

	printf("Length=%d\n", strlen(SourceString));

	// Convert special characters
	// sprintf(FixedString, SourceString);

	printf("Length=%d\n", strlen(SourceString));

	for (j = 0; j < 1; j++)
	{

		// Run the 'tokenizer'
		nTokenCount = Tokenize((const char*)SourceString);

		// Display how many tokens were found
		printf("Token count = %d\n\n", g_TokenCount);

		// Display all the tokens
		for (int i = 0; i < nTokenCount; i++)
			printf("%2d \"%s\"\n", i, g_Tokens[i]);

	}
}
#endif
