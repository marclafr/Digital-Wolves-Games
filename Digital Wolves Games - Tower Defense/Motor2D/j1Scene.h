#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include <vector>
#include "j1Module.h"
#include "Entity.h"

#define BASIC_TOWER_WOOD_COST 75
#define BASIC_TOWER_STONE_COST 25
#define TWOHANDED_WOOD_COST 15
#define TWOHANDED_STONE_COST 15
#define BASIC_WALL_STONE_COST 50

struct SDL_Texture;
class Resources;
class Building;
enum RESOURCE_TYPE;
class AnimationManager;

enum PlacingWallDirection {
	NORTH,
	SOUTH,
	WEST,
	EAST,
	DIAGONAL,
};
class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Enable Module with submodules
	void CreateSceneUI();
	void EnableScene();
	bool CanBuildTower();
	void BuildTower();
	bool CanBuildWall();
	void BuildWall();
	bool CanTrainSoldier();
	void TrainSoldier();

	void PlacingBasicTower();
	void CheckClick();
	void PlacingBombardTower();
	
	void PlacingWall();

	void HandleInput(SDL_Event event);

	bool placing_basic_tower = false;
	bool placing_bombard_tower = false;
	bool placing_wall = false;
	bool game_scene = true;

	bool win = false;
	bool lose = false;

private:
	j1Audio* scene_music;
	bool selecting = false;

	SDL_Texture* debug_tex;

	SDL_Rect select_rect;

	Building* townhall;
	Building* townhalltower1;
	Building* townhalltower2;

	std::vector<Entity*> ClickingVector; 
	iPoint firstSelectedForWall;
	iPoint LastSelectedForWall;
	PlacingWallDirection direction;
	bool placing_wall_clicked = false;

public:
	Resources* GetResource(RESOURCE_TYPE type);

	std::vector<Entity*> selection;

public:

	//RESOURCES
	Resources* resource_food;
	Resources* resource_wood;
	Resources* resource_gold;
	Resources* resource_stone;
	//--

	//ANIMATION TEST
	AnimationManager* a1;
	AnimationManager* a2;
	AnimationManager* a3;
	AnimationManager* a4;
	AnimationManager* a5;
	AnimationManager* a6;
	AnimationManager* a7;
	AnimationManager* a8;
	AnimationManager* a_fire;
	//--
};

#endif // __j1SCENE_H__