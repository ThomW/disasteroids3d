
#ifndef TIMER_H
#define TIMER_H

#include <windows.h>
#include <time.h>

// Create A Structure For The Timer Information
typedef struct timer_s
{
  __int64			frequency;							// Timer Frequency
  float				resolution;							// Timer Resolution
  unsigned long	mm_timer_start;					// Multimedia Timer Start Value
  unsigned long	mm_timer_elapsed;					// Multimedia Timer Elapsed Time
  bool				performance_timer;				// Using The Performance Timer?
  __int64			performance_timer_start;		// Performance Timer Start Value
  __int64			performance_timer_elapsed;		// Performance Timer Elapsed Time
} timer_t;

extern timer_t g_timer;

// Timer routines
void TimerInit(void);
float TimerGetTime(void);
float TimerGetTimeMS(void);

#endif 