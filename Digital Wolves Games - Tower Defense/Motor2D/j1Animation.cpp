#include "j1Animation.h"

j1Animation::j1Animation()
{}

// Destructor
j1Animation::~j1Animation()
{}

void j1Animation::SetSpeed(float spd)
{
	speed = spd;
}

void j1Animation::SetLoopState(bool state)
{
	loop = state;
}


void j1Animation::PushBack(const SDL_Rect& rect)
{
	frames.push_back(rect);
}

SDL_Rect& j1Animation::GetCurrentFrame()
{
	current_frame += speed;
	if (current_frame >= last_frame)
	{
		current_frame = (loop) ? 0.0f : last_frame - 1;
		loops++;
	}
	else if (current_frame < 0)
	{
		current_frame = last_frame - 1;
	}
	return frames[(int)current_frame];
}

bool j1Animation::Finished() const
{
	return loops > 0;
}

void j1Animation::Reset()
{
	current_frame = 0.0f;
	loops = 0;
}

void j1Animation::Clean()
{
	frames.clear();
}