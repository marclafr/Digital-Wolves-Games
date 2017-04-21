#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

#define BASIC_TOWER_WOOD_COST 75
#define BASIC_TOWER_STONE_COST 25
#define TWOHANDED_WOOD_COST 15
#define TWOHANDED_STONE_COST 15
#define BASIC_WALL_STONE_COST 50
#define WINNING_TIME 600

struct SDL_Texture;
class UILabel;
class UIComponents;
class UIButton;
class UICheckbutton;
class UIHUDPanelButtons;
class UIHUDPanelInfo;
class UIHUDDescription;
class UIHUDResources;
class UIHUDTownHallBarLife;
class Resources;
class Building;
enum RESOURCE_TYPE;

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

	bool placing_tower = false;
	bool placing_wall = false;
	bool game_scene = true;

	bool win = false;
	bool lose = false;
private:
	SDL_Texture* debug_tex;
	SDL_Texture* tower_tex;
	SDL_Texture* wall_tex;
	UIComponents* top_hud;
	UIButton* objectives;
	UIButton* tree_tech;
	UIButton* ingame_menu;
	UIHUDResources* resources_panel;
	UILabel* title_game_name;

	UIComponents* down_hud;
	UICheckbutton* btn_description;
	UIHUDPanelButtons* panel;
	UIHUDTownHallBarLife* townhall_bar_life;

	SDL_Rect select_rect;

	Resources* resource_food;
	Resources* resource_wood;
	Resources* resource_gold;
	Resources* resource_stone;

	Building* townhall;
	Building* townhalltower1;
	Building* townhalltower2;

	UIButton* new_wave_button;
	bool clicked = false;

public:
	Resources* GetResource(RESOURCE_TYPE type);


public:
	UIHUDPanelInfo* panel_info;
	UIHUDDescription* hud_description;
	j1Timer game_time;

	UIComponents* info_ui;
};

#endif // __j1SCENE_H__