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




Resources::Resources(RESOURCE_TYPE r_type, fPoint pos) : Entity(E_RESOURCE, pos, S_ALLY), resource_type(r_type)
{
	SDL_Rect rect;
	switch (r_type)
	{
	case R_FOOD:
		SetSide(S_ALLY);
		SetHp(10);
		SetArmor(1);
		collect_time = 11.0f;
		amount_collected = 50;
		build_time = 7.0f;
		rect = { 203, 0, 222, 217 };
		SetRect(rect);
		SetPivot(0, 0);
		SetTextureID(T_RESOURCE);
		totallybuilded = true;
		CollectTimer.Start();
		break;

	case R_WOOD:
		SetSide(S_ALLY);
		SetHp(10);
		SetArmor(1);
		collect_time = 11.0f;
		amount_collected = 100;
		build_time = 7.0f;
		rect = { 638, 0, 208, 175 };
		SetRect(rect);
		SetPivot(0, 0);
		SetTextureID(T_RESOURCE);
		totallybuilded = true;
		CollectTimer.Start();
		break;

	case R_GOLD:
		SetSide(S_ALLY);
		SetHp(10);
		SetArmor(1);
		collect_time = 11.0f;
		amount_collected = 100;
		build_time = 7.0f;
		rect = { 638, 316, 213, 130 };	//TODO CHANGE RECT/PIVOT : SPRITE?????
		SetRect(rect);
		SetPivot(0, 0);
		SetTextureID(T_RESOURCE);
		totallybuilded = true;
		CollectTimer.Start();
		break;

	case R_STONE:
		SetSide(S_ALLY);
		SetHp(10);
		SetArmor(1);
		collect_time = 11.0f;
		amount_collected = 50;
		build_time = 7.0f;
		rect = { 638, 183, 213, 130 };
		SetRect(rect);
		SetPivot(0, 0);
		SetTextureID(T_RESOURCE);
		totallybuilded = true;
		CollectTimer.Start();
		break;

	default:
		LOG("Error BUILDING TYPE STATS NULL");
		resource_type = R_NO_RESOURCE;
		break;
	}
	buildtimer.Start();
	iPoint p = App->map->WorldToMap(pos.x, pos.y);

	if (App->pathfinding->IsWalkable(p) == true)
	{
		App->pathfinding->MakeNoWalkable(p);
	}
}

Resources::~Resources()
{
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
	if (CollectTimer.ReadSec() >= collect_time)
	{
		AddResource(amount_collected);
		CollectTimer.Start();
	}
}

void Resources::Draw()
{
	App->render->PushInGameSprite(this);
}

const RESOURCE_TYPE Resources::GetResourceType() const
{
	return resource_type;
}

const double Resources::GetBuildTime() const
{
	return buildtimer.Read();
}

int Resources::GetResource()
{
	return resource;
}

bool Resources::ReduceCollectTime(float reduction)
{
	collect_time -= reduction;
	if (collect_time < 1.0f)
	{
		collect_time = 1.0f;
		return false;
	}
	return true;
}

void Resources::IncreaseResourceAmount(int amount)
{
	amount_collected += amount;
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