#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class UILabel;
class UIComponents;
class UIButton;
class UICheckbutton;
class UIHUDPanelButtons;
class UIHUDPanelInfo;
class UIHUDDescription;
class UIHUDResources;

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
	void EnableScene();

private:
	SDL_Texture* debug_tex;
	SDL_Texture* tower_tex;
	bool placing_tower = false;
	UIComponents* top_hud;
	UIButton* objectives;
	UIButton* tree_tech;
	UIButton* ingame_menu;
	UIHUDResources* resources_panel;
	UILabel* title_game_name;

	UIComponents* down_hud;
	UICheckbutton* btn_description;
	UIHUDPanelButtons* panel;

	SDL_Rect select_rect;

public:
	UIHUDPanelInfo* panel_info;
	UIHUDDescription* hud_description;
};

#endif // __j1SCENE_H__