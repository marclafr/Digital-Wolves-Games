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
#include "j1Score.h"
#include "Task.h"

#include "UIButton.h"
#include "UILabel.h"
#include "UIHUDScoreBar.h"
#include "UICheckbutton.h"
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
	trophies_unselected = true;
	components_trophies_deleted = false;
	achievements_unselected = true;
	components_achievements_deleted = true;
	investigation_unselected = true;
	components_investigation_deleted = true;

	//BACKGROUND
	App->uimanager->AddComponent(UIT_UIIMAGE, { 0, 0, 1336, 622 }, { 0, 1504, 1366, 622 });

		//Under Backgound
	under_background = App->uimanager->AddComponent(UIT_UIIMAGE, { 0, 622, 1336, 144 }, { 0, 2131, 1366, 144 });

	CreateAllButtons();

	CreateTrophies();
	
	CreateAchievements();

	if (App->scene->win == true)
	{
		//Title win
		App->uimanager->AddLabel(655, 21, "You Win!!");
		App->audio->PlayMusic("audio/music/Main_Theme01.ogg", 0.0f);
	}

	if (App->scene->lose == true)
	{
		//Title Lose
		App->uimanager->AddLabel(655, 21, "You Lose!!");
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
		App->score->IncreaseScore();

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
		App->score->DecreaseScore();

	OptionSelected();

	App->render->BlitScoreScene();
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

	App->score->Disable();
	App->uimanager->SetAllToDelete();

	return true;
}

void j1ScoreScene::ActualTrophie()
{
	if (App->score->GetScore() > 560)
		actual_trophie = App->uimanager->AddComponent(UIT_UIIMAGE, { 850, 410, 87, 98 }, { 677, 1370, 87, 98 });
	else if (App->score->GetScore() > 1830)
		actual_trophie = App->uimanager->AddComponent(UIT_UIIMAGE, { 845, 410, 97, 113 }, { 765, 1355, 97, 113 });
	else if (App->score->GetScore() > 3220)
		actual_trophie = App->uimanager->AddComponent(UIT_UIIMAGE, { 837, 410, 113, 129 }, { 941, 1162, 113, 129 });
	else if (App->score->GetScore() > 4680)
		actual_trophie = App->uimanager->AddComponent(UIT_UIIMAGE, { 833, 410, 119, 139 }, { 1058, 1162, 119, 139 });
	else if (App->score->GetScore() > 6250)
		actual_trophie = App->uimanager->AddComponent(UIT_UIIMAGE, { 825, 410, 136, 153 }, { 1181, 1162, 136, 153 });
	else if(true)
		actual_trophie = App->uimanager->AddComponent(UIT_UIIMAGE, { 0, 0, 0, 0 }, { 0, 0, 0, 0 });
}

void j1ScoreScene::CreateAchievements()
{
	//ACHIEVEMENT 1
	achievement1 = App->uimanager->AddComponent(UIT_UIIMAGE, { 355, 73, 718, 130 }, { 0, 2606, 718, 130 });
	title_achievement1 = App->uimanager->AddLabel(410, 130, "Achievement 1", { 0,0,0,0 });
	check_achievement1 = App->uimanager->AddCheckButton({ 1005, 120, 40, 39 }, { 995, 869, 40, 39 }, { 1036, 868, 40, 39 });

	//ACHIEVEMENT 2
	achievement2 = App->uimanager->AddComponent(UIT_UIIMAGE, { 355, 203, 718, 130 }, { 0, 2737, 718, 130 });
	title_achievement2 = App->uimanager->AddLabel(410, 260, "Achievement 2", { 0,0,0,0 });
	check_achievement2 = (UICheckbutton*)App->uimanager->AddCheckButton({ 1005, 250, 40, 39 }, { 995, 869, 40, 39 }, { 1036, 868, 40, 39 });

	components_achievements_deleted = false;
}

void j1ScoreScene::CreateTrophies()
{
	//TROPHIES
		//Wood
	trophie_wood = App->uimanager->AddComponent(UIT_UIIMAGE, { 380, 200, 87, 98 }, { 677, 1370, 87, 98 });

		//Bronze
	trophie_bronze = App->uimanager->AddComponent(UIT_UIIMAGE, { 500, 185, 97, 113 }, { 765, 1355, 97, 113 });

		//Silver
	trophie_silver = App->uimanager->AddComponent(UIT_UIIMAGE, { 630, 169, 113, 129 }, { 941, 1162, 113, 129 });

		//Gold
	trophie_gold = App->uimanager->AddComponent(UIT_UIIMAGE, { 772, 159, 119, 139 }, { 1058, 1162, 119, 139 });

		//Rubi
	trophie_rubi = App->uimanager->AddComponent(UIT_UIIMAGE, { 920, 145, 136, 153 }, { 1181, 1162, 136, 153 });

	score_bar = App->uimanager->AddScoreBar();

	//Title Score
	sprintf_s(text_score, 256, "Score: %d", App->score->GetScore());
	title_score = App->uimanager->AddLabel(490, 400, text_score, BLACK);

	//Title Enemies Killed
	char text_enemies[256];
	sprintf_s(text_enemies, 256, "Enemies Killed: %d", App->score->GetEnemiesKilled());
	title_enemies_killed = App->uimanager->AddLabel(490, 430, text_enemies, BLACK);

	//Title Time
	char text_time[256];
	int minutes = 0;
	int seconds = 0;
	App->score->GetTimer(minutes, seconds);
	sprintf_s(text_time, 256, "Time: %d:%d", minutes, seconds);
	title_time = App->uimanager->AddLabel(490, 460, text_time, BLACK);

	//Title Act Rank
	char text_rank[256];
	sprintf_s(text_rank, 256, "Your Trophie", App->score->GetScore()); //??
	title_act_rank = (UILabel*)App->uimanager->AddLabel(850, 380, text_rank, BLACK);

	ActualTrophie();

	components_trophies_deleted = false;
}

