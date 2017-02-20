#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "j1Module.h"
#include <vector>
#include <list>
#include "j1Units.h"
#define MAX_FRAMES 100

struct SDL_Texture;


class Animation
{
public:
	Animation(std::string name);
	~Animation();

	std::string name;
	UNITS_TYPE unit_type;
	ACTION_TYPE action_type;
	DIRECTION_TYPE direction_type;

	std::vector<SDL_Rect> frames;
	std::vector<fPoint> pivot_points;

	float current_frame;
	int last_frame = 0;
	bool loop = true;
	int loops = 0;
	float speed = 1.0f;

public:
	void SetSpeed(float speed);
	void SetLoopState(bool state);
	void PushBack(const SDL_Rect& rect);
	SDL_Rect& GetCurrentFrame();

	bool CleanUp();
	bool Finished() const;
	void Reset();

	void SetUnit(const pugi::xml_node node);
	void SetAction(const pugi::xml_node node);
	void SetDirection(const pugi::xml_node node);
};


class j1Animation:public j1Module
{
public:

	j1Animation();

	// Destructor
	virtual ~j1Animation();
	
	bool Awake(pugi::xml_node& config);

	bool Start();

	bool CleanUp();

private:
	std::list<SDL_Texture*> textures;
	std::list<Animation*> animations;

};

#endif