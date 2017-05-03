#define RECT_INGAME_WITHOUT_UI {0, 27, 1360, 624}

#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "Camera.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Scene.h"
#include "j1Animation.h"
#include "j1EntityManager.h"
#include "j1Collision.h"
#include "Units.h"
#include "Resources.h"
#include "j1SceneManager.h"
#include "j1MainMenu.h"
#include "j1ScoreScene.h"
#include "j1WaveManager.h"
#include "UIButton.h"
#include "UILabel.h"
#include "UIHUDPanelButtons.h"
#include "UIHUDDescription.h"
#include "UIHUDResources.h"
#include "UIHUDTownHallBarLife.h"
#include "UICheckbutton.h"
#include "UIGetEntitiesInfo.h"
#include "j1UIManager.h"
#include "j1Investigations.h"
#include "Towers.h"

j1Scene::j1Scene() : j1Module()
{
	name.assign("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->pathfinding->Enable();
	App->wave_manager->Enable();
	App->audio->PlayMusic("audio/music/Music_enviroment03.ogg", 0.0f);

	App->render->camera->SetPosition(iPoint(2300, -800));
	if(App->map->Load("AlphaOne.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		uchar* data2 = NULL;
		if(App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);
		if (App->map->CreateConstructibleMap1(w, h, &data) && App->map->CreateConstructibleMap2(w,h,&data2))
			App->pathfinding->SetConstructibleMaps(w, h, data, data2);
		RELEASE_ARRAY(data2);
		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png", T_MAP);
	
	tower_tex = App->tex->GetTexture(T_TURRET);
	wall_tex = App->tex->GetTexture(T_WALL);
	//wall_tex = 

	CreateSceneUI();
	//ENTITIES
	townhall = (Building*)App->entity_manager->CreateBuilding(B_TOWNHALL, fPoint(-720, 672), S_ALLY);
	resource_food = (Resources*)App->entity_manager->CreateResource(FOOD, fPoint(1680, 1008));
	resource_gold = (Resources*)App->entity_manager->CreateResource(GOLD, fPoint(1680, 1008));
	resource_stone = (Resources*)App->entity_manager->CreateResource(STONE, fPoint(1680, 1008));
	resource_wood = (Resources*)App->entity_manager->CreateResource(WOOD, fPoint(1824, 1080));
	townhalltower1 = (Building*)App->entity_manager->CreateTower(T_BOMBARD_TOWER, fPoint(-624, 528));
	townhalltower2 = (Building*)App->entity_manager->CreateTower(T_BASIC_TOWER, fPoint(-432, 624));
	
	//Reset scores and timers
	game_time.Start();
	App->entity_manager->ResetScores();

	//Animation test
	a1 = new AnimationManager(App->anim->GetAnimationType(ANIM_SIMPLE_ARROW));
	a2 = new AnimationManager(App->anim->GetAnimationType(ANIM_FIRE_ARROW));
	a3 = new AnimationManager(App->anim->GetAnimationType(ANIM_ICE_ARROW));
	a4 = new AnimationManager(App->anim->GetAnimationType(ANIM_AIR_ARROW));
	a5 = new AnimationManager(App->anim->GetAnimationType(ANIM_SIMPLE_BOMB));
	a6 = new AnimationManager(App->anim->GetAnimationType(ANIM_FIRE_BOMB));
	a7 = new AnimationManager(App->anim->GetAnimationType(ANIM_ICE_BOMB));
	a8 = new AnimationManager(App->anim->GetAnimationType(ANIM_AIR_BOMB));
	a_fire = new AnimationManager(App->anim->GetAnimationType(ANIM_FIRE_FLOOR));
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	// debug pathfing ------------------
	int x, y;
	App->input->GetMousePosition(x, y);
	
	iPoint p = App->render->ScreenToWorld(x, y);
	
	p = App->map->WorldToMap(p.x, p.y);

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	//ANIMATION TEST
	SDL_Rect rect_test;
	iPoint pivot;

	a1->Update(rect_test, pivot);
	App->render->Blit(App->tex->GetTexture(T_ARROW_BOMB), 0, 300, &rect_test);
	a2->Update(rect_test, pivot);
	App->render->Blit(App->tex->GetTexture(T_ARROW_BOMB), 100, 300, &rect_test);
	a3->Update(rect_test, pivot);
	App->render->Blit(App->tex->GetTexture(T_ARROW_BOMB), 200, 300, &rect_test);
	a4->Update(rect_test, pivot);
	App->render->Blit(App->tex->GetTexture(T_ARROW_BOMB), 300, 300, &rect_test);
	a5->Update(rect_test, pivot);
	App->render->Blit(App->tex->GetTexture(T_ARROW_BOMB), 0, 400, &rect_test);
	a6->Update(rect_test, pivot);
	App->render->Blit(App->tex->GetTexture(T_ARROW_BOMB), 100, 400, &rect_test);
	a7->Update(rect_test, pivot);
	App->render->Blit(App->tex->GetTexture(T_ARROW_BOMB), 200, 400, &rect_test);
	a8->Update(rect_test, pivot);
	App->render->Blit(App->tex->GetTexture(T_ARROW_BOMB), 300, 400, &rect_test);

	a_fire->Update(rect_test, pivot);
	App->render->Blit(App->tex->GetTexture(T_FIRE), 0, 200, &rect_test, SDL_FLIP_NONE, pivot.x, pivot.y);
	//--


	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint res = App->render->ScreenToWorld(x, y);

	//TEST INVESTIGATIONS
	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		App->investigations->WantToInvestigate(App->investigations->GetInvestigation(INV_GOLD));

	if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
		App->investigations->WantToInvestigate(App->investigations->GetInvestigation(INV_INFANTRY_ATTACK));
	//--

	//CREATE UNITS
	if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
		App->entity_manager->CreateUnit(U_GOD, { -200.0f, 372 }, S_ALLY);

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		App->entity_manager->CreateUnit(U_SPEARMAN, { -200.0f, 372 }, S_ALLY);

	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
		App->entity_manager->CreateUnit(U_HEAVYCAVALRYARCHER, { -200.0f, 372 }, S_ALLY);

	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
		App->entity_manager->CreateUnit(U_KNIGHT, { -200.0f, 372 }, S_ALLY);

	if (App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
		App->entity_manager->CreateUnit(U_CAVALIER, { -200.0f, 372 }, S_ALLY);

	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
		App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, { -200.0f, 572 }, S_ENEMY);
	//--

	//DEBUG: increase resources
	if (App->debug_features.add_food)
	{
		resource_food->AddResource(1000);
		App->debug_features.add_food = false;
	}
	if (App->debug_features.add_wood)
	{
		resource_wood->AddResource(1000);
		App->debug_features.add_wood = false;
	}
	
	if (App->debug_features.add_stone)
	{
		resource_stone->AddResource(1000);
		App->debug_features.add_stone = false;
	}

	if (App->debug_features.add_gold)
	{
		resource_gold->AddResource(1000);
		App->debug_features.add_gold = false;
	}
	//--

	//App->map->Draw();

	// Camera Movement
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)// || ((y < (App->render->camera->GetHeight() / 30) && res.y > -30))
		App->render->camera->MoveUp(floor(450.0f * dt));

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)//|| ((y > 750) && res.y < 2317)
		App->render->camera->MoveDown(floor(450.0f * dt));

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)// || (x < (App->render->camera->GetWidth() / 70) && res.x > -2400)
		App->render->camera->MoveLeft(floor(450.0f * dt));

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)//|| (x >(((App->render->camera->GetWidth() / 50)*49.8f)) && res.x < 2349)
		App->render->camera->MoveRight(floor(450.0f * dt));

	/*
	if (App->render->camera->GetPosition().x > 2700)
	{
		App->render->camera->SetPosition(iPoint(2699, App->render->camera->GetPosition().y));
	}
	if (App->render->camera->GetPosition().x < -1200)
	{
		App->render->camera->SetPosition(iPoint(-1199, App->render->camera->GetPosition().y));
	}
	if (App->render->camera->GetPosition().y > 100)
	{
		App->render->camera->SetPosition(iPoint(App->render->camera->GetPosition().x, 100));
	}
	if (App->render->camera->GetPosition().y < -1600)
	{
		App->render->camera->SetPosition(iPoint(App->render->camera->GetPosition().x, -1600));
	}
	*/
	// Debug pathfinding ------------------------------

	iPoint p = App->render->ScreenToWorld(x, y);
	iPoint r = App->map->WorldToMap(p.x, p.y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	iPoint pos;

	for(std::vector<iPoint>::const_iterator item = App->pathfinding->GetLastPath().begin();
		item != App->pathfinding->GetLastPath().end();
		++item)
	{
		pos = App->map->MapToWorld(item->x, item->y);
		App->render->Blit(debug_tex, pos.x - 32, pos.y - 32);
	}

	//SELECTION
	if (selecting)
	{
		select_rect.w = x - App->render->camera->GetPosition().x;
		select_rect.h = y - App->render->camera->GetPosition().y;
		App->render->DrawQuad({ select_rect.x, select_rect.y, select_rect.w - select_rect.x, select_rect.h - select_rect.y }, 255, 255, 255, 255, false);


		if (App->input->GetMouseButtonDown(1) == KEY_UP)
		{
			App->entity_manager->SelectInQuad(select_rect);
			selecting = false;
		}
	}

	//--

	App->render->BlitAllEntities();

	if (placing_basic_tower == true)
	{
		PlacingBasicTower();
	}
	if (placing_bombard_tower == true)
	{
		PlacingBombardTower();
	}
	if (placing_wall == true) {
		PlacingWall();
	}
	return true;
}


// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	//TODO: defeat
	if (townhall->GetHp() <= 0)
	{
		lose = true;
		App->scene_manager->ChangeScene(SC_SCORE);
	}
	if (game_time.ReadSec() >= WINNING_TIME)
	{
		win = true;
		App->entity_manager->AddScore(townhall->GetHp() * 3);
		App->scene_manager->ChangeScene(SC_SCORE);
	}

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		App->scene_manager->ChangeScene(SC_SCORE);


	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
		win = true;
		App->scene_manager->ChangeScene(SC_SCORE);

	}

	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN) {
		lose = true;
		App->scene_manager->ChangeScene(SC_SCORE);
	}
	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->wave_manager->Disable();
	App->entity_manager->CleanUp();
	App->uimanager->SetAllToDelete();
	App->pathfinding->CleanUp();
	return true;
}

bool j1Scene::CanBuildTower()
{
	return resource_wood->CanUseResource(BASIC_TOWER_WOOD_COST) && resource_stone->CanUseResource(BASIC_TOWER_STONE_COST);
}

void j1Scene::BuildTower()
{
	resource_wood->UseResource(BASIC_TOWER_WOOD_COST);
	resource_stone->UseResource(BASIC_TOWER_STONE_COST);
}

bool j1Scene::CanBuildWall()
{
	return resource_stone->CanUseResource(BASIC_WALL_STONE_COST);
}

