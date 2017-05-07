#ifndef __TOWERS_
#define __TOWERS_

#include "Buildings.h"
#include "j1Timer.h"
#include "Projectile.h"
#define HEIGHT_BASIC_TOWER 146
#define ELEMENT_TERRAIN_TIME 3
class AnimationManager;

enum TOWER_TYPE
{
	T_NO_TYPE = 0,
	T_BASIC_TOWER,
	T_FIRE_TOWER,
	T_ICE_TOWER,
	T_AIR_TOWER,
	T_BOMBARD_TOWER,
	T_BOMBARD_FIRE_TOWER,
	T_BOMBARD_ICE_TOWER,
	T_BOMBARD_AIR_TOWER
};

enum TOWER_ELEMENT_TYPE
{
	TE_NULL = 0,
	TE_NO_ELEMENT,
	TE_FIRE,
	TE_ICE,
	TE_AIR
};

class Tower : public Building
{
private:

	TOWER_TYPE tower_type = T_NO_TYPE;
	PROJECTILE_TYPE projectile_type;
	float rate_of_fire;
	int projectile_spd;
	float range;
	j1Timer AttackTimer;
	Entity* Target = nullptr;
	j1Timer PrintElementTerrainTimer;
	fPoint element_terrain_pos;
	AnimationManager* anim_fire = nullptr;
	AnimationManager* anim_ice_floor = nullptr;
	void PrintElementTerrain(TOWER_ELEMENT_TYPE element, fPoint center, int radius);
	TOWER_ELEMENT_TYPE GetElementFromTower(TOWER_TYPE tower);
public:
	iPoint arrowpos;
	Tower(TOWER_TYPE t_type, fPoint pos);
	~Tower();

	void Update(); // defines order
	void AI();
	void Draw();
	bool attacking = false;
	const TOWER_TYPE GetTowerType() const;
	const int GetRange() const;
	void UpgradeTurret();
};

#endif //__TOWERS_