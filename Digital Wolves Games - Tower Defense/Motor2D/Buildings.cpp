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
#include "Camera.h"


Building::Building(BUILDING_TYPE b_type, fPoint pos, Side side) : Entity(E_BUILDING, pos, side), building_type(b_type)
{
	SDL_Rect rect;
	SetTextureID(T_TURRET);
	switch (b_type)
	{
	case B_TURRET:
		SetSide(side);
		build_rect = Rectng({ (int)GetX(), (int)GetY() }, 96, 47,	GetPivot());
		break;

	case B_WOOD_WALL:
		SetSide(side);
		SetHp(500);
		SetAttack(0);
		SetArmor(8);
		build_rect = Rectng({ (int)GetX(), (int)GetY() }, 96, 47, GetPivot());
		break;

	case B_TOWNHALL:
		SetSide(side);
		SetHp(1500);
		SetAttack(0);
		SetArmor(8);
		rect = { 477,0,366,317 };
		SetRect(rect);
		SetPivot(0.52459 * 366, 0.72555 * 317);
		SetTextureID(T_TOWNHALL);
		totally_built = true;
		build_rect = Rectng({ (int)GetX(), (int)GetY() }, 375, 170, {0,0});
		break;
	case B_UNIVERSITY:
		SetSide(side);
		SetHp(1500);
		SetAttack(0);
		SetArmor(8);
		rect = {0,158,483,291};
		SetRect(rect);
		SetPivot(0.509317*483, 0.726923*291);
		SetTextureID(T_TOWNHALL);
		totally_built = true;
		build_rect = Rectng({ (int)GetX(), (int)GetY() }, 483, 210, {0,-20});
		break;
	default:
		LOG("Error BUILDING TYPE STATS NULL");
		building_type = B_NO_BUILDING;
		break;
	}
	buildtimer.Start();
	iPoint p = App->map->WorldToMap(pos.x, pos.y);

	if (App->pathfinding->IsWalkable(p) == true && (building_type == B_TURRET || building_type == B_WOOD_WALL))
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

void Building::Update(float dt)
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
	if (totally_built != true)
	{
		if (buildtimer.ReadSec() <= 6)
		{
			SetRect({98,0,100,75});
			SetPivot(0.55 * 100, 75*0.643836);
		}
		
		else
		{
			SetRect({ 610,289,100,106 });
			SetPivot(0.49 * 100, 106 * 0.754717);
			totally_built = true;
		}
	}
	if (App->render->camera->InsideRenderTarget(App->render->camera->GetPosition().x + GetX(), App->render->camera->GetPosition().y + GetY()))
		App->render->PushInGameSprite(this);
}

const BUILDING_TYPE Building::GetBuildingType() const
{
	return building_type;
}

void Building::SetBuildingType(BUILDING_TYPE type)
{
	building_type = type;
}

void Building::UpgradeWall(BUILDING_TYPE type)
{
	if (this->IsBuilt())
	{
		switch (type)
		{
		case B_STONE_WALL:
			SetRect({ 1020,12,99,178});
			SetPivot(0.494949 * 99, 178 * 0.865169);
			building_type = B_STONE_WALL;
			break;
		case B_BRICK_WALL:
			SetRect({0,66,95,169});
			SetPivot(0.454211*96,169*0.899822);
			building_type = B_BRICK_WALL;
			break;
		default:
			break;
		}
	}
}

Rectng Building::GetBuildRectangle()
{
	return build_rect;
}

const double Building::GetBuildTime() const
{
	return buildtimer.ReadSec();
}

const double Building::GetDieTime() const
{
	return DieTimer.ReadSec();
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
	if (App->render->camera->InsideRenderTarget(App->render->camera->GetPosition().x + GetX(), App->render->camera->GetPosition().y + GetY())) App->audio->PlayFx(App->entity_manager->fx_building_destroyed);
	
	SetTextureID(T_TOWNHALL);
	SDL_Rect rect;
	rect = { 313, 1, 91, 51 };
	SetRect(rect);
	SetPivot(0.362637 * 91, 0.431373 * 51);
	DieTimer.Start();
	SetHp(0);
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