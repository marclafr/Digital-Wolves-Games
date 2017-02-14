// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "d1Timer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
d1Timer::d1Timer()
{
	Start();
}

// ---------------------------------------------
void d1Timer::Start()
{
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
uint32 d1Timer::Read() const
{
	return SDL_GetTicks() - started_at;
}

// ---------------------------------------------
float d1Timer::ReadSec() const
{
	return float(SDL_GetTicks() - started_at) / 1000.0f;
}