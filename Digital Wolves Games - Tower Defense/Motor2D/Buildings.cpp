#include "Buildings.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"




Building::Building(BUILDING_TYPE b_type, fPoint pos, Side side) : Entity(E_BUILDING, pos, side), building_type(b_type)
{
	SDL_Rect rect;
	switch (b_type)
	{
	case B_TURRET:
		SetSide(side);
		SetHp(700);
		SetAttack(30);
		SetArmor(1);
		rate_of_fire = 1;
		range = 250;
		build_time = 7;
		rect = { 610,1,107,206 };
		SetRect(rect);
		SetPivot(0.504673 * 107, 0.902913 * 206);
		SetTextureID(T_TURRET);
		break;
	case B_STONE_WALL:
		SetSide(side);
		SetHp(300);
		SetAttack(0);
		rate_of_fire = 0;
		range = 0;
		rect = { 365,435,95,57 };
		SetRect(rect);
		SetPivot(0.505263 * 95, 0.578947 * 57);
		SetTextureID(T_WALL);
		break;
	case B_TOWNHALL:
		SetSide(side);
		SetHp(1500);
		SetAttack(0);
		rate_of_fire = 0;
		range = 0;
		rect = { 836,1,366,317 };
		SetRect(rect);
		SetPivot(0.52459 * 192, 0.72555 * 230);//
		SetTextureID(T_TOWNHALL);
		totallybuilded = true;
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

void Building::Update()
{

	if (totallybuilded == true && GetBuildingType() == B_TURRET)
	{
		AI();
	}
	Draw();

	if (Target != nullptr) {
		if (Target->GetHp() <= 0) {
			attacking = false;
			Target = nullptr;
		}
	}
	if (attacking == true) {
		iPoint targetpos;
		targetpos.x = Target->GetX();
		targetpos.y = Target->GetY() - 35;
		UpdateArrow(arrowpos, targetpos);
		if (arrowpos.x > targetpos.x - 3 && arrowpos.x < targetpos.x + 3 && arrowpos.y < targetpos.y + 3 && arrowpos.y > targetpos.y- 3)
		{
			attacking = false;
			Target->Damaged(20);
		}
		SDL_Rect rect = { 1,45,25,4 };
		iPoint pivots(0.5 * 25, 0.25 * 4);
		UpdateArrowSprite(targetpos, rect, pivots);
		App->render->Blit(App->tex->GetTexture(T_ARROW), arrowpos.x, arrowpos.y, &rect, SDL_FLIP_NONE, pivots.x, pivots.y);
	}
	if (GetHp() < 0) {
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
}

void Building::AI()
{
	if (Target == nullptr)
	{
		attacking = false;
		std::vector<Entity*> EntityVector = App->entity_manager->GetEntityVector();
		std::vector<Entity*>::iterator item = EntityVector.begin();
		for (; item != EntityVector.end(); item++)
		{
			if ((*item)->GetEntityType() == E_UNIT)
			{
				//TODO:Use a function from entity instead
				if ((*item)->GetX() >= (GetX() - GetRange()) && (*item)->GetX() < (GetX() + GetRange()) && (*item)->GetY() >= (GetY() - GetRange()) && (*item)->GetY() < (GetY() + GetRange()) && (*item)->GetHp() > 0 && (*item)->GetSide() == S_ENEMY)
				{
					Target = *item;
					AttackTimer.Start();
				}
			}
		}
	}
	else {
		if (Target->GetHp() <= 0)
		{
			Target = nullptr;
			attacking = false;
		}
		else
		{
			if (Target != nullptr && Target->GetSide() != S_ALLY && Target->GetHp() > 0 && attacking == false)
			{
				if (Target->GetX() >= (GetX() - GetRange()) && Target->GetX() < (GetX() + GetRange()) && Target->GetY() >= (GetY() - GetRange()) && Target->GetY() < (GetY() + GetRange()))
				{
					arrowpos.x = GetX();
					arrowpos.y = GetY() - 100;
					attacking = true;
				}
				else
				{
					Target = nullptr;
				}
			}
		}
	}
	//std::list<Entity>::iterator ptarget = App->entity_manager->
}

void Building::Draw()
{
	if (totallybuilded != true) {
		if (buildtimer.ReadMs() <= 3000)
		{
			if (GetBuildingType() == B_TURRET) {
				SDL_Rect rect = { 394,1,96,64 };
				SetRect(rect);
				SetPivot(0.53125 * 96, 0.59375 * 64);
			}
			else
			{
				SDL_Rect rect = { 365,435,95,57 };
				SetRect(rect);
				SetPivot(0.505263 * 95, 0.578947 * 57);
			}
		}
		else if (buildtimer.ReadMs() > 3000 && buildtimer.ReadMs() <= 6000)
		{
			if (GetBuildingType() == B_TURRET) {
				SDL_Rect rect = { 376,539,100,73 };
				SetRect(rect);
				SetPivot(0.55 * 100, 0.643836 * 73);
			}
			else
			{
				SDL_Rect rect = { 462,435,94,99 };
				SetRect(rect);
				SetPivot(0.5 * 94, 0.757576 * 99);
			}
		}
		else if (buildtimer.ReadMs() > 6000 && buildtimer.ReadMs() <= 9000)
		{
			if (GetBuildingType() == B_TURRET) {
				SDL_Rect rect = { 478,539,100,73 };
				SetRect(rect);
				SetPivot(0.55 * 100, 0.643836 * 73);
			}
			else
			{
				SDL_Rect rect = { 558,435,94,147 };
				SetRect(rect);
				SetPivot(0.5 * 94, 0.829932 * 147);
			}
		}
		else if (buildtimer.ReadMs() > 9000 && buildtimer.ReadMs() <= 11000)
		{
			if (GetBuildingType() == B_TURRET) {
				SDL_Rect rect = { 610,1,107,206 };
				SetRect(rect);
				SetPivot(0.504673 * 107, 0.902913 * 206);
				totallybuilded = true;
			}
			else
			{
				SDL_Rect rect = { 1,584,100,147 };
				SetRect(rect);
				SetPivot(0.5 * 100, 0.829932 * 147);
			}
		}
		else
		{
			SDL_Rect rect = { 325,218,99,178 };
			SetRect(rect);
			SetPivot(0.494949 * 99, 178 * 0.865169);
			totallybuilded = true;
		}
	}
	App->render->PushEntity(this);
}

const BUILDING_TYPE Building::GetBuildingType() const
{
	return building_type;
}

const int Building::GetRange() const
{
	return range;
}

const double Building::GetBuildTime() const
{
	return buildtimer.ReadMs();
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