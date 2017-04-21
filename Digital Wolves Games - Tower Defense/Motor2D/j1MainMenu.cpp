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
#include "j1ScoreScene.h"
#include "j1EntityManager.h"
#include "j1MainMenu.h"
#include "j1SceneManager.h"

#include "UIButton.h"
#include "UILabel.h"
#include "j1UIManager.h"

j1MainMenu::j1MainMenu() : j1Module()
{
	name.assign("mainmenu");
}

// Destructor
j1MainMenu::~j1MainMenu()
{}

// Called before render is available
bool j1MainMenu::Awake()
{
	LOG("Loading MainMenu");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1MainMenu::Start()
{
	App->audio->PlayMusic("audio/music/Menu01.ogg");

	//BACKGROUND
	background = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	background->Set({ 0, 0, 1336, 767 }, { 0, 0, 1366, 767 });

	//SINGLE PLAYER
	single_player = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	single_player->Set({ 539, 17, 177, 240 }, { 414, 769, 177, 240 });
	single_player->SetInteractive(true);

	title_single_player = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	title_single_player->Set(590, 30, "Single Player");

	//TUTORIAL
	tutorial = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	tutorial->Set({ 148, 11, 155, 232 }, { 592, 770, 155, 232 });
	tutorial->SetInteractive(false);

	//HISTORY
	history = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	history->Set({ 301, 210, 138, 123 }, { 149, 862, 138, 123 });
	history->SetInteractive(false);

	//TROPHIES
	trophies = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	trophies->Set({ 500, 282, 148, 159 }, { 0, 770, 148, 159 });
	trophies->SetInteractive(true);

	title_trophies = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	title_trophies->Set(545, 286, "Trophies");

	

	//OPTIONS
	options = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	options->Set({ 300, 448, 125, 122 }, { 288, 862, 125, 123 });
	options->SetInteractive(false);
	
	//EXIT
	exit = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	exit->Set({ 174, 677, 200, 91 }, { 149, 770, 200, 91 });
	exit->SetInteractive(true);

	title_exit = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	title_exit->Set(265, 708, "Exit");

	if (App->scene->win == true)
	{
		title_win = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
		char text_score[256];
		sprintf_s(text_score, 256, " You Win!!  Score: %d", App->entity_manager->GetScore());
		title_win->Set(900, 690, text_score);
		App->audio->PlayMusic("audio/music/Main_Theme01.ogg");
	}

	if (App->scene->lose == true)
	{
		title_lose = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
		char text_score[256];
		sprintf_s(text_score, 256, " You Lose :(  Score: %d", App->entity_manager->GetScore());
		title_lose->Set(900, 690, text_score);	
		App->audio->PlayMusic("audio/music/Lost_Game01.ogg");
	}

	return true;
}

// Called each loop iteration
bool j1MainMenu::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool j1MainMenu::Update(float dt)
{
	
	if (single_player->GetStat() == SELECTED) 
	{
		single_player->Set({ 539, 17, 177, 240 }, { 298, 1229, 177, 240 });
	}
	if (single_player->GetStat() == UNSELECTED)
	{
		single_player->Set({ 539, 17, 177, 240 }, { 414, 769, 177, 240 });
	}

	

	if (exit->GetStat() == SELECTED) 
	{
	exit->Set({ 174, 673, 200, 95 }, { 477, 1374, 200, 95 });
	}
	if (exit->GetStat() == UNSELECTED) 
	{
	exit->Set({ 174, 677, 200, 91 }, { 149, 770, 200, 91 });
	}
	
	

	if (trophies->GetStat() == SELECTED)
	{
		trophies->Set({ 500, 282, 148, 160 }, { 792, 1162, 148, 160 });
	}
	if (trophies->GetStat() == UNSELECTED)
	{
		trophies->Set({ 500, 282, 148, 159 }, { 0, 770, 148, 159 });
	}

	if (trophies->GetStat() == CLICKL_UP)
	{
		App->scene_manager->ChangeScene(App->score_scene, this);
	}
	

	if (single_player->GetStat() == CLICKL_UP) 
			{
				App->scene_manager->ChangeScene(App->scene, this);
				App->scene->win = false;
				App->scene->lose = false;
			}
	return true;
}

// Called each loop iteration
bool j1MainMenu::PostUpdate()
{
	bool ret = true;

	//Unit test

	if (exit->GetStat() == CLICKL_UP)
		ret = false;

	return ret;
}

// Called before quitting
bool j1MainMenu::CleanUp()
{
	LOG("Freeing  MainMenu");
	App->uimanager->CleanUp();
	return true;
}

