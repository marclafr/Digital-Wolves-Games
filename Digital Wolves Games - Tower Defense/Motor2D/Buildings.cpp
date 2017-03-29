#include "Buildings.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"




Building::Building(BUILDING_TYPE b_type, fPoint pos) : Entity(BUILDING, pos), building_type(b_type)
{
	SDL_Rect rect;
	switch (b_type)
	{
	case TURRET:
		SetHp(1000);
		SetAttack(12);
		SetArmor(1);
		rate_of_fire = 1;
		range = 8;
		build_time = 7;
		rect = {610,1,107,206};
		SetRect(rect);
		SetPivot(0.53125 * 96, 0.59375 * 64);
		SetTextureID(T_TURRET);
		break;
	default:
		LOG("Error BUILDING TYPE STATS NULL");
		building_type = NO_BUILDING;
		break;
	}
	buildtimer.Start();
	iPoint p = App->map->WorldToMap(pos.x, pos.y);

	if (App->pathfinding->IsWalkable(p) == true)
	{
		App->pathfinding->MakeNoWalkable(p);
	}
}

void Building::Update()
{
	if (totallybuilded == true) 
	{
		AI();
	}
	Draw();
}

void Building::AI()
{
	if (Target == nullptr) 
	{
		std::vector<Entity*> EntityVector = App->entity_manager->GetEntityVector();
		std::vector<Entity*>::iterator item = EntityVector.begin();
		for (; item != EntityVector.end(); item++) 
		{
			if ((*item)->GetEntityType() == UNIT)
			{
				if ((*item)->GetX() >= (GetX() - 120) && (*item)->GetX() < (GetX() + 120) && (*item)->GetY() >= (GetY() - 120) && (*item)->GetY() < (GetY() + 120) && (*item)->GetHp() > 0)
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
		}
		if (AttackTimer.ReadMs() > 900)
		{
			if (Target != nullptr && Target->GetHp() > 0)
			{
				if (Target->GetX() >= (GetX() - 120) && Target->GetX() < (GetX() + 120) && Target->GetY() >= (GetY() - 120) && Target->GetY() < (GetY() + 120))
				{
					Attack(Target);
					AttackTimer.Start();
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
	
	if (buildtimer.ReadMs() <= 3000)
	{
		SDL_Rect rect = { 394,1,96,64 };
		SetRect(rect);
		SetPivot(0.53125 * 96, 0.59375 * 64);
	}
	else if (buildtimer.ReadMs() > 3000 && buildtimer.ReadMs() <= 6000) 
	{
		SDL_Rect rect = { 244,539,100,73};
		SetRect(rect);
		SetPivot(0.55*100, 0.643836*73);
	}
	else if (buildtimer.ReadMs() > 6000 && buildtimer.ReadMs() <= 9000)
	{
		SDL_Rect rect = { 346,539,100,73 };
		SetRect(rect);
		SetPivot(0.55 * 100, 0.643836 * 73);
	}
	else 
	{
		SDL_Rect rect = { 610,1,107,206 };
		SetRect(rect);
		SetPivot(0.504673*107, 0.902913*206);
		totallybuilded = true;
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
