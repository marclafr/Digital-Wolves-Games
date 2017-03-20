#include "Buildings.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "p2Log.h"




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
		rect = { 610,1,107,206 };
		SetRect(rect);
		SetPivot(0.504673, 0.902913);
		SetTextureID(T_TURRET);
		break;
	default:
		LOG("Error BUILDING TYPE STATS NULL");
		building_type = NO_BUILDING;
		break;
	}

}

void Building::Update()
{
	//AI();
	Draw();
}

void Building::AI()
{

}

void Building::Draw()
{
	App->render->PushEntity(this);
}