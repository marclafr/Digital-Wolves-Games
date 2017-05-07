#ifndef _TASK
#define _TASK

#include "j1App.h"
#include "j1EntityManager.h"
#include "j1SceneManager.h"
#include "j1Scene.h"
#include "j1MainMenu.h"
#include "j1ScoreScene.h"
#include "UIComponents.h"
#include "Buildings.h"
#include "Towers.h"
#include "Units.h"

class Task
{
private:
	char* name;

public:
	virtual bool Execute() = 0;
};

//TASKS
class CreateTowerTask : public Task
{
private:
	BUILDING_TYPE type;
	fPoint position;
	Side side;

public:
	CreateTowerTask() : type(B_NO_BUILDING), position(fPoint(0.0f, 0.0f)), side(S_NO_SIDE)
	{}
	CreateTowerTask(BUILDING_TYPE type, fPoint position, Side side) : type(type), position(position), side(side)
	{}

	void Set(BUILDING_TYPE type, fPoint position, Side side)
	{
		this->type = type;
		this->position = position;
		this->side = side;
	}

	bool Execute()
	{
		App->entity_manager->CreateBuilding(type, position, side);

		return true;
	}
};

class ChangeMainMenuSceneToTask : public Task
{
private:
	SCENES to_scene = SC_NO_SCENE;

public:
	ChangeMainMenuSceneToTask(SCENES to_scene) : to_scene(to_scene) {}

	bool Execute()
	{
		App->scene_manager->ChangeScene(to_scene);

		if (to_scene == SC_GAME)
		{
			App->scene->win = false;
			App->scene->lose = false;
		}
		return true;
	}
};

class SetPreUpdateFalseTask : public Task
{
public:
	bool Execute()
	{
		App->main_menu->SetRetPreUpdate(false);
		return true;
	}
};

class ChangeScoreSceneToTask : public Task
{
private:
	SCENES to_scene = SC_NO_SCENE;

public:
	ChangeScoreSceneToTask(SCENES to_scene) : to_scene(to_scene) {}

	bool Execute()
	{
		App->scene_manager->ChangeScene(to_scene);

		if (to_scene == SC_GAME)
		{
			App->score_scene->SetSceneChange(true);
			App->scene->win = false;
			App->scene->lose = false;
		}
		else if (SC_MAIN_MENU)
		{
			if (!App->score_scene->IsSceneChange())
			{
				App->scene->win = false;
				App->scene->lose = false;
			}
		}
		return true;
	}
};

class ChangeBackGroundTask : public Task
{
private:
	SDL_Rect rect_atlas;
	BUTTONSUNDERGROUND underground;

public:
	ChangeBackGroundTask(const SDL_Rect & atlas, const BUTTONSUNDERGROUND underground) : rect_atlas(atlas), underground(underground) {}

	bool Execute()
	{
		App->score_scene->GetUnderBackground()->SetAtlas(rect_atlas);
		App->score_scene->ChangeUnselected(underground);
		return true;
	}
};

enum ENTITY_TASKTYPE
{
	ET_BASICTOWER,
	ET_BOMBARDTOWER,
	ET_WALL,
	ET_UNIT
};

class EntityTask : public Task
{
private:
	ENTITY_TASKTYPE type;

public:
	EntityTask(ENTITY_TASKTYPE type) : type(type) {}

	const ENTITY_TASKTYPE GetEntityType() const
	{
		return type;
	}
};


class PlaceBasicTowerTask : public EntityTask
{
public:
	PlaceBasicTowerTask(ENTITY_TASKTYPE type = ET_BASICTOWER) : EntityTask(type) {}

	bool Execute()
	{
		App->scene->placing_wall = false;
		App->scene->placing_tower = T_BASIC_TOWER;
		return true;
	}
};

class PlaceBombardTowerTask : public EntityTask
{
public:
	PlaceBombardTowerTask(ENTITY_TASKTYPE type = ET_BOMBARDTOWER) : EntityTask(type) {}

	bool Execute() 
	{
		App->scene->placing_wall = false;
		App->scene->placing_tower = T_BOMBARD_TOWER;
		return true;
	}
};

class PlaceWallTask : public EntityTask
{
public:
	PlaceWallTask(ENTITY_TASKTYPE type = ET_WALL) : EntityTask(type) {}

	bool Execute()
	{
		App->scene->placing_wall = true;
		App->scene->placing_tower = T_NO_TYPE;
		return true;
	}
};

class TrainUnitTask : public EntityTask
{
private:
	UNIT_TYPE u_type;
public:
	TrainUnitTask(UNIT_TYPE u_type, ENTITY_TASKTYPE type = ET_UNIT) : EntityTask(type), u_type(u_type) {}

	bool Execute()
	{
		if (App->scene->resources->CanTrainSoldier(U_TWOHANDEDSWORDMAN))
			App->scene->resources->TrainSoldier(U_TWOHANDEDSWORDMAN);
		return true;
	}

	const UNIT_TYPE GetUnitType()const
	{
		return u_type;
	}
};
#endif