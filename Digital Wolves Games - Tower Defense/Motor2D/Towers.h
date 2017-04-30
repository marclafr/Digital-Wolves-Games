#ifndef __TOWERS_
#define __TOWERS_

#include "Buildings.h"
#include "j1Timer.h"

#define HEIGHT_BASIC_TOWER 146
enum TOWER_TYPE
{
	T_NO_TYPE = 0,
	T_BASIC_TOWER,
	T_BOMBARD_TOWER

};

enum TOWER_ELEMENT_TYPE
{

};

class Tower : public Building
{
private:

	TOWER_TYPE tower_type = T_NO_TYPE;
	float rate_of_fire;
	float range;
	j1Timer AttackTimer;
	Entity* Target = nullptr;

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