void j1Scene::BuildWall()
{
	resource_stone->UseResource(BASIC_WALL_STONE_COST);
}

bool j1Scene::CanTrainSoldier()
{
	return resource_wood->CanUseResource(TWOHANDED_WOOD_COST) && resource_stone->CanUseResource(TWOHANDED_STONE_COST);
}

void j1Scene::TrainSoldier()
{
	resource_wood->UseResource(TWOHANDED_WOOD_COST);
	resource_stone->UseResource(TWOHANDED_STONE_COST);
}

void j1Scene::PlacingBasicTower()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	iPoint r = App->map->WorldToMap(p.x, p.y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);
	SDL_Rect rect;
	if (CanBuildTower())
	{
		if (App->pathfinding->IsConstructible_neutral(r) == false && App->pathfinding->IsConstructible_ally(r) == false)
		{
			rect = {411,0,107,208};
			App->render->Blit(tower_tex, p.x, p.y, &rect, SDL_FLIP_NONE, 107 * 0.5, 206 * 0.902913);
		}
		else
		{
			rect = { 521,0,107,208 };
			//rect = { 610,1,107,206 };
			App->render->Blit(tower_tex, p.x, p.y, &rect, SDL_FLIP_NONE, 107 * 0.5, 206 * 0.902913);
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				if (App->collision->AbleToBuild(iPoint(p.x, p.y - 9)))
				{
					App->audio->PlayFx(App->entity_manager->fx_construction);
					if (App->pathfinding->IsConstructible_neutral(r) == true)
					{
						App->entity_manager->CreateTower(T_BASIC_TOWER, fPoint(p.x, p.y - 9));
						BuildTower();
					}
					else if (App->pathfinding->IsConstructible_ally(r) == true)
					{
						App->entity_manager->CreateTower(T_BASIC_TOWER, fPoint(p.x, p.y - 9));
						BuildTower();
					}
				}
			}
		}
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		placing_basic_tower = false;
	}
}

