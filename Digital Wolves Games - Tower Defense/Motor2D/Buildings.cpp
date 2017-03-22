#include "Buildings.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "p2Log.h"
#include "j1EntityManager.h"




Building::Building(BUILDING_TYPE b_type, fPoint pos) : Entity(BUILDING, pos), building_type(b_type)
{
	SDL_Rect rect;
	switch (b_type)
	{
	case TURRET:
		SetHp(1000);
		attack = 12;
		SetArmor(1);
		rate_of_fire = 1;
		range = 8;
		build_time = 7;
		rect = {610,1,107,206};
		SetRect(rect);
		SetPivot(0.504673, 0.902913);
		SetTextureID(T_TURRET);
		break;
	default:
		LOG("Error BUILDING TYPE STATS NULL");
		building_type = NO_BUILDING;
		break;
	}
	buildtimer.Start();
}

void Building::Update()
{
	//AI();
	Draw();
}

void Building::AI()
{
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
	}
	App->render->PushEntity(this);
}