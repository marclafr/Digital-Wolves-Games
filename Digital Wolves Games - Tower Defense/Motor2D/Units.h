#ifndef _UNITS
#define _UNITS

#include "Entity.h"
#include "j1Timer.h"
#include <list>

#define XY_TILES_RELATION 2
#define TOWN_HALL iPoint(20,20) //TODO final townhall destination;

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