void j1ScoreScene::CreateAllButtons()
{
	UIButton* btn;
	UILabel* label;
	//BACK_MENU
	btn = App->uimanager->AddButton({ 1014, 638, 129, 36 }, { 1234, 1432, 129, 36 });
	btn->SetMouseOnTopTextRect({ 1226, 1386, 137, 42 }, 2);
	btn->SetTask(new ChangeScoreSceneToTask(SC_MAIN_MENU));

	//PLAY AGAIN
	btn = App->uimanager->AddButton({ 846, 637, 129, 37 }, { 1102, 1431, 129, 37 });
	btn->SetMouseOnTopTextRect({ 1085, 1385, 137, 43 }, 2);
	btn->SetTask(new ChangeScoreSceneToTask(SC_GAME));

	//SCORE
	btn = App->uimanager->AddButton({ 278, 696, 126, 55 }, { 0, 0, 0, 0 });
	btn->SetTask(new ChangeBackGroundTask({ 0, 2131, 1366, 144 }, BU_TROPHIES));
	trophies_unselected = false;

	btn->SetLabel(label = App->uimanager->AddLabel(316, 710, "Trophies", { 255,255,255,0 }));
	label->SetColorMouseOnTop({ 255,255,0,0 });

	//ACHIEVEMENTS
	btn = App->uimanager->AddButton({ 416, 696, 123, 55 }, { 0, 0, 0, 0 });
	btn->SetTask(new ChangeBackGroundTask({ 0, 2278, 1366, 144 }, BU_ACHIEVEMENTS));

	btn->SetLabel(label = App->uimanager->AddLabel(440, 710, "Achievements", { 255,255,255,0 }));
	label->SetColorMouseOnTop({ 255,255,0,0 });

	//INVESTIGATIONS
	btn = App->uimanager->AddButton({ 555, 696, 123, 55 }, { 0, 0, 0, 0 });
	btn->SetTask(new ChangeBackGroundTask({ 0, 2426, 1366, 144 }, BU_INVESTIGATIONS));

	btn->SetLabel(label = App->uimanager->AddLabel(575, 710, "Investigations", { 255,255,255,0 }));
	label->SetColorMouseOnTop({ 255,255,0,0 });
}

void j1ScoreScene::SetSceneChange(bool is_change)
{
	scene_changing = is_change;
}

bool j1ScoreScene::IsSceneChange()
{
	return scene_changing;
}

UIComponents * j1ScoreScene::GetUnderBackground()
{
	return under_background;
}

void j1ScoreScene::ChangeUnselected(BUTTONSUNDERGROUND unselected)
{
	switch (unselected)
	{
	case BU_TROPHIES:
		trophies_unselected = false;
		achievements_unselected = true;
		investigation_unselected = true;
		break;
	case BU_ACHIEVEMENTS:
		trophies_unselected = true;
		achievements_unselected = false;
		investigation_unselected = true;
		break;
	case BU_INVESTIGATIONS:
		trophies_unselected = true;
		achievements_unselected = true;
		investigation_unselected = false;
		break;
	}
}

void j1ScoreScene::OptionSelected()
{
	if (trophies_unselected)
	{
		if (!components_trophies_deleted)
		{
			trophie_wood->SetToDelete();
			trophie_bronze->SetToDelete();
			trophie_silver->SetToDelete();
			trophie_gold->SetToDelete();
			trophie_rubi->SetToDelete();;
			score_bar->SetToDelete();
			title_score->SetToDelete();
			title_enemies_killed->SetToDelete();
			title_time->SetToDelete();
			title_act_rank->SetToDelete();
			actual_trophie->SetToDelete();
			components_trophies_deleted = true;
		}
	}
	else
	{
		if(components_trophies_deleted)
			CreateTrophies();
	}

	if (achievements_unselected)
	{
		if (!components_achievements_deleted)
		{
			achievement1->SetToDelete();
			title_achievement1->SetToDelete();
			check_achievement1->SetToDelete();
			achievement2->SetToDelete();
			title_achievement2->SetToDelete();
			check_achievement2->SetToDelete();
			components_achievements_deleted = true;
		}
	}
	else
	{
		if(components_achievements_deleted)
			CreateAchievements();
	}

	if (investigation_unselected)
	{

	}
}