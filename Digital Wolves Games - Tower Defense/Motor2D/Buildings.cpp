#include "Buildings.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include "j1Audio.h"

Building::Building(BUILDING_TYPE b_type, fPoint pos, Side side) : Entity(E_BUILDING, pos, side), building_type(b_type)
{
	SDL_Rect rect;
	switch (b_type)
	{
	case B_TURRET:
		SetSide(side);
		base_height = 25;
		base_width = 25;
		break;

	case B_STONE_WALL:
		SetSide(side);
		SetHp(500);
		SetAttack(0);
		rect = { 365,435,95,57 };
		SetRect(rect);
		SetPivot(0.505263 * 95, 0.578947 * 57);
		SetTextureID(T_WALL);
		base_height = 25;
		base_width = 25;
		break;

	case B_TOWNHALL:
		SetSide(side);
		SetHp(1500);
		SetAttack(0);
		rect = { 836,1,366,317 };
		SetRect(rect);
		SetPivot(0.52459 * 366, 0.72555 * 317);//
		SetTextureID(T_TOWNHALL);
		totally_built = true;
		base_height = 125;
		base_width = 125;
		break;
	default:
		LOG("Error BUILDING TYPE STATS NULL");
		building_type = B_NO_BUILDING;
		break;
	}
	buildtimer.Start();
	iPoint p = App->map->WorldToMap(pos.x, pos.y);

	if (App->pathfinding->IsWalkable(p) == true && (building_type == B_TURRET || building_type == B_STONE_WALL))
	{
		App->pathfinding->MakeNoWalkable(p);
	}
	if (side == S_NEUTRAL) {
		App->pathfinding->MakeNoConstruible_neutral(p);
	}
	if (side == S_ALLY) {
		App->pathfinding->MakeNoConstruible_ally(p);
	}
}

Building::~Building()
{
}

void Building::Update()
{
	Draw();

	if (this->GetHp() <= 0 && alive == true)
		ConvertToRubble();

	if (alive == false && DieTimer.ReadSec() >= 2)
		DestroyBuilding();
}

void Building::AI()
{

}

void Building::Draw()
{
	if (totally_built != true) {
		if (buildtimer.ReadSec() <= 3)
		{
			SDL_Rect rect = { 365,435,95,57 };
			SetRect(rect);
			SetPivot(0.505263 * 95, 0.578947 * 57);
		}
		else if (buildtimer.ReadSec() > 3 && buildtimer.ReadSec() <= 6)
		{
			SDL_Rect rect = { 462,435,94,99 };
			SetRect(rect);
			SetPivot(0.5 * 94, 0.757576 * 99);
		}
		else if (buildtimer.ReadSec() > 6 && buildtimer.ReadSec() <= 9)
		{
			SDL_Rect rect = { 558,435,94,147 };
			SetRect(rect);
			SetPivot(0.5 * 94, 0.829932 * 147);
		}
		else if (buildtimer.ReadSec() > 9 && buildtimer.ReadSec() <= 11)
		{
			SDL_Rect rect = { 1,584,100,147 };
			SetRect(rect);
			SetPivot(0.5 * 100, 0.829932 * 147);
		}
		else
		{
			SDL_Rect rect = { 325,218,99,178 };
			SetRect(rect);
			SetPivot(0.494949 * 99, 178 * 0.865169);
			totally_built = true;
		}
	}
	App->render->PushEntity(this);
}

const BUILDING_TYPE Building::GetBuildingType() const
{
	return building_type;
}

const double Building::GetBuildTime() const
{
	return buildtimer.ReadSec();
}

const double Building::GetDieTime() const
{
	return DieTimer.ReadSec();
}

const int Building::GetHeight() const
{
	return base_height;
}

const int Building::GetWidth()const
{
	return base_width;
}

bool Building::IsBuilt() const
{
	return totally_built;
}

void Building::BuildingComplete()
{
	totally_built = true;
}

bool Building::IsAlive() const
{
	return alive;
}

