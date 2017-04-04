#include "Resources.h"
#include "Buildings.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"




Resources::Resources(RESOURCE_TYPE r_type, fPoint pos) : Entity(RESOURCE, pos, ALLY), resource_type(r_type)
{
	SDL_Rect rect;
	switch (r_type)
	{
	case STONE:
		SetSide(ALLY);
		SetHp(10);
		SetArmor(1);
		collect_time = 10;
		build_time = 7;
		rect = { 1,146,171,101 };
		SetRect(rect);
		SetPivot(0.491228 * 171, 0.653465 * 101);
		SetTextureID(T_RESOURCE);
		totallybuilded = true;
		CollectTimer.Start();
		break;

	case WOOD:
		SetSide(ALLY);
		SetHp(10);
		SetArmor(1);
		collect_time = 10;
		build_time = 7;
		rect = { 242,0,156,138 };
		SetRect(rect);
		SetPivot(0.491228 * 171, 0.653465 * 101);
		SetTextureID(T_RESOURCE);
		totallybuilded = true;
		CollectTimer.Start();
		break;
	default:
		LOG("Error BUILDING TYPE STATS NULL");
		resource_type = NO_RESOURCE;
		break;
	}
	buildtimer.Start();
	iPoint p = App->map->WorldToMap(pos.x, pos.y);

	if (App->pathfinding->IsWalkable(p) == true)
	{
		App->pathfinding->MakeNoWalkable(p);
	}
}

void Resources::Update()
{
	if (GetHp() <= 0) 
	{
		this->Die();
	}
	if (totallybuilded == true)
	{
		AI();
	}
	Draw();
}

void Resources::AI()
{
	if (CollectTimer.ReadMs() >= collect_time * 1000) {
		AddResource(100);
		CollectTimer.Start();
	}
}

void Resources::Draw()
{
	App->render->PushEntity(this);
}

const RESOURCE_TYPE Resources::GetResourceType() const
{
	return resource_type;
}

const double Resources::GetBuildTime() const
{
	return buildtimer.ReadMs();
}

int Resources::GetResource()
{
	return resource;
}

void Resources::AddResource(int add) 
{
	resource += add;
}
bool Resources::CanUseResource(int cost)
{
	if (resource >= cost)
		return true;
	return false;
}
bool Resources::UseResource(int cost)
{
	if (resource >= cost)
	{
		resource -= cost;
		return true;
	}
	return false;
}
/*
Resources::Resources() {
	wood = 2000;
	food = 2000;
	gold = 2000;
	stone = 2000;
}

Resources::~Resources()
{}

int Resources::GetWood() 
{
	return wood;
}
int Resources::GetFood()
{
	return food;
}
int Resources::GetGold()
{
	return gold;
}
int Resources::GetStone()
{
	return stone;
}

void Resources::AddWood(int adding) 
{
	wood += adding;
}
void Resources::AddFood(int adding)
{
	food += adding;
}
void Resources::AddGold(int adding)
{
	gold += adding;
}
void Resources::AddStone(int adding)
{
	stone += adding;
}

void Resources::UseWood(int used)
{
	wood -= used;
}
void Resources::UseFood(int used)
{
	food -= used;
}
void Resources::UseGold(int used)
{
	gold -= used;

}
void Resources::UseStone(int used)
{
	stone -= used;
}
*/