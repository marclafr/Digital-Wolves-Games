#ifndef __j1SCORESCENE_H__
#define __j1SCORESCENE_H__

#include "j1Module.h"

struct SDL_Texture;

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

	void OptionSelected();

	void ActualTrophie();

	bool game_scene = true;

	void VisualEffectsUI();
	void CreateAchievements();
	void CreateTrophies();
	void CreateAllButtons();

private:
	/*
	UIComponents* background;
	UIComponents* under_background;

	UIButton* play_again;
	UIButton* back_menu;

	UIButton* trophies;
	UILabel* title_trophies;

	UIButton* achievements;
	UILabel* title_achievements;

	UIButton* investigations;
	UILabel* title_investigations;

	UILabel* title_win;
	UILabel* title_lose;
	UILabel* title_score;
	char text_score[256];
	UILabel* title_enemies_killed;
	UILabel* title_time;

	UILabel* title_act_rank;
	UIComponents* actual_trophie;

	//TROPHIES
	UIComponents* trophie_wood;
	UIComponents* trophie_bronze;
	UIComponents* trophie_silver;
	UIComponents* trophie_gold;
	UIComponents* trophie_rubi;

	UIHUDScoreBar* score_bar;

	//ACHIEVEMENTS
	UIComponents* achievement1;
	UILabel* title_achievement1;
	UICheckbutton* check_achievement1;

	UIComponents* achievement2;
	UILabel* title_achievement2;
	UICheckbutton* check_achievement2;
	*/
	bool scene_changing = false;
	bool trophies_unselected = true;
	bool achievements_unselected = true;
	bool investigation_unselected = true;
};

#endif // __j1SCORESCENE_H__