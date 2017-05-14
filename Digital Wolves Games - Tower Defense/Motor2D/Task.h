#ifndef _TASK
#define _TASK

#include "j1App.h"
#include "j1EntityManager.h"
#include "j1SceneManager.h"
#include "j1Scene.h"
#include "j1MainMenu.h"
#include "j1ScoreScene.h"
#include "UIComponents.h"
#include "UIHUDMenuInGame.h"
#include "Buildings.h"
#include "Towers.h"
#include "Units.h"
#include "p2Log.h"
#include "j1Tutorial.h"
#include "j1WaveManager.h"

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

class ChangeMainMenuSceneToTutorial : public Task
{
private:
	SCENES to_scene = SC_NO_SCENE;

public:
	ChangeMainMenuSceneToTutorial(SCENES to_scene) : to_scene(to_scene) {}

	bool Execute()
	{
		App->scene_manager->ChangeScene(to_scene);

		if (to_scene == SC_GAME)
		{
			App->scene->win = false;
			App->scene->lose = false;
			App->tutorial->tutorial = true;
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
	ET_UNIT,
	ET_UPGRADE_TOWER,
	ET_UPGRADE_WALL,
	ET_INVESTIGATION,
	ET_DELETETOWER,
	ET_DELETEWALL
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
		if (App->tutorial->tutorial1_completed) App->tutorial->PanelSelected = true;
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
		if (App->tutorial->tutorial1_completed) App->tutorial->PanelSelected = true;
		return true;
	}
};

class UpgradeTowerTask : public EntityTask
{
private:
	Tower* tower = nullptr;
	TURRET_UPGRADE type = TU_NULL;

public:
	UpgradeTowerTask(TURRET_UPGRADE type, ENTITY_TASKTYPE et_type = ET_UPGRADE_TOWER) : EntityTask(et_type), type(type) {}

	const TURRET_UPGRADE GetUpgradeType()const
	{
		return type;
	}

	void SetTower(Tower* tower)
	{
		this->tower = tower;
	}

	bool Execute()
	{
		if (tower->IsAlive())
		{
			tower->UpgradeTurret(type);

			if (App->tutorial->tutorial4_completed)
			{
				App->tutorial->TowerUpgradeSelected = true;
			}

			App->entity_manager->UnselectEverything();
		}
		return true;
	}
};

class UpgradeWallTask : public EntityTask
{
private:
	Building* wall = nullptr;
	BUILDING_TYPE type;
public:
	UpgradeWallTask(BUILDING_TYPE type, ENTITY_TASKTYPE et_type = ET_UPGRADE_WALL) : EntityTask(et_type), type(type) {}

	const BUILDING_TYPE GetWallType() const
	{
		return type;
	}

	void SetWall(Building* wall)
	{
		this->wall = wall;
		type = wall->GetBuildingType();
	}

	bool Execute()
	{
		if (wall->IsAlive())	wall->UpgradeWall(type);
		App->entity_manager->UnselectEverything();
		return true;
	}
};

class DoInvestigation : public EntityTask
{
private:
	INVESTIGATION_TYPE type;

public:
	DoInvestigation(INVESTIGATION_TYPE type, ENTITY_TASKTYPE et_type = ET_INVESTIGATION) : EntityTask(et_type), type(type) {}

	const INVESTIGATION_TYPE GetInvestigationType() const
	{
		return type;
	}

	bool Execute()
	{
		App->investigations->WantToInvestigate(App->investigations->GetInvestigation(type));
		return true;
	}
};

class DeleteTowerTask : public EntityTask
{
private:
	Tower* tower = nullptr;
	TURRET_UPGRADE type = TU_NULL;

public:
	DeleteTowerTask(ENTITY_TASKTYPE type = ET_DELETETOWER) : EntityTask(type) {}

	void SetTower(Tower* tower)
	{
		this->tower = tower;
	}

	bool Execute()
	{
		if (tower->IsAlive())	tower->ConvertToRubble();
		App->entity_manager->UnselectEverything();
		return true;
	}
};

class DeleteWallTask : public EntityTask
{
private:
	Building* wall = nullptr;

public:
	DeleteWallTask(ENTITY_TASKTYPE type = ET_DELETEWALL) : EntityTask(type) {}

	void SetWall(Building* wall)
	{
		this->wall = wall;
	}

	bool Execute()
	{
		if (wall->IsAlive())	wall->ConvertToRubble();
		App->entity_manager->UnselectEverything();
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
		if (App->tutorial->tutorial1_completed) App->tutorial->PanelSelected = true;
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
		if (App->scene->resources->CanTrainSoldier(u_type))
		{
			App->scene->resources->TrainSoldier(u_type);
		}
			
		if (App->tutorial->tutorial1_completed) App->tutorial->PanelSelected = true;
		return true;
	}

	const UNIT_TYPE GetUnitType()const
	{
		return u_type;
	}
};

class ChangeToFacebook : public Task
{
public:
	bool Execute()
	{
		ShellExecute(NULL, "open", "https://www.facebook.com/Digital-Wolves-Games-233798633695568/?ref=bookmarks", NULL, NULL, SW_SHOWMAXIMIZED);
		return true;
	}
};

class ChangeToTwitter : public Task
{
public:
	bool Execute()
	{
		ShellExecute(NULL, "open", "https://twitter.com/DigitalWolvesG", NULL, NULL, SW_SHOWMAXIMIZED);
		return true;
	}
};

class ChangeToGithub : public Task
{
public:
	bool Execute()
	{
		ShellExecute(NULL, "open", "https://github.com/marclafr/Digital-Wolves-Games/wiki", NULL, NULL, SW_SHOWMAXIMIZED);
		return true;
	}
};

class ChangeToBugs : public Task
{
public:
	bool Execute()
	{
		ShellExecute(NULL, "open", "https://github.com/marclafr/Digital-Wolves-Games/issues", NULL, NULL, SW_SHOWMAXIMIZED);
		return true;
	}
};

class BringNextWave : public Task
{
public:
	bool Execute()
	{
		App->wave_manager->BringNextWave();
		if (App->tutorial->tutorial5_completed) App->tutorial->NextWaveButtonSelected = true;
		return true;
	}
};

class Surrender : public Task
{
public:
	bool Execute()
	{
		App->scene->lose = true;
		App->scene_manager->ChangeScene(SC_SCORE);
		return true;
	}
};

class ReturnToGame : public Task
{
private:
	UIHUDMenuInGame* menu_ingame;

public:
	ReturnToGame(UIHUDMenuInGame* menu_ingame) : menu_ingame(menu_ingame) {}

	bool Execute()
	{
		menu_ingame->DeletePanel();
		return true;
	}
};

class LoadLastChackpoint : public Task
{
public:
	bool Execute()
	{

		return true;
	}
};

class InGameToMainMenuScene : public Task
{
public:
	bool Execute()
	{
		App->scene_manager->ChangeScene(SC_MAIN_MENU);
		return true;
	}
};

#endif