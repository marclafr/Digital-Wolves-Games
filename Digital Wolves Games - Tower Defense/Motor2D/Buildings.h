#ifndef _BUILDINGS
#define _BUILDINGS

#include "SDL/include/SDL_rect.h"
#include "Entity.h"
#include "j1Textures.h"
#include "j1Module.h"
#include "Entity.h"
#include "Units.h"
#include "j1PerfTimer.h"
#include <vector>

enum BUILDING_TYPE
{
	B_NO_BUILDING = 0,
	B_TURRET,
	B_STONE_WALL,
	B_TOWNHALL//PER TENIR ALGO
};

class Building : public Entity
{
private:

	enum BUILDING_TYPE building_type;
	float rate_of_fire;
	float range;
	float build_time;
	j1PerfTimer buildtimer;
	j1PerfTimer AttackTimer;
	Entity* Target = nullptr;
	bool totallybuilded = false;
public:
	iPoint arrowpos;
	Building(BUILDING_TYPE b_type, fPoint pos, Side side);

	void Update(); // defines order
	void UpdateArrow(iPoint &arrow, iPoint Target);
	void UpdateArrowSprite(iPoint Target, SDL_Rect& rect, iPoint &pivots);
	virtual void AI();
	virtual void Draw();
	bool attacking = false;
	const BUILDING_TYPE GetBuildingType() const;
	const int GetRange() const;
	const double GetBuildTime() const;
};

#endif


