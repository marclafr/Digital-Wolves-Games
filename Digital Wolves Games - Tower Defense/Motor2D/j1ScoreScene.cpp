#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "Camera.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include "j1MainMenu.h"
#include "j1SceneManager.h"
#include "j1ScoreScene.h"

#include "UIButton.h"
#include "UILabel.h"
#include "UIHUDScoreBar.h"
#include "UIGetEntitiesInfo.h"
#include "j1UIManager.h"

j1ScoreScene::j1ScoreScene() : j1Module()
{
	name.assign("scorescene");
}

// Destructor
j1ScoreScene::~j1ScoreScene()
{}

// Called before render is available
bool j1ScoreScene::Awake()
{
	LOG("Loading ScoreScene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1ScoreScene::Start()
{
	App->audio->PlayMusic("audio/music/Menu01.ogg", 0.0f);
	scene_changing = false;

	//BACKGROUND
	background = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	background->Set({ 0, 0, 1336, 622 }, { 0, 1504, 1366, 622 });

	under_background = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	under_background->Set({ 0, 622, 1336, 144 }, { 0, 2131, 1366, 144 });

	//BACK_MENU
	back_menu = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	back_menu->Set({ 1014, 638, 129, 36 }, { 1234, 1432, 129, 36 });
	back_menu->SetInteractive(true);

	//PLAY AGAIN
	play_again = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	play_again->Set({ 846, 637, 129, 37 }, { 1102, 1431, 129, 37 });
	play_again->SetInteractive(true);

	//SCORE
	trophies = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	trophies->Set({ 278, 696, 126, 55 }, { 0, 0, 0, 0 });
	trophies->SetInteractive(true);
	trophies_unselected = false;

	title_trophies = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	title_trophies->Set(316, 710, "Trophies");

	//ACHIEVEMENTS
	achievements = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	achievements->Set({ 416, 696, 123, 55 }, { 0, 0, 0, 0 });
	achievements->SetInteractive(true);

	title_achievements = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	title_achievements->Set(440, 710, "Achievements");

	//INVESTIGATIONS
	investigations = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	investigations->Set({ 555, 696, 123, 55 }, { 0, 0, 0, 0 });
	investigations->SetInteractive(true);

	title_investigations = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	title_investigations->Set(575, 710, "Investigations");

	//TROPHIES
	trophie_wood = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	trophie_wood->Set({ 380, 200, 87, 98 }, { 677, 1370, 87, 98 });

	trophie_bronze = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	trophie_bronze->Set({ 500, 185, 97, 113 }, { 765, 1355, 97, 113 });

	trophie_silver = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	trophie_silver->Set({ 630, 169, 113, 129 }, { 941, 1162, 113, 129 });

	trophie_gold = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	trophie_gold->Set({ 772, 159, 119, 139 }, { 1058, 1162, 119, 139 });

	trophie_rubi = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	trophie_rubi->Set({ 920, 145, 136, 153 }, { 1181, 1162, 136, 153 });

	score_bar = (UIHUDScoreBar*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIHUDSCOREBAR);

	//TEXT
	title_score = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	sprintf_s(text_score, 256, "Score: %d", App->entity_manager->GetScore());
	title_score->Set(490, 380, text_score);

	title_enemies_killed = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	char text_enemies[256];
	sprintf_s(text_enemies, 256, "Enemies Killed: %d", App->entity_manager->GetEnemiesKilled());
	title_enemies_killed->Set(490, 410, text_enemies);

	title_time = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	char text_time[256];
	sprintf_s(text_time, 256, "Time: %d:%d", App->entity_manager->GetMins(), App->entity_manager->GetSecs());
	title_time->Set(490, 440, text_time);

	title_act_rank = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	char text_rank[256];
	sprintf_s(text_rank, 256, "Your Trophie", App->entity_manager->GetScore());
	title_act_rank->Set(850, 380, text_rank);
	
	actual_trophie = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);

	if (App->scene->win == true)
	{
		title_win = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
		title_win->Set(655, 21, "You Win!!");
		App->audio->PlayMusic("audio/music/Main_Theme01.ogg", 0.0f);
	}

	if (App->scene->lose == true)
	{
		title_lose = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
		title_lose->Set(655, 21, "You Lose!!");
		App->audio->PlayMusic("audio/music/Lost_Game01.ogg", 0.0f);
	}
	
	return true;
}

// Called each loop iteration
bool j1ScoreScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1ScoreScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
		App->entity_manager->IncreaseScore();

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
		App->entity_manager->DecreaseScore();


	sprintf_s(text_score, 256, "Score: %d", App->entity_manager->GetScore());
	title_score->Set(490, 380, text_score);
	ActualTrophie();

	//PLAY AGAIN
	if (play_again->GetStat() == SELECTED)
	{
		play_again->Set({ 842, 634, 137, 43 }, { 1085, 1385, 137, 43 });
	}
	if (play_again->GetStat() == UNSELECTED)
	{
		play_again->Set({ 846, 637, 129, 37 }, { 1102, 1431, 129, 37 });
	}
	if (play_again->GetStat() == CLICKL_UP)
	{
		App->scene_manager->ChangeScene(SC_GAME);
		scene_changing = true;
		App->scene->win = false;
		App->scene->lose = false;

	}

	//BACK MENU
	if (!scene_changing) 
	{
		if (back_menu->GetStat() == SELECTED)
		{
			back_menu->Set({ 1010, 635, 137, 42 }, { 1226, 1386, 137, 42 });
		}
		if (back_menu->GetStat() == UNSELECTED)
		{
			back_menu->Set({ 1014, 638, 129, 36 }, { 1234, 1432, 129, 36 });
		}
		if (back_menu->GetStat() == CLICKL_UP)
		{
			App->scene_manager->ChangeScene(SC_MAIN_MENU);
			App->scene->win = false;
			App->scene->lose = false;
		}
	}

	//SCORE
	if (trophies->GetStat() == CLICKL_UP)
	{
		under_background->Set({ 0, 622, 1336, 144 }, { 0, 2131, 1366, 144 });
		trophies_unselected = false;
	}

	//ACHIEVEMENTS
	if (achievements->GetStat() == CLICKL_UP)
	{
		under_background->Set({ 0, 622, 1336, 144 }, { 0, 2278, 1366, 144 });
		trophies_unselected = true;
	}

	//INVESTIGATIONS
	if (investigations->GetStat() == CLICKL_UP)
	{
		under_background->Set({ 0, 622, 1336, 144 }, { 0, 2426, 1366, 144 });
		trophies_unselected = true;
	}

	OptionSelected();

	return true;
}

