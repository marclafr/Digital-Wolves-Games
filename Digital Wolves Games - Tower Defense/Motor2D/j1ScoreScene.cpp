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
	background = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIIMAGE);
	background->Set({ 0, 0, 1336, 622 }, { 0, 1504, 1366, 622 });

	under_background = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIIMAGE);
	under_background->Set({ 0, 622, 1336, 144 }, { 0, 2131, 1366, 144 });

	//BACK_MENU
	back_menu = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIBUTTON);
	back_menu->Set({ 1014, 638, 129, 36 }, { 1234, 1432, 129, 36 });
	back_menu->SetInteractive(true);

	//PLAY AGAIN
	play_again = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIBUTTON);
	play_again->Set({ 846, 637, 129, 37 }, { 1102, 1431, 129, 37 });
	play_again->SetInteractive(true);

	//SCORE
	trophies = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIBUTTON);
	trophies->Set({ 278, 696, 126, 55 }, { 0, 0, 0, 0 });
	trophies->SetInteractive(true);
	trophies_unselected = false;

	title_trophies = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UILABEL);
	title_trophies->Set(316, 710, "Trophies");

	//ACHIEVEMENTS
	achievements = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIBUTTON);
	achievements->Set({ 416, 696, 123, 55 }, { 0, 0, 0, 0 });
	achievements->SetInteractive(true);

	title_achievements = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UILABEL);
	title_achievements->Set(440, 710, "Achievements");

	//INVESTIGATIONS
	investigations = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIBUTTON);
	investigations->Set({ 555, 696, 123, 55 }, { 0, 0, 0, 0 });
	investigations->SetInteractive(true);

	title_investigations = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UILABEL);
	title_investigations->Set(575, 710, "Investigations");

	//TROPHIES //TEXT
	CreateTrophies();

	if (App->scene->win == true)
	{
		title_win = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UILABEL);
		title_win->Set(655, 21, "You Win!!");
		App->audio->PlayMusic("audio/music/Main_Theme01.ogg", 0.0f);
	}

	if (App->scene->lose == true)
	{
		title_lose = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UILABEL);
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
	if (play_again->GetStat() == BS_UNCLICKED)
	{
		App->scene_manager->ChangeScene(SC_GAME);
		scene_changing = true;
		App->scene->win = false;
		App->scene->lose = false;

	}

	//BACK MENU
	if (!scene_changing)
	{
		if (back_menu->GetStat() == BS_UNCLICKED)
		{
			App->scene_manager->ChangeScene(SC_MAIN_MENU);
			App->scene->win = false;
			App->scene->lose = false;
		}
	}

	//SCORE
	if (trophies->GetStat() == BS_UNCLICKED)
	{
		under_background->Set({ 0, 622, 1336, 144 }, { 0, 2131, 1366, 144 });
		trophies_unselected = false;
	}

	//ACHIEVEMENTS
	if (achievements->GetStat() == BS_UNCLICKED)
	{
		under_background->Set({ 0, 622, 1336, 144 }, { 0, 2278, 1366, 144 });
		trophies_unselected = true;
	}

	//INVESTIGATIONS
	if (investigations->GetStat() == BS_UNCLICKED)
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
	if (App->entity_manager->GetScore() > 1830) {
		actual_trophie->Set({ 845, 410, 97, 113 }, { 765, 1355, 97, 113 });
	}
	if (App->entity_manager->GetScore() > 3220) {
		actual_trophie->Set({ 837, 410, 113, 129 }, { 941, 1162, 113, 129 });
	}
	if (App->entity_manager->GetScore() > 4680) {
		actual_trophie->Set({ 833, 410, 119, 139 }, { 1058, 1162, 119, 139 });
	}
	if (App->entity_manager->GetScore() > 6250) {
		actual_trophie->Set({ 825, 410, 136, 153 }, { 1181, 1162, 136, 153 });
	}
}

void j1ScoreScene::CreateTrophies()
{
	trophie_wood = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIIMAGE);
	trophie_wood->Set({ 380, 200, 87, 98 }, { 677, 1370, 87, 98 });

	trophie_bronze = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIIMAGE);
	trophie_bronze->Set({ 500, 185, 97, 113 }, { 765, 1355, 97, 113 });

	trophie_silver = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIIMAGE);
	trophie_silver->Set({ 630, 169, 113, 129 }, { 941, 1162, 113, 129 });

	trophie_gold = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIIMAGE);
	trophie_gold->Set({ 772, 159, 119, 139 }, { 1058, 1162, 119, 139 });

	trophie_rubi = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIIMAGE);
	trophie_rubi->Set({ 920, 145, 136, 153 }, { 1181, 1162, 136, 153 });

	score_bar = (UIHUDScoreBar*)App->uimanager->addUIComponent(UICOMPONENT_TYPE:: UIT_UIHUDSCOREBAR);

	title_score = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UILABEL);
	sprintf_s(text_score, 256, "Score: %d", App->entity_manager->GetScore());
	title_score->Set(490, 400, text_score, { 0,0,0,0 });

	title_enemies_killed = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UILABEL);
	char text_enemies[256];
	sprintf_s(text_enemies, 256, "Enemies Killed: %d", App->entity_manager->GetEnemiesKilled());
	title_enemies_killed->Set(490, 430, text_enemies, { 0,0,0,0 });

	title_time = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UILABEL);
	char text_time[256];
	sprintf_s(text_time, 256, "Time: %d:%d", App->entity_manager->GetMins(), App->entity_manager->GetSecs());
	title_time->Set(490, 460, text_time, { 0,0,0,0 });

	title_act_rank = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UILABEL);
	char text_rank[256];
	sprintf_s(text_rank, 256, "Your Trophie", App->entity_manager->GetScore());
	title_act_rank->Set(850, 380, text_rank, { 0,0,0,0 });

	actual_trophie = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIIMAGE);

	trophies_created = true;
}

void j1ScoreScene::DeleteTrophies()
{
	trophie_wood->SetToDelete(true);
	trophie_bronze->SetToDelete(true);
	trophie_silver->SetToDelete(true);
	trophie_gold->SetToDelete(true);
	trophie_rubi->SetToDelete(true);
	score_bar->SetToDelete(true);
	title_score->SetToDelete(true);
	title_enemies_killed->SetToDelete(true);
	title_time->SetToDelete(true);
	title_act_rank->SetToDelete(true);
	actual_trophie->SetToDelete(true);

	trophies_created = false;
}

void j1ScoreScene::OptionSelected()
{
	if (trophies_unselected)
		if (trophies_created)
			DeleteTrophies();
		else
			if (trophies_created == false)
				CreateTrophies();

	if (achievements_unselected)
	{

	}

	if (investigation_unselected)
	{

	}
}

