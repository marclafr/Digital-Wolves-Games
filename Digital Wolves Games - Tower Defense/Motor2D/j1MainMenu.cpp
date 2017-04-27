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
	App->audio->PlayMusic("audio/music/Menu01.ogg", 0.0f);

	//BACKGROUND
	background = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIIMAGE);
	background->Set({ 0, 0, 1336, 767 }, { 0, 0, 1366, 767 });
	background->SetInteractive(false);

	//SINGLE PLAYER
	single_player = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIBUTTON);
	single_player->Set({ 539, 17, 177, 240 }, { 414, 769, 177, 240 });
	single_player->SetInteractive(true);
	single_player->SetMouseOnTopTextRect({ 298, 1229, 177, 240 });

	title_single_player = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UILABEL);
	title_single_player->Set(590, 30, "Single Player");

	//TUTORIAL
	tutorial = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIBUTTON);
	tutorial->Set({ 148, 11, 155, 232 }, { 592, 770, 155, 232 });
	tutorial->SetInteractive(false);

	//HISTORY
	history = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIBUTTON);
	history->Set({ 301, 210, 138, 123 }, { 149, 862, 138, 123 });
	history->SetInteractive(false);

	//TROPHIES
	trophies = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIBUTTON);
	trophies->Set({ 500, 282, 148, 159 }, { 0, 770, 148, 159 });
	trophies->SetInteractive(true);
	trophies->SetMouseOnTopTextRect({ 792, 1162, 148, 160 });

	title_trophies = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UILABEL);
	title_trophies->Set(545, 286, "Trophies");

	//OPTIONS
	options = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIBUTTON);
	options->Set({ 300, 448, 125, 122 }, { 288, 862, 125, 123 });
	options->SetInteractive(false);
	
	//EXIT
	exit = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UIBUTTON);
	exit->Set({ 174, 677, 200, 91 }, { 149, 770, 200, 91 });
	exit->SetInteractive(true);
	exit->SetMouseOnTopTextRect({ 477, 1374, 200, 95 });


	title_exit = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIT_UILABEL);
	title_exit->Set(265, 708, "Exit");

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
	//TODO change to tasks
	if (trophies->GetStat() == BS_MOUSE_ON_TOP)
		App->scene_manager->ChangeScene(SC_SCORE);

	if (single_player->GetStat() == BS_MOUSE_ON_TOP)
	{
		App->scene_manager->ChangeScene(SC_GAME);
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
	//todo Change to tasks
	if (exit->GetStat() == BS_MOUSE_ON_TOP)
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

