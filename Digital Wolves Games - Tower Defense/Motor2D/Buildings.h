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
	NO_BUILDING = 0,
	TURRET //PER TENIR ALGO
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
public:

	Building(BUILDING_TYPE b_type, fPoint pos);

	void Update(); // defines order

	virtual void AI();
	virtual void Draw();

	const BUILDING_TYPE GetBuildingType() const;
	const int GetRange() const;
};

#endif


