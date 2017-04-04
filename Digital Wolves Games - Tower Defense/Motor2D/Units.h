#ifndef _UNITS
#define _UNITS

#include "Entity.h"
#include "j1Timer.h"
#include <list>

#define XY_TILES_RELATION 2
#define TOWN_HALL iPoint(-768,696) //TODO final townhall destination;

class Animation;
struct PathList;
struct PathNode;

enum UNIT_TYPE
{
	U_NO_UNIT = 0,
	U_VILLAGER,
	U_TWOHANDEDSWORDMAN,
	U_CAVALRYARCHER,
	U_SIEGERAM
};

enum UNIT_CLASS
{
	C_NO_CLASS = 0,
	C_INFANTRY,
	C_ARCHER,
	C_CAVALRY,
	C_SIEGE,
	C_SUICIDAL
};

enum ACTION_TYPE
{
	A_NO_ACTION = 0,
	A_ATTACK,
	A_DIE,
	A_DISAPPEAR,
	A_IDLE,
	A_WALK
};

enum DIRECTION
{
	D_NO_DIRECTION = 0,
	D_NORTH,
	D_NORTH_EAST,
	D_EAST,
	D_SOUTH_EAST,
	D_SOUTH,
	D_SOUTH_WEST,
	D_WEST,
	D_NORTH_WEST
};

enum UNIT_STATE
{
	NONE = 0,
	FIGHTING,
	MOVING_TO_FIGHT,
	VIGILANT,
	MOVING,
	DEAD
};

class Unit : public Entity
{
private:
	enum UNIT_TYPE unit_type;
	enum ACTION_TYPE action_type;
	enum DIRECTION direction;

	int attack;
	int range;
	int vision_range;
	float speed;
	float rate_of_fire;
	int unit_radius;
	iPoint destination;
	iPoint path_objective;
	fPoint move_vector;
	float angle;
	UNIT_CLASS unit_class;
	const int GetRandNum(int num);
	int rand_num;
	Animation* animation;
	bool changed;
	Entity* attacking;
	UNIT_STATE state;

	std::list<iPoint> path_list;

	int priority;

public:

	Unit(UNIT_TYPE u_type, fPoint pos, Side side, int priority);
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
	const UNIT_STATE GetUnitState() const;
	const int GetUnitRadius() const;
	int GetPath(iPoint dest);
	const int GetAttack() const;
	const int GetRange() const;
	const int GetVisionRange() const;
	const bool IsMoving() const;

	const int GetPriority() const;
	void PopFirstPath();
	void SetAction(const ACTION_TYPE action);
	void SetIsMoving();

	//TODO:this should be private?
	bool GetNextTile();


	void LookAt(iPoint pos);
	bool GoTo(iPoint destination);
	
	//FX
	unsigned int fx_twohanded_die01;
	unsigned int fx_twohanded_die02;
	unsigned int fx_twohanded_die03;
	unsigned int fx_twohanded_die04;
	unsigned int fx_twohanded_die05;
	
};

#endif