void j1Scene::PlacingBombardTower()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	iPoint r = App->map->WorldToMap(p.x, p.y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);
	SDL_Rect rect;
	if (CanBuildTower())
	{
		if (App->pathfinding->IsConstructible_neutral(r) == false && App->pathfinding->IsConstructible_ally(r) == false)
		{
			rect = {889,0,130,281};
			App->render->Blit(tower_tex, p.x, p.y, &rect, SDL_FLIP_NONE, 130 * 0.5, 281 * 0.902913);
		}
		else
		{
			rect = { 759,0,130,281};
			App->render->Blit(tower_tex, p.x, p.y, &rect, SDL_FLIP_NONE, 130 * 0.5, 281 * 0.914591);
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				if (App->collision->AbleToBuild(iPoint(p.x, p.y - 9)))
				{
					App->audio->PlayFx(App->entity_manager->fx_construction);
					if (App->pathfinding->IsConstructible_neutral(r) == true)
					{
						App->entity_manager->CreateTower(T_BOMBARD_TOWER, fPoint(p.x, p.y - 9));
						BuildTower();
					}
					else if (App->pathfinding->IsConstructible_ally(r) == true)
					{
						App->entity_manager->CreateTower(T_BOMBARD_TOWER, fPoint(p.x, p.y - 9));
						BuildTower();
					}
				}
			}
		}
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		placing_bombard_tower = false;
	}
}

void j1Scene::PlacingWall()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	iPoint r = App->map->WorldToMap(p.x, p.y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	if (App->pathfinding->IsConstructible_ally(r) == false)
	{
		SDL_Rect rect;
		rect = { 689,122,99,178 };
		App->render->Blit(wall_tex, p.x, p.y, &rect, SDL_FLIP_NONE, 0.494949 * 99, 178 * 0.865169);
	}
	else if (App->pathfinding->IsConstructible_ally(r) == true)
	{
		SDL_Rect rect;
		rect = { 690,303,99,178 };
		//rect = { 325,218,99,178 };
		App->render->Blit(wall_tex, p.x, p.y, &rect, SDL_FLIP_NONE, 0.494949 * 99, 178 * 0.865169);
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			if (App->collision->AbleToBuild(iPoint(p.x, p.y - 9)))
			{
				if (App->pathfinding->IsConstructible_ally(r) == true)
					if (CanBuildWall())
					{
						BuildWall();
						App->audio->PlayFx(App->entity_manager->fx_construction);
						App->entity_manager->CreateBuilding(B_STONE_WALL, fPoint(p.x, p.y - 9), S_ALLY);
					}
			}
		}
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		placing_wall = false;
	}
}

