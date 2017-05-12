#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include <vector>
#include "j1Module.h"
#include "Entity.h"
#include "ResourceManager.h"
#include "Towers.h"

struct SDL_Texture;
class Resources;
class Building;
enum TOWER_TYPE;
enum RESOURCE_TYPE;
enum BUILDING_TYPE;
class AnimationManager;

enum PlacingWallDirection {
	NORTH,
	SOUTH,
	WEST,
	EAST,
	NORTH_WEST,
	NORTH_EAST,
	SOUTH_WEST,
	SOUTH_EAST
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

	void PlacingTower(TOWER_TYPE type);
	
	void PlacingWall();

	void HandleInput(SDL_Event event);

	void TutorialUI();

private:


	bool game_scene = true;

	j1Audio* scene_music;
	bool selecting = false;

	SDL_Rect select_rect;

	Building* townhall;
 
	iPoint firstSelectedForWall;
	iPoint LastSelectedForWall;
	PlacingWallDirection direction;
	bool placing_wall_clicked = false;

public:
	bool win = false;
	bool lose = false;

	TOWER_TYPE placing_tower = T_NO_TYPE;
	bool placing_wall = false;

	ResourceManager* resources = nullptr;

	std::vector<Entity*> selection;

	//TEST FOR SAMPER: TODO delete this and all related
	AnimationManager* anim_test2;
	AnimationManager* anim_test3;
	AnimationManager* anim_test4;
	AnimationManager* anim_test5;
	//--
};

#endif // __j1SCENE_H__