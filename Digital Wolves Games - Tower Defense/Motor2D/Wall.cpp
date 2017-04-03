#include "Wall.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"

Wall::Wall(WALL_TYPE w_type, fPoint pos) : Entity(WALL, pos, ALLY), wall_type(w_type)
{
	SDL_Rect rect;
	switch (w_type)
	{
	case STONE_WALL:
		SetSide(ALLY);
		SetHp(500);
		SetArmor(1);
		build_time = 7;
		rect = { 610,1,107,206 };
		SetRect(rect);
		SetPivot(0.504673 * 107, 0.902913 * 206);
		SetTextureID(T_TURRET);
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
}

void Wall::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && GetEntityStatus() == E_SELECTED) {
		this->SetHp(0);
	}
	if (totallybuilded == true)
	{
		AI();
	}
	Draw();
	if (GetHp() < 0) {
		this->Die();
	}
}

void Wall::AI()
{
	
}

void Wall::Draw()
{

	if (buildtimer.ReadMs() <= 3000)
	{
		SDL_Rect rect = { 394,1,96,64 };
		SetRect(rect);
		SetPivot(0.53125 * 96, 0.59375 * 64);
	}
	else if (buildtimer.ReadMs() > 3000 && buildtimer.ReadMs() <= 6000)
	{
		SDL_Rect rect = { 376,539,100,73 };
		SetRect(rect);
		SetPivot(0.55 * 100, 0.643836 * 73);
	}
	else if (buildtimer.ReadMs() > 6000 && buildtimer.ReadMs() <= 9000)
	{
		SDL_Rect rect = { 478,539,100,73 };
		SetRect(rect);
		SetPivot(0.55 * 100, 0.643836 * 73);
	}
	else
	{
		SDL_Rect rect = { 610,1,107,206 };
		SetRect(rect);
		SetPivot(0.504673 * 107, 0.902913 * 206);
		totallybuilded = true;
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
