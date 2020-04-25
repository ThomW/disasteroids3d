//
// cvars.cpp
//

#include "cvars.h"

// Pointer to cvars list
cvar_t *cvars = NULL;


// Searches CVars for name
cvar_t *Cvar_FindVar(const char *name)
{
	cvar_t *ptr = NULL;

	for (ptr = cvars; ptr != NULL; ptr = ptr->next)
		if (!stricmp(name, ptr->name))
			return ptr;
			
	return NULL;
}


void Cvar_Register(cvar_t *variable)
{

	char *tempstr;
	
	// Search for duplicate Cvar and exit
	// if one is found
	if (Cvar_FindVar(variable->name))
		return;
	
	// malloc the Cvar's string variable because it will 
	// be free'd if the value is changed
	tempstr = variable->string;
	variable->string = (char *)malloc(strlen(variable->string)+1);
	if (variable->string == NULL)
	{
		printf("ERROR: Insufficient memory available\n");
		return;
	}
	
	strcpy(variable->string, tempstr);
	variable->value = (float)atof(variable->string);

	// Add the Cvar to the beginning of the cvar list
	variable->next = cvars;
	cvars = variable;

	return;

}


// Save the value of all of the CVars flagged as persistant
bool Cvar_SaveAll(const char *Filename)
{
	FILE *fp = NULL;
	cvar_t *ptr = NULL;
	
	if (!Filename)
		return FALSE;
	
	fp = fopen(Filename,"w");
	if (!fp)
		return FALSE;

	for (ptr = cvars; ptr != NULL; ptr = ptr->next)
		if (ptr->persistant)
			fprintf(fp, "%s \"%s\"\n", ptr->name, ptr->string);

	fclose(fp);
	
	return TRUE;
}


bool Cvar_SetString(const char *name, const char *newstring)
{
	cvar_t *ptr = NULL;

	ptr = Cvar_FindVar(name);

	if (!ptr)
		return FALSE;

	free(ptr->string);
	ptr->string = (char *)malloc(strlen(newstring) + 1);
	strcpy(ptr->string, newstring);
	ptr->value = (float)atof(ptr->string);

	return TRUE;
	
}


bool Cvar_SetValue(const char *name, const float newvalue)
{
	char value[16];

	sprintf(value, "%f", newvalue);

	return Cvar_SetString(name, value);
}



char *Cvar_GetString(const char *name)
{
	cvar_t *ptr = NULL;

	ptr = Cvar_FindVar(name);

	if (!ptr)
		return "";

	return ptr->string;

}



float Cvar_GetValue(const char *name)
{
	cvar_t *ptr = NULL;

	ptr = Cvar_FindVar(name);

	if (!ptr)
		return 0;

	return ptr->value;
}
