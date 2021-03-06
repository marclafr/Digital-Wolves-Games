#ifndef _UNITS
#define _UNITS

#include "PugiXml\src\pugixml.hpp"
#include <vector>
#include "Entity.h"
#include "j1Timer.h"
#include "IsoPrimitives.h"

#define XY_TILES_RELATION 2
#define TOWN_HALL iPoint(0, 272) //Final townhall destination;
#define SLOW_PROPORTION 1.7f
#define SLOW_TIME 5.0f

//range in tiles
#define CLOSE_COMBAT_RANGE 1
#define MID_COMBAT_RANGE 2
#define LONG_COMBAT_RANGE 3
#define EXTRA_LONG_COMBAT_RANGE 4	//MANGONEL

//close combat approach
#define APPROACH 65 //pixels from ennemy while attacking
#define VISION_RANGE 300 //pixels

//AI
#define CHECK_DESTINATION_FRAMES 3 //chechs if destination full every x frames

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
	A_WALK,
	A_APPROACH,
	A_CENTER
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

	int ai_update;

	int attack;
	int range;
	float speed;
	float rate_of_fire;
	iPoint destination; //tile
	iPoint path_objective;
	fPoint move_vector;
	float angle;
	Elipse unit_circle;
	UNIT_CLASS unit_class;
	int rand_num;
	AnimationManager* animation;
	AnimationManager* idle_siege;
	bool changed;
	Entity* target;
	bool siege_attacked = false;

	std::vector<iPoint> path_vec;

	bool slowed = false;
	j1Timer slow_timer;

	//Investigations bonuses:
	bool bonus_attack = false;
	bool bonus_defense = false;
	Unit* collision = nullptr;

	void UnitDies();

	bool OutOfHP() const;
	void EnemyInSight();
	void GoToEnemy();
	void ChangeDirecctionToEnemy();
	void GoIdle();
	bool DestinationFull() const;
	bool EnemyDead();
	void DoDamage();
	bool AproachEnemy();
	void SetAttackPosition();
	void StartAttack();
	void MoveAway();
	void GetNewDestination();
	void CheckUnitsBuffs();
	void GoToTileCenter();
	bool CenterUnit();
	void ChangeAnimation();
	int GetFrameAttack();
	void Collisions();

	bool GetNextTile();

public:

	Unit(UNIT_TYPE u_type, fPoint pos, Side side, int priority);
	~Unit();
	
	void Update( float dt); // defines order

	bool Walk();
	void AI();
	void Draw();

	const DIRECTION GetDir() const;
	const UNIT_TYPE GetUnitType() const;
	const UNIT_CLASS GetUnitClass() const;
	const ACTION GetAction() const;
	const int GetUnitRadius() const;
	bool GetPath(iPoint dest);
	bool GetTilePath(iPoint& tile);
	const int GetAttack() const;
	const int GetRange() const;
	const Elipse GetUnitCircle() const;
	const bool IsMoving() const;
	const iPoint& GetDestination() const;

	const Unit* GetCollision() const;
	void SetAction(const ACTION action);

	void LookAt(iPoint pos);
	bool GoTo(iPoint dest);
	bool GoToTile(iPoint& tile);
	bool ChangeDirection(iPoint dest);
	bool ChangeDirectionTile(iPoint& tile);
	void PlayDeathSound() const;
	void PlayAttackSound() const;

	void SlowUnit();

	void Save(pugi::xml_node& );
};

#endif