void Building::ConvertToRubble()
{
	//TODO: If townhall, change the rect
	SetTextureID(T_TOWNHALL);
	SDL_Rect rect;
	rect = { 313, 1, 91, 51 };
	SetRect(rect);
	SetPivot(0.362637 * 91, 0.431373 * 51);
	DieTimer.Start();
	alive = false;
	totally_built = true;
}

void Building::DestroyBuilding()
{
	iPoint p = App->map->WorldToMap(GetX(), GetY());
	if (GetSide() == S_NEUTRAL) {
		App->pathfinding->MakeConstruible_neutral(p);
	}
	if (GetSide() == S_ALLY) {
		App->pathfinding->MakeConstruible_ally(p);
	}
	App->pathfinding->MakeWalkable(p);
	this->Die();
}

const int Building::GetRange() const
{
	//TODO
	return 250;
}

void Building::UpdateArrow(iPoint &arrowpos, iPoint targetpos)
{
	if (arrowpos.x < targetpos.x && arrowpos.y < targetpos.y) {

		arrowpos.x += 2;
		arrowpos.y += 2;
	}
	if (arrowpos.x < targetpos.x && arrowpos.y > targetpos.y) {

		arrowpos.x += 2;
		arrowpos.y -= 2;
	}
	if (arrowpos.x > targetpos.x && arrowpos.y > targetpos.y) {

		arrowpos.x -= 2;
		arrowpos.y -= 2;
	}
	if (arrowpos.x > targetpos.x && arrowpos.y < targetpos.y) {

		arrowpos.x -= 2;
		arrowpos.y += 2;
	}
	if (arrowpos.x > targetpos.x && arrowpos.y == targetpos.y) {
		arrowpos.x -= 2;
	}
	if (arrowpos.x < targetpos.x && arrowpos.y == targetpos.y) {
		arrowpos.x += 2;
	}
	if (arrowpos.x == targetpos.x && arrowpos.y < targetpos.y) {
		arrowpos.y += 2;
	}
	if (arrowpos.x == targetpos.x && arrowpos.y > targetpos.y) {
		arrowpos.y -= 2;
	}
}
void Building::UpdateArrowSprite(iPoint targetpos, SDL_Rect& rect, iPoint &pivots)
{
	if (arrowpos.x < targetpos.x && arrowpos.y < targetpos.y) {

		rect = { 1,78,12,13 };
		pivots.x = 0.5 * 12;
		pivots.y = 0.384615 * 13;
	}
	else if (arrowpos.x < targetpos.x && arrowpos.y > targetpos.y) {

		rect = { 1,78,12,13};
		pivots.x = 0.5 * 12;
		pivots.y = 0.384615 * 13;
	}
	else if (arrowpos.x > targetpos.x && arrowpos.y > targetpos.y) {

		rect = { 55,1,13,12 };
		pivots.x = 0.538462 * 13;
		pivots.y = 0.5 * 12;
	}
	else if (arrowpos.x > targetpos.x && arrowpos.y < targetpos.y) {
		rect = { 55,1,13,12 };
		pivots.x = 0.538462 * 13;
		pivots.y = 0.5 * 12;
		
	}
	else if (arrowpos.x > targetpos.x && arrowpos.y == targetpos.y) {
		rect = { 1,45,25,4 };
		pivots.x = 0.518519 * 25;
		pivots.y = 0.333333 * 4;
	}
	else if (arrowpos.x < targetpos.x && arrowpos.y == targetpos.y) {
		rect = { 1,45,25,4 };
		pivots.x = 0.518519 * 25;
		pivots.y = 0.333333 * 4;
	}
	else if (arrowpos.x == targetpos.x && arrowpos.y < targetpos.y) {
		rect = { 52,78,3,15 };
		pivots.x = 0.666667 * 3;
		pivots.y = 0.4 * 15;
	}
	else if (arrowpos.x == targetpos.x && arrowpos.y > targetpos.y) {
		rect = { 1,1,3,13 };
		pivots.x = 0.518519 * 3;
		pivots.y = 0.461538 * 13;
	}
}