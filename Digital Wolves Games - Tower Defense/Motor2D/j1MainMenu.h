#ifndef __j1MAINMENU_H__
#define __j1MAINMENU_H__

#include "j1Module.h"

struct SDL_Texture;
class UILabel;
class UIComponents;
class UIButton;
class UICheckbutton;
class UIHUDPanelButtons;
class UIHUDPanelInfo;
class UIHUDDescription;


class j1MainMenu : public j1Module
{
public:

	j1MainMenu();

	// Destructor
	virtual ~j1MainMenu();

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

	bool game_scene = true;

private:
	
	UIComponents* background;

	UIButton* single_player;
	UILabel* title_single_player;

	UIButton* tutorial;
	UIButton* history;
	UIButton* trophies;
	UIButton* options;
	
	UIButton* exit;
	UILabel* title_exit;

	
	UILabel* title_win;
	UILabel* title_lose;


public:

};

#endif // __j1MAINMENU_H__