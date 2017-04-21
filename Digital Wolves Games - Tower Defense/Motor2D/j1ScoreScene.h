#ifndef __j1SCORESCENE_H__
#define __j1SCORESCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class UILabel;
class UIComponents;
class UIButton;



class j1ScoreScene : public j1Module
{
public:

	j1ScoreScene();

	// Destructor
	virtual ~j1ScoreScene();

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
	UIComponents* under_background;

	UIButton* play_again;
	UIButton* back_menu;

	UIButton* score;
	UILabel* title_score;

	UIButton* achievements;
	UILabel* title_achievements;

	UIButton* investigations;
	UILabel* title_investigations;

	UILabel* title_win;
	UILabel* title_lose;

	bool scene_changing = false;

public:

};

#endif // __j1SCORESCENE_H__