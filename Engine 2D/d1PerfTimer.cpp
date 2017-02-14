// ----------------------------------------------------
// d1PerfTimer.cpp
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "d1PerfTimer.h"
#include "SDL\include\SDL_timer.h"

uint64 d1PerfTimer::frequency = 0;

// ---------------------------------------------
d1PerfTimer::d1PerfTimer()
{
	if(frequency == 0)
		frequency = SDL_GetPerformanceFrequency();

	Start();
}

// ---------------------------------------------
void d1PerfTimer::Start()
{
	started_at = SDL_GetPerformanceCounter();
}

// ---------------------------------------------
double d1PerfTimer::ReadMs() const
{
	return 1000.0 * (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
}

// ---------------------------------------------
uint64 d1PerfTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - started_at;
}


