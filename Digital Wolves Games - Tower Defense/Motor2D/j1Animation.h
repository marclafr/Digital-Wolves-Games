#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "j1Module.h"
#include <vector>
#include <list>
#define MAX_FRAMES 100

struct SDL_Texture;

//Enums
enum UNIT_TYPE
{
	NO_UNIT = 0,
	TWOHANDEDSWORDMAN

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

class j1Animation:public j1Module
{
public:

	j1Animation();

	// Destructor
	virtual ~j1Animation();


	std::vector<SDL_Rect> frames;
	std::list<j1Animation*> animations;	// must be a group of animations

public:

	void SetSpeed(float speed);
	void SetLoopState(bool state);
	void PushBack(const SDL_Rect& rect);
	SDL_Rect& GetCurrentFrame();


	bool Awake(pugi::xml_node& config);

	bool Start();

	bool CleanUp();

	bool Finished() const;
	void Reset();

private:
	float current_frame;
	int last_frame = 0;
	bool loop = true;
	int loops = 0;
	float speed = 1.0f;

	SDL_Texture* twohandedswordman_texture;

};

#endif