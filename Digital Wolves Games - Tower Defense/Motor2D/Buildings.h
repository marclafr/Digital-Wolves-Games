#ifndef _BUILDINGS
#define _BUILDINGS

#include "SDL/include/SDL_rect.h"
#include "Entity.h"
#include "j1Textures.h"
#include "j1Module.h"
#include "Entity.h"
#include "Units.h"
#include "j1Timer.h"
#include <vector>
#include "j1Investigations.h"

enum BUILDING_TYPE
{
	B_NO_BUILDING = 0,
	B_TURRET,
	B_CANNON,
	B_WOOD_WALL,
	B_STONE_WALL,
	B_TOWNHALL//PER TENIR ALGO
};

class Building : public Entity
{
private:
	enum BUILDING_TYPE building_type;
	j1Timer buildtimer;
	j1Timer DieTimer;
	bool alive = true;
	bool totally_built = false;
	int base_height = 0;
	int base_width = 0;

public:
	iPoint arrowpos;
	Building(BUILDING_TYPE b_type, fPoint pos, Side side);
	~Building();

	virtual void Update(); // defines order
	virtual void AI();
	virtual void Draw();
	const BUILDING_TYPE GetBuildingType() const;
	void UpgradeWall();
	const double GetBuildTime() const;
	const double GetDieTime() const;
	const int GetHeight() const;
	const int GetWidth() const;
	bool IsBuilt() const;
	void BuildingComplete();
	bool IsAlive() const;
	void ConvertToRubble();
	void DestroyBuilding();
	const int GetRange() const;

};

#endif


