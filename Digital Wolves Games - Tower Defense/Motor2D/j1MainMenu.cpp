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
#include "Task.h"

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
	App->uimanager->AddComponent(UIT_UIIMAGE, { 0, 0, 1336, 767 }, { 0, 0, 1366, 767 });

	//SINGLE PLAYER
	UIButton* single_player = (UIButton*)App->uimanager->AddButton({ 539, 17, 177, 240 }, { 414, 769, 177, 240 });
	single_player->SetMouseOnTopTextRect({ 298, 1229, 177, 240 });
	single_player->SetTask(new ChangeMainMenuSceneToTask(SC_GAME));

	App->uimanager->AddLabel(590, 30, "Single Player");

	//TUTORIAL
	UIButton* tutorial = App->uimanager->AddButton({ 148, 11, 155, 232 }, { 592, 770, 155, 232 });
	tutorial->SetInteractive(false);

	//HISTORY
	UIButton* history = App->uimanager->AddButton({ 301, 210, 138, 123 }, { 149, 862, 138, 123 });
	history->SetInteractive(false);

	//TROPHIES
	UIButton* trophies = (UIButton*)App->uimanager->AddButton({ 500, 282, 148, 159 }, { 0, 770, 148, 159 });
	trophies->SetMouseOnTopTextRect({ 792, 1162, 148, 160 });
	trophies->SetTask(new ChangeMainMenuSceneToTask(SC_SCORE));

	App->uimanager->AddLabel(545, 286, "Trophies");

	//OPTIONS
	UIButton* options = App->uimanager->AddButton({ 300, 448, 125, 122 }, { 288, 862, 125, 123 });
	options->SetInteractive(false);

	//EXIT
	UIButton* exit = App->uimanager->AddButton({ 174, 677, 200, 91 }, { 149, 770, 200, 91 });
	exit->SetMouseOnTopTextRect({ 477, 1374, 200, 95 });
	exit->SetTask(new SetPreUpdateFalseTask());

	App->uimanager->AddLabel(265, 708, "Exit");

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
	return true;
}

// Called each loop iteration
bool j1MainMenu::PostUpdate()
{
	return ret_preupdate;
}

// Called before quitting
bool j1MainMenu::CleanUp()
{
	LOG("Freeing  MainMenu");
	App->uimanager->SetAllToDelete();
	return true;
}

void j1MainMenu::SetRetPreUpdate(bool ret)
{
	ret_preupdate = ret;
}