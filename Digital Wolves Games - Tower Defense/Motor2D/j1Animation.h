#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include <vector>
#define MAX_FRAMES 100

//Enums
enum UNIT_TYPE
{
	NO_UNIT = 0,

};

enum ACTION_TYPE
{
	NO_ACTION = 0,
	ATTACK,
	DIE,
	DISSAPEAR,
	IDLE,
	WALK
};

enum DIRECTION_TYPE
{
	NO_DIRECTION = 0,
	NORTH,
	NORTH_EAST,
	EAST,
	SOUTH_EAST,
	SOUTH,
	SOUTH_WEST,
	WEST,
	NORTH_WEST
};
//--

class j1Animation
{
public:

	j1Animation();

	// Destructor
	virtual ~j1Animation();


	std::vector<SDL_Rect> frames;

public:

	void SetSpeed(float speed);
	void SetLoopState(bool state);
	void PushBack(const SDL_Rect& rect);
	SDL_Rect& GetCurrentFrame();
	bool Finished() const;
	void Reset();
	void Clean();

private:
	float current_frame;
	int last_frame = 0;
	bool loop = true;
	int loops = 0;
	float speed = 1.0f;

};

#endif