void j1Scene::HandleInput( SDL_Event event)
{
	int x = 0;
	int y = 0;
	SDL_Rect rect_ingame_no_ui = RECT_INGAME_WITHOUT_UI;

	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		
		App->input->GetMousePosition(x, y);

		if (x > rect_ingame_no_ui.x && x < rect_ingame_no_ui.w && y > rect_ingame_no_ui.y && y < rect_ingame_no_ui.h)
		{
			if (event.button.button == MK_LEFT)
			{
				App->entity_manager->UnselectEverything();

				select_rect.x = x - App->render->camera->GetPosition().x;
				select_rect.y = y - App->render->camera->GetPosition().y;
				select_rect.w = select_rect.x;
				select_rect.h = select_rect.y;

				selecting = true;
			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		break;

	case SDL_KEYDOWN:

		if (event.button.button == SDL_SCANCODE_L)
			App->LoadGame("save_game.xml");

		if (event.button.button == SDL_SCANCODE_S)
			App->SaveGame("save_game.xml");

		if (event.button.button == SDL_SCANCODE_SPACE)
			App->render->camera->Move(iPoint(1200, -250), 10);

		if (event.button.button == SDL_SCANCODE_1)
		{
			if (placing_basic_tower == false) 
			{
				placing_basic_tower = true;
				placing_bombard_tower = false;
				placing_wall = false;
			}
			else
				placing_basic_tower = false;
		}

		if (event.button.button == SDL_SCANCODE_2)
		{
			if (placing_bombard_tower == false)
			{
				placing_bombard_tower = true;
				placing_basic_tower = false;
				placing_wall = false;
			}
			else
				placing_bombard_tower = false;
		}

		if (event.button.button == SDL_SCANCODE_3)
		{
			if (placing_wall == false)
			{
			placing_wall = true;
			placing_basic_tower = false;
			placing_bombard_tower = false;
			}
			else
				placing_wall = false;
				
		}

		if (event.button.button == SDL_SCANCODE_4)
		{
			if (App->scene->CanTrainSoldier())
			{
				App->scene->TrainSoldier();
				App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(-480, 552), S_ALLY);
			}
		}
		break;

	case SDL_KEYUP:
		break;
	}
}

Resources* j1Scene::GetResource(RESOURCE_TYPE type)
{
	switch (type)
	{
	case FOOD:
		return resource_food;
		break;
	case WOOD:
		return resource_wood;
		break;
	case GOLD:
		return resource_gold;
		break;
	case STONE:
		return resource_stone;
		break;
	default:
		return nullptr;
		break;
	}
}

void j1Scene::CreateSceneUI()
{
	//UIElements
	//Top_HUD
	App->uimanager->AddComponent(UIT_UIIMAGE, { 0, 0, 1336, 23 }, { 0, 1011, 1366, 23 });

	//Objectives
	App->uimanager->AddButton({ 1252, 2, 36, 14 }, { 1252, 996, 36, 14 });

	//Tree Tech
	App->uimanager->AddButton({ 1288, 2, 35, 14 }, { 1289, 996, 35, 14 });

	//In Game Menu
	App->uimanager->AddButton({ 1323, 2, 36 , 15 }, { 1325, 996, 36, 14 });

	//Resource Panel
	App->uimanager->AddResourcesPanel();

	//Title Game Name
	App->uimanager->AddLabel(685, 3, "AoE 2: Defenders");

	//Down_HUD
	App->uimanager->AddComponent(UIT_UIIMAGE, { 0, 643, 1366, 125 }, { 0, 1036, 1366, 125 });

	//Button Description
	UICheckbutton* btn_description = App->uimanager->AddCheckButton({ 1316, 653, 19, 17 }, { 1347, 1163, 19, 17 }, { 1347, 1163, 19, 17 });
	btn_description->SetStat(CB_CHECK);

	//Panel Buttons
	UIHUDPanelButtons* panel = App->uimanager->AddPanelButtons();
	info_button* panel_btns = panel->AddButton(0, 0, 878, 910);
	panel_btns->SetBuilding(B_TURRET);
	panel_btns = panel->AddButton(2, 0, 774, 962);
	panel_btns->SetUnit(U_TWOHANDEDSWORDMAN, S_ALLY);
	panel_btns = panel->AddButton(1, 0, 774, 910);
	panel_btns->SetBuilding(B_STONE_WALL);

	//Panel Info
	App->uimanager->AddPanelInfo();

	//Panel Description
	UIHUDDescription* hud_description = App->uimanager->AddPanelDescription();
	hud_description->SetEnableButton(btn_description);

	//Town Hall Bar Life
	App->uimanager->AddTownHallBarLife();

	//New Wave Button
	UIButton* new_wave_button = App->uimanager->AddButton({ 1256, 95, 98 , 99 }, { 476, 1229, 98, 99 });
	new_wave_button->SetClickedTextRect({ 687, 1227, 104, 104 });
	new_wave_button->SetMouseOnTopTextRect({ 580, 1226, 104, 104 });

	//INFO SCORE, TIME, ENEMIES LEFT
	UIComponents* info_ui = App->uimanager->AddComponent(UIT_UIIMAGE, { 1236, 25, 130, 65 }, { 405, 1162, 130, 65 });
	App->uimanager->SetInfoUIComponent(info_ui);
}