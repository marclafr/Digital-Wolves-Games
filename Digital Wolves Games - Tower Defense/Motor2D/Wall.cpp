#include "Wall.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"

Wall::Wall(WALL_TYPE w_type, fPoint pos) : Entity(E_WALL, pos, S_ALLY), wall_type(w_type)
{
	SDL_Rect rect;
	switch (w_type)
	{
	case STONE_WALL:
		SetSide(S_ALLY);
		SetHp(350);
		SetArmor(1);
		build_time = 7;
		rect = { 365,435,95,57 };
		SetRect(rect);
		SetPivot(0.505263*95, 0.578947*57);
		SetTextureID(T_WALL);
		break;
	default:
		LOG("Error BUILDING TYPE STATS NULL");
		wall_type = NO_WALL;
		break;
	}
	buildtimer.Start();
	iPoint p = App->map->WorldToMap(pos.x, pos.y);

	if (App->pathfinding->IsWalkable(p) == true)
	{
		App->pathfinding->MakeNoWalkable(p);
	}
	if(GetSide() == S_ALLY)
	{
		App->pathfinding->MakeNoConstruible_ally(p);
	}
}

void Wall::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && GetEntityStatus() == ST_SELECTED) {
		this->SetHp(0);
	}
	if (totallybuilded == true)
	{
		AI();
	}
	Draw();
	if (GetHp() < 0) {
		iPoint p = App->map->WorldToMap(GetX(), GetY());
		App->pathfinding->MakeNoWalkable(p);
		App->pathfinding->MakeNoConstruible_ally(p);
		this->Die();
	}
}

void Wall::AI()
{
	
}

void Wall::Draw()
{
	if (totallybuilded != true) {
		if (buildtimer.ReadMs() <= 3000)
		{
			SDL_Rect rect = { 365,435,95,57 };
			SetRect(rect);
			SetPivot(0.505263 * 95, 0.578947 * 57);
		}
		else if (buildtimer.ReadMs() > 3000 && buildtimer.ReadMs() <= 6000)
		{
			SDL_Rect rect = { 462,435,94,99 };
			SetRect(rect);
			SetPivot(0.5 * 94, 0.757576 * 99);
		}
		else if (buildtimer.ReadMs() > 6000 && buildtimer.ReadMs() <= 9000)
		{
			SDL_Rect rect = { 558,435,94,147 };
			SetRect(rect);
			SetPivot(0.5 * 94, 0.829932 * 147);
		}
		else if (buildtimer.ReadMs() > 9000 && buildtimer.ReadMs() <= 11000)
		{
			SDL_Rect rect = { 1,584,100,147 };
			SetRect(rect);
			SetPivot(0.5 * 100, 0.829932 * 147);

		}
		else {
			SDL_Rect rect = { 325,218,99,178 };
			SetRect(rect);
			SetPivot(0.494949 * 99, 178 * 0.865169);
			totallybuilded = true;
		}
	}
	App->render->PushEntity(this);
}

const WALL_TYPE Wall::GetWallType() const
{
	return wall_type;
}

const double Wall::GetBuildTime() const
{
	return buildtimer.ReadMs();
}
