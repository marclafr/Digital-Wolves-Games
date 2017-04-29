#ifndef _TASK
#define _TASK

#include "j1App.h"
#include "j1EntityManager.h"
#include "j1SceneManager.h"
#include "j1Scene.h"
#include "j1MainMenu.h"

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
	CreateTowerTask() : type(B_NO_BUILDING), position(fPoint(0.0f,0.0f)), side(S_NO_SIDE)
	{}
	CreateTowerTask(BUILDING_TYPE type,	fPoint position, Side side): type(type), position(position), side(side)
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
	}
};

class ChangeSceneTask : public Task
{
private:
	SCENES to_scene = SC_NO_SCENE;

public:
	ChangeSceneTask(SCENES to_scene) : to_scene(to_scene) {}

	bool Execute()
	{
		App->scene_manager->ChangeScene(to_scene);

		if (to_scene == SC_GAME)
		{
			App->scene->win = false;
			App->scene->lose = false;
		}
	}
};

class SetPreUpdateFalseTask : public Task
{
public:
	bool Execute()
	{
		App->main_menu->SetRetPreUpdate(false);
	}
};

class ChangeBackgroundScoreSceneTask : public Task
{
private:
	SDL_Rect to_atlas;

public:

};
#endif