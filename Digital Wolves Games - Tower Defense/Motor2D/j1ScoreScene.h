#ifndef __j1SCORESCENE_H__
#define __j1SCORESCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class UIComponents;
class UIButton;
class UILabel;
class UICheckbutton;
class UIHUDScoreBar;

enum BUTTONSUNDERGROUND
{
	BU_TROPHIES,
	BU_ACHIEVEMENTS,
	BU_INVESTIGATIONS
};

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

	void CreateAchievements();
	void CreateTrophies();
	void CreateAllButtons();
	void SetSceneChange(bool);
	bool IsSceneChange();
	UIComponents* GetUnderBackground();
	void ChangeUnselected(BUTTONSUNDERGROUND);

private:
	UIComponents* under_background;

	UILabel* title_score;
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

	char text_score[256];

	bool scene_changing = false;
	bool trophies_unselected = true;
	bool components_trophies_deleted = false;
	bool achievements_unselected = true;
	bool components_achievements_deleted = true;
	bool investigation_unselected = true;
	bool components_investigation_deleted = true;
};

#endif // __j1SCORESCENE_H__