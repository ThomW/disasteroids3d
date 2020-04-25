
#include "timer.h"


BOOL bTimerInitialized = FALSE;

timer_t g_timer;

void TimerInit(void)								// Initialize Our Timer (Get It Ready)
{
	memset(&g_timer, 0, sizeof(g_timer));					// Clear Our Timer Structure

	// Check To See If A Performance Counter Is Available
	// If One Is Available The Timer Frequency Will Be Updated
	if (!QueryPerformanceFrequency((LARGE_INTEGER *) &g_timer.frequency))
	{
		// No Performace Counter Available
		g_timer.performance_timer	= FALSE;				// Set Performance Timer To FALSE
		g_timer.mm_timer_start	= timeGetTime();		// Use timeGetTime() To Get Current Time
		g_timer.resolution = 1.0f/1000.0f;				// Set Our Timer Resolution To .001f
		g_timer.frequency = 1000;							// Set Our Timer Frequency To 1000
		g_timer.mm_timer_elapsed	= g_timer.mm_timer_start;		// Set The Elapsed Time To The Current Time
	}
	else
	{
		// Performance Counter Is Available, Use It Instead Of The Multimedia Timer
		// Get The Current Time And Store It In performance_timer_start
		QueryPerformanceCounter((LARGE_INTEGER *) &g_timer.performance_timer_start);
		g_timer.performance_timer		= TRUE;				// Set Performance Timer To TRUE
		// Calculate The Timer Resolution Using The Timer Frequency
		g_timer.resolution		= (float) (((double)1.0f)/((double)g_timer.frequency));
		// Set The Elapsed Time To The Current Time
		g_timer.performance_timer_elapsed	= g_timer.performance_timer_start;
	}

	// Set flag to store that timer was already initialized
	bTimerInitialized = TRUE;

}


// Get Time In Seconds
float TimerGetTime()
{
	__int64 time;									// time Will Hold A 64 Bit Integer

	// Make sure the timer's initialized -- if not initialize it!
	if (!bTimerInitialized)
		TimerInit();

	// Are We Using The Performance Timer?
	if (g_timer.performance_timer)
	{
		// Grab The Current Performance Time
		QueryPerformanceCounter((LARGE_INTEGER *) &time);

		// Return The Current Time Minus The Start Time Multiplied By The Resolution 
		return (((float)(time - g_timer.performance_timer_start) * g_timer.resolution));
	}
	else
	{
		// Return The Current Time Minus The Start Time Multiplied By The Resolution 
		return (((float)(timeGetTime() - g_timer.mm_timer_start) * g_timer.resolution));
	}
}

float TimerGetTimeMS()
{
	return TimerGetTime() * 1000.0f;
}

