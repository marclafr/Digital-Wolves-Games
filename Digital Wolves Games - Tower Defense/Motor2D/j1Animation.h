#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "j1Module.h"
#include <vector>
#include <list>
#include "Entity.h"
#include "Units.h"
#include "Buildings.h"
#include "j1Timer.h"
#include "Units.h"

#define IDLE_ANIMATION_WAIT 2.0f
#define START_SPEED 100.0f

class Unit;

enum ANIMATION_NAME
{
	NO_ANIM_NAME = 0,
	ANIM_UNIT,
	ANIM_SIMPLE_ARROW,
	ANIM_FIRE_ARROW,
	ANIM_ICE_ARROW,
	ANIM_AIR_ARROW,
	ANIM_SIMPLE_BOMB,
	ANIM_FIRE_BOMB,
	ANIM_ICE_BOMB,
	ANIM_AIR_BOMB,
	ANIM_BUILDINGS_FIRE,
	ANIM_FIRE_EXPLOSION,
	ANIM_FIRE_FLOOR,
	ANIM_ICE_EXPLOSION,
	ANIM_ICE_FLOOR
};

class Animation
{
	friend class j1Animation;

public:
	Animation(ANIMATION_NAME name);
	~Animation();

private:
	ANIMATION_NAME name;
	UNIT_TYPE unit_type;
	ACTION action;
	DIRECTION direction_type;

	std::vector<SDL_Rect> frames;
	std::vector<iPoint> pivot_points;

	float speed = 100.0f; //More speed equals slow down the animation
	bool loop = true;

public:
	void SetSpeed(float speed);
	void SetLoopState(bool state);

	const int GetNumFrames() const;
	const float GetSpeed() const;
	const bool GetLoopState() const;
	const SDL_Rect GetFrame(int frame_num) const;
	const iPoint GetPivot(int frame_num) const;
	const ACTION GetActionType() const;
	const UNIT_TYPE GetUnitType() const;
	const DIRECTION GetDirection() const;
	const ANIMATION_NAME GetName() const;

private:
	bool CleanUp();
	void SetUnit(const pugi::xml_node node);
	void SetAction(const pugi::xml_node node);
	void SetDirection(const pugi::xml_node node);
};

class AnimationManager
{
private:
	Animation* anim_type;
	float current_frame;
	j1Timer	anim_timer;
	j1Timer	idle_wait_timer;
	float speed; //More speed equals slow down the animation
	bool loop;
	bool wait_started;
	bool finished;

public:
	AnimationManager();
	AnimationManager(Animation* type); //remember
	AnimationManager(const AnimationManager& copy);
	~AnimationManager();

	void ChangeAnimation(Animation* type, float speed = 0.0f);

	const AnimationManager operator = (const AnimationManager& anim);

	bool Update(SDL_Rect& rect, iPoint& pivot_point);

	bool Finished();
	void Reset();

};

//------------------------------------------------------------------------//

class j1Animation :public j1Module
{
public:

	j1Animation();

	// Destructor
	virtual ~j1Animation();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool CleanUp();

	Animation* GetAnimationType(const ANIMATION_NAME name, const UNIT_TYPE unit = U_NO_UNIT, const ACTION action = A_NO_ACTION, const DIRECTION direction = D_NO_DIRECTION) const;

private:
	std::vector<Animation*> animation_types;
	ANIMATION_NAME AnimString2Enum(const std::string name);

};

#endif