// console.h

#ifndef CONSOLE_H
#define CONSOLE_H

#include <windows.h>
#include <stdio.h>

#include "game.h"
#include "cvars.h"

void con_init(void);
void con_kill(void);
void con_resize(const int, const int);
void con_draw(const float);
void con_update(void);

void con_dprintf(const char*, ...);

void con_printf(const char*);
void con_printfvar(const char*, ...);

BOOL con_dump(const char*);

extern BOOL g_con_active;

#endif
