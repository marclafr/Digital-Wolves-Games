#ifndef _UNITS
#define _UNITS

#include "Entity.h"
#include "j1Timer.h"
#include <list>

#define XY_TILES_RELATION 2

class Animation;
struct PathList;
struct PathNode;

enum UNIT_TYPE
{
	NO_UNIT = 0,
	VILLAGER,
	TWOHANDEDSWORDMAN,
	CAVALRYARCHER,
	SIEGERAM
};

enum UNIT_CLASS
{
	NO_CLASS = 0,
	INFANTRY,
	ARCHER,
	CAVALRY,
	SIEGE,
	SUICIDAL
};

enum ACTION_TYPE
{
	NO_ACTION = 0,
	ATTACK,
	DIE,
	DISAPPEAR,
	IDLE,
	WALK
};

enum DIRECTION
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

class Unit : public Entity
{
private:
	enum UNIT_TYPE unit_type;
	enum ACTION_TYPE action_type;
	enum DIRECTION direction;

	int attack;
	int range;
	float speed;
	float rate_of_fire;
	int unit_radius;
	iPoint destination;
	iPoint path_objective;
	fPoint move_vector;
	float angle;
	bool GetNextTile();
	UNIT_CLASS unit_class;
	const int GetRandNum(int num);
	int rand_num;
	Animation* animation;
	bool changed;

	std::list<iPoint> path_list;

	bool moving = false;
public:

	Unit(UNIT_TYPE u_type, fPoint pos);
	~Unit();
	
	void Update(); // defines order

	void Move();
	void AI();
	void Draw();
	void Die();
	

	const DIRECTION GetDir() const;
	const UNIT_TYPE GetUnitType() const;
	const UNIT_CLASS GetUnitClass() const;
	const ACTION_TYPE GetActionType() const;
	const int GetUnitRadius() const;
	int GetPath(iPoint dest);
	const int GetAttack() const;
	const int GetRange() const;

	//FX
	unsigned int fx_twohanded_die01;
	unsigned int fx_twohanded_die02;
	unsigned int fx_twohanded_die03;
	unsigned int fx_twohanded_die04;
	unsigned int fx_twohanded_die05;
	
};

#endif