// Called each loop iteration
bool j1ScoreScene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		App->scene_manager->ChangeScene(SC_MAIN_MENU);

	return ret;
}

// Called before quitting
bool j1ScoreScene::CleanUp()
{
	LOG("Freeing  MainMenu");
	App->uimanager->CleanUp();
	return true;
}

void j1ScoreScene::ActualTrophie()
{
	if (App->entity_manager->GetScore() > 560) {
		actual_trophie->Set({ 850, 410, 87, 98 }, { 677, 1370, 87, 98 });
	}
	if (App->entity_manager->GetScore() > 1000) {
		actual_trophie->Set({ 845, 410, 97, 113 }, { 765, 1355, 97, 113 });
	}
	if (App->entity_manager->GetScore() > 1500) {
		actual_trophie->Set({ 837, 410, 113, 129 }, { 941, 1162, 113, 129 });
	}
	if (App->entity_manager->GetScore() > 2000) {
		actual_trophie->Set({ 833, 410, 119, 139 }, { 1058, 1162, 119, 139 });
	}
	if (App->entity_manager->GetScore() > 2500) {
		actual_trophie->Set({ 825, 410, 136, 153 }, { 1181, 1162, 136, 153 });
	}
}

void j1ScoreScene::OptionSelected()
{
	if (trophies_unselected)
	{
		trophie_wood->SetDraw(false);
		trophie_bronze->SetDraw(false);
		trophie_silver->SetDraw(false);
		trophie_gold->SetDraw(false);
		trophie_rubi->SetDraw(false);
		score_bar->SetDraw(false);
		title_score->SetDraw(false);
		title_enemies_killed->SetDraw(false);
		title_time->SetDraw(false);

		title_act_rank->SetDraw(false);
		actual_trophie->SetDraw(false);
	}
	else
	{
		trophie_wood->SetDraw(true);
		trophie_bronze->SetDraw(true);
		trophie_silver->SetDraw(true);
		trophie_gold->SetDraw(true);
		trophie_rubi->SetDraw(true);
		score_bar->SetDraw(true);
		title_score->SetDraw(true);
		title_enemies_killed->SetDraw(true);
		title_time->SetDraw(true);

		title_act_rank->SetDraw(true);
		actual_trophie->SetDraw(true);
	}

	if (achievements_unselected)
	{

	}

	if (investigation_unselected)
	{

	}
}

