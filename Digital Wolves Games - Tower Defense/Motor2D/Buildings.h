#ifndef _BUILDINGS
#define _BUILDINGS

#include "SDL/include/SDL_rect.h"
#include "Entity.h"
#include "j1Textures.h"
#include "j1Module.h"
#include "Entity.h"
#include "Units.h"



enum BUILDING_TYPE
{
	NO_BUILDING = 0,
	TURRET //PER TENIR ALGO
};

class Building : public Entity
{
private:

	enum BUILDING_TYPE building_type;
	int attack;
	float rate_of_fire;
	float range;
	float build_time;

public:

	Building(BUILDING_TYPE b_type, fPoint pos);

	void Update(); // defines order

	virtual void AI();
	virtual void Draw();
};

#endif


