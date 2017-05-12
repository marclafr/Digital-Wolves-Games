#ifndef _UNITS
#define _UNITS

#include <vector>
#include "Entity.h"
#include "j1Timer.h"
#include "IsoPrimitives.h"

#define XY_TILES_RELATION 2
#define TOWN_HALL iPoint(-75, 272) //TODO final townhall destination;
#define SLOW_PROPORTION 1.4f
#define SLOW_TIME 2.0f

class AnimationManager;
struct PathList;
struct PathNode;

enum UNIT_TYPE
{
	U_NO_UNIT = 0,
	U_VILLAGER,
	U_GOD,

	//INFANTRY
	U_MILITIA,
	U_MANATARMS,
	U_LONGSWORDMAN,
	U_TWOHANDEDSWORDMAN,
	U_CHAMPION,

	U_SPEARMAN,
	U_PIKEMAN,
	//ARCHERS
	U_ARCHER,
	U_ARBALEST,
	U_CAVALRYARCHER,
	U_HEAVYCAVALRYARCHER,
	//CAVALRY
	U_KNIGHT,
	U_CAVALIER,
	U_PALADIN,
	//SIEGE
	U_SIEGERAM,
	U_MANGONEL

	//TODO: ADD UNIT
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

enum ACTION
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

class Unit : public Entity
{
private:
	enum UNIT_TYPE unit_type;
	enum ACTION action;
	enum DIRECTION direction;

	int attack;
	int range;
	int vision_range;
	float speed;
	float rate_of_fire;
	iPoint destination;
	iPoint path_objective;
	fPoint move_vector;
	float angle;
	Circle unit_circle;
	UNIT_CLASS unit_class;
	int rand_num;
	AnimationManager* animation;
	AnimationManager* idle_siege;
	bool changed;
	Entity* target;
	Entity* attacking;

	std::vector<iPoint> path_vec;

	int priority;
	bool slowed = false;
	j1Timer slow_timer;

	//Investigations bonuses:
	bool bonus_attack = false;
	bool bonus_defense = false;

	void UnitDies();

public:

	Unit(UNIT_TYPE u_type, fPoint pos, Side side, int priority);
	~Unit();

	void Update(float dt); // defines order

	bool Move();
	void AI();
	void Draw();

	const DIRECTION GetDir() const;
	const UNIT_TYPE GetUnitType() const;
	const UNIT_CLASS GetUnitClass() const;
	const ACTION GetAction() const;
	const int GetUnitRadius() const;
	bool GetPath(iPoint dest);
	const int GetAttack() const;
	const int GetRange() const;
	const Circle GetUnitCircle() const;
	const int GetVisionRange() const;
	const bool IsMoving() const;

	void CheckCollisions() const;

	const int GetPriority() const;
	void SetAction(const ACTION action);

	//TODO:this should be private?
	bool GetNextTile();

	void LookAt(iPoint pos);
	bool GoTo(iPoint destination);
	bool ChangeDirection(iPoint destination);
	void PlayDeathSound() const;
	void PlayAttackSound() const;

	bool FindEmptyAttackPos(iPoint& pos) const;

	void SlowUnit();
};


#endif


