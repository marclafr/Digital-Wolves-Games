#ifndef _TASK
#define _TASK

#include "p2Point.h"
class Tower;
class Building;
class UIHUDMenuInGame;
enum BUILDING_TYPE;
enum Side;
enum SCENES;
enum BUTTONSUNDERGROUND;
enum TURRET_UPGRADE;
enum UNIT_TYPE;
enum INVESTIGATION_TYPE;
struct SDL_Rect;
struct info_button;

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
	CreateTowerTask();

	CreateTowerTask(BUILDING_TYPE type, fPoint position, Side side);

	void Set(BUILDING_TYPE type, fPoint position, Side side);

	bool Execute();
};

class ChangeMainMenuSceneToTask : public Task
{
private:
	SCENES to_scene;

public:
	ChangeMainMenuSceneToTask(SCENES to_scene);

	bool Execute();
};

class ChangeMainMenuSceneToTutorial : public Task
{
private:
	SCENES to_scene;

public:
	ChangeMainMenuSceneToTutorial(SCENES to_scene);

	bool Execute();
};

class SetPreUpdateFalseTask : public Task
{
public:
	bool Execute();
};

class ChangeScoreSceneToTask : public Task
{
private:
	SCENES to_scene;

public:
	ChangeScoreSceneToTask(SCENES to_scene);

	bool Execute();
};

class ChangeBackGroundTask : public Task
{
private:
	SDL_Rect rect_atlas;
	BUTTONSUNDERGROUND underground;

public:
	ChangeBackGroundTask(const SDL_Rect & atlas, const BUTTONSUNDERGROUND underground);

	bool Execute();
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
	ET_DELETEWALL,
	ET_MOREBUTTONS,
	ET_BACKBUTTONS
};

class EntityTask : public Task
{
private:
	ENTITY_TASKTYPE type;

public:
	EntityTask(ENTITY_TASKTYPE type);

	const ENTITY_TASKTYPE GetEntityType() const;
};


class PlaceBasicTowerTask : public EntityTask
{
public:
	PlaceBasicTowerTask(ENTITY_TASKTYPE type = ET_BASICTOWER);

	bool Execute();
};

class PlaceBombardTowerTask : public EntityTask
{
public:
	PlaceBombardTowerTask(ENTITY_TASKTYPE type = ET_BOMBARDTOWER);

	bool Execute();
};

class UpgradeTowerTask : public EntityTask
{
private:
	Tower* tower = nullptr;
	TURRET_UPGRADE type;

public:
	UpgradeTowerTask(TURRET_UPGRADE type, ENTITY_TASKTYPE et_type = ET_UPGRADE_TOWER);

	const TURRET_UPGRADE GetUpgradeType()const;

	void SetTower(Tower* tower);

	bool Execute();
};

class UpgradeWallTask : public EntityTask
{
private:
	Building* wall = nullptr;
	BUILDING_TYPE type;
public:
	UpgradeWallTask(BUILDING_TYPE type, ENTITY_TASKTYPE et_type = ET_UPGRADE_WALL);

	const BUILDING_TYPE GetWallType() const;

	void SetWall(Building* wall);

	bool Execute();
};

class DoInvestigation : public EntityTask
{
private:
	INVESTIGATION_TYPE type;

public:
	DoInvestigation(INVESTIGATION_TYPE type, ENTITY_TASKTYPE et_type = ET_INVESTIGATION);

	const INVESTIGATION_TYPE GetInvestigationType() const;

	bool Execute();
};

class DeleteTowerTask : public EntityTask
{
private:
	Tower* tower = nullptr;
	TURRET_UPGRADE type;

public:
	DeleteTowerTask(ENTITY_TASKTYPE type = ET_DELETETOWER);

	void SetTower(Tower* tower);

	bool Execute();
};

class DeleteWallTask : public EntityTask
{
private:
	Building* wall = nullptr;

public:
	DeleteWallTask(ENTITY_TASKTYPE type = ET_DELETEWALL);

	void SetWall(Building* wall);

	bool Execute();
};

class PlaceWallTask : public EntityTask
{
public:
	PlaceWallTask(ENTITY_TASKTYPE type = ET_WALL);

	bool Execute();
};

class TrainUnitTask : public EntityTask
{
private:
	UNIT_TYPE u_type;
public:
	TrainUnitTask(UNIT_TYPE u_type, ENTITY_TASKTYPE type = ET_UNIT);

	bool Execute();

	const UNIT_TYPE GetUnitType()const;
};

class ChangeToFacebook : public Task
{
public:
	bool Execute();
};

class ChangeToTwitter : public Task
{
public:
	bool Execute();
};

class ChangeToGithub : public Task
{
public:
	bool Execute();
};

class ChangeToBugs : public Task
{
public:
	bool Execute();
};

class BringNextWave : public Task
{
public:
	bool Execute();
};

class Surrender : public Task
{
public:
	bool Execute();
};

class ReturnToGame : public Task
{
private:
	UIHUDMenuInGame* menu_ingame;

public:
	ReturnToGame(UIHUDMenuInGame* menu_ingame);

	bool Execute();
};

class LoadLastChackpoint : public Task
{
public:
	bool Execute();
};

class InGameToMainMenuScene : public Task
{
public:
	bool Execute();
};

#endif