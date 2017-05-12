#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "Camera.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Scene.h"
#include "j1Animation.h"
#include "j1EntityManager.h"
#include "j1Collision.h"
#include "j1Score.h"
#include "ProjectileManager.h"
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
#include "j1UIManager.h"
#include "ResourceManager.h"
#include "UIGetEntitiesInfo.h"
#include "j1Tutorial.h"

#define RECT_INGAME_WITHOUT_UI {0, 15, 1360, 620}

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
	App->map->Enable();
	App->anim->Enable();
	App->collision->Enable();
	App->entity_manager->Enable();
	App->projectile_manager->Enable();
	App->wave_manager->Enable();//TODO put after tutorial 
	App->investigations->Enable();
	App->score->Enable();

	App->audio->PlayMusic("audio/music/Music_enviroment03.ogg", 0.0f);

	App->render->camera->SetPosition(iPoint(1700, -2400));
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

	CreateSceneUI();
	//ENTITIES
	townhall = (Building*)App->entity_manager->CreateBuilding(B_TOWNHALL, fPoint(-75, 272), S_ALLY);
	resources = new ResourceManager();
	App->entity_manager->CreateTower(T_BOMBARD_TOWER, fPoint(-300, 370));
	App->entity_manager->CreateTower(T_BASIC_TOWER, fPoint(150, 370));
	App->entity_manager->CreateBuilding(B_UNIVERSITY, fPoint(1073,799), S_ALLY);
	
	//Reset scores and timers
	App->score->Reset();
	if (App->tutorial->tutorial)
	{
		App->tutorial->Enable();
		App->tutorial->TutorialReset();
		TutorialUI();
	}

	//TEST FOR SAMPER: TODO delete this and all related
	anim_test2 = new AnimationManager(App->anim->GetAnimationType(ANIM_FIRE_EXPLOSION));
	anim_test3 = new AnimationManager(App->anim->GetAnimationType(ANIM_FIRE_FLOOR));
	anim_test4 = new AnimationManager(App->anim->GetAnimationType(ANIM_ICE_EXPLOSION));
	anim_test5 = new AnimationManager(App->anim->GetAnimationType(ANIM_ICE_FLOOR));
	//--
	

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	// debug pathfing ------------------
	int x, y;
	App->input->GetMousePosition(x, y);
	
	iPoint p = App->render->ScreenToWorld(x, y);
	LOG("%d, %d", p.x, p.y);
	p = App->map->WorldToMap(p.x, p.y);
	
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint res = App->render->ScreenToWorld(x, y);

	//TEST FOR SAMPER: TODO delete this and all related
	SDL_Rect rect;
	iPoint pivot;
	anim_test2->Update(rect, pivot);
	App->render->PushInGameSprite(App->tex->GetTexture(T_EXPLOSIONS_AND_FLOOR), 300, 800, &rect, SDL_FLIP_NONE, pivot.x, pivot.y);
	anim_test3->Update(rect, pivot);
	App->render->PushInGameSprite(App->tex->GetTexture(T_EXPLOSIONS_AND_FLOOR), 500, 800, &rect, SDL_FLIP_NONE, pivot.x, pivot.y);
	anim_test4->Update(rect, pivot);
	App->render->PushInGameSprite(App->tex->GetTexture(T_EXPLOSIONS_AND_FLOOR), 300, 1000, &rect, SDL_FLIP_NONE, pivot.x, pivot.y);
	anim_test5->Update(rect, pivot);
	App->render->PushInGameSprite(App->tex->GetTexture(T_EXPLOSIONS_AND_FLOOR), 500, 1000, &rect, SDL_FLIP_NONE, pivot.x, pivot.y);
	//--

	if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
		App->entity_manager->CreateUnit(U_MANGONEL, { -200, 575 }, S_ALLY);

	if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
		App->entity_manager->CreateUnit(U_MANGONEL, { -200, 575 }, S_ENEMY);

	//Test fade to black
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		App->render->camera->FadeToBlack(3, 2);

	//CREATE UNITS
	if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
		App->entity_manager->CreateUnit(U_GOD, { -200.0f, 372 }, S_ALLY);

	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
		App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, { -200.0f, 572 }, S_ENEMY);	
	//--

	if (placing_tower == T_BASIC_TOWER)
		PlacingTower(T_BASIC_TOWER);
	if (placing_tower == T_BOMBARD_TOWER)
		PlacingTower(T_BOMBARD_TOWER);
	if (placing_wall == true)
		PlacingWall();

	//SELECTION
	if(selecting)
		if (App->input->GetMouseButtonDown(MK_LEFT) == KEY_UP)
		{
			App->entity_manager->SelectInQuad(select_rect, selection);
			selecting = false;
		}

	App->debug_features.UpdateDebug();

	App->render->BlitGameScene();

	//Selection quad
	if (selecting)
	{
		select_rect.w = x - App->render->camera->GetPosition().x;
		select_rect.h = y - App->render->camera->GetPosition().y;
		App->render->DrawQuad({ select_rect.x, select_rect.y, select_rect.w - select_rect.x, select_rect.h - select_rect.y }, 255, 255, 255, 255, false);
	}

	// Camera Movement (has to go after blit so that sprites print in the right camera position)
	App->render->camera->KeyboardMove(dt);
	App->render->camera->MouseMove(x, y, dt);

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
	if (App->score->GetTime() >= WINNING_TIME)
	{
		win = true;
		App->score->AddScore(townhall->GetHp() * 3);
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

	delete resources;

	App->score->active = false;

	App->investigations->Disable();
	App->wave_manager->Disable();
	App->projectile_manager->Disable();
	App->entity_manager->Disable();
	App->collision->Disable();
	App->anim->Disable();
	App->map->Disable();
	App->pathfinding->Disable();
	App->tutorial->Disable();
	
	App->uimanager->SetAllToDelete();

	return true;
}

void j1Scene::PlacingTower(TOWER_TYPE type)
{
	SDL_Texture* tower_tex;
	SDL_Rect rect;
	iPoint pivot;

	int x = 0;
	int y = 0;
	App->input->GetMousePosition(x, y);

	iPoint pos = App->render->ScreenToWorld(x, y);
	iPoint tile_pos = App->map->WorldToMap(pos.x, pos.y);
	pos = App->map->WorldToMap(pos.x, pos.y);
	pos = App->map->MapToWorld(pos.x, pos.y);

	if (resources->CanBuildTower(type))
	{
		if (App->pathfinding->IsConstructible_neutral(tile_pos) == false && App->pathfinding->IsConstructible_ally(tile_pos) == false)
		{
			App->tex->GetTowerTexture(tower_tex, rect, pivot, type, BTT_RED); //texture rect
			App->render->PushInGameSprite(tower_tex, pos.x, pos.y, &rect, SDL_FLIP_NONE, pivot.x, pivot.y);
		}
		else
		{
			App->tex->GetTowerTexture(tower_tex, rect, pivot, type, BTT_GREEN);
			App->render->PushInGameSprite(tower_tex, pos.x, pos.y, &rect, SDL_FLIP_NONE, pivot.x, pivot.y);

			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				if (App->collision->AbleToBuild(pos))
				{
					App->audio->PlayFx(App->entity_manager->fx_construction);

					if (App->pathfinding->IsConstructible_neutral(tile_pos) == true)
						resources->BuildTower(type, pos);
					else if (App->pathfinding->IsConstructible_ally(tile_pos) == true)
						resources->BuildTower(type, pos);
				}
			}
		}
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
		placing_tower = T_NO_TYPE;
}
//SDL_SetTextureAlphaMod(wall_tex, 180);


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
			if (event.button.button == MK_LEFT)
			{
				App->entity_manager->UnselectEverything();

				select_rect.x = x - App->render->camera->GetPosition().x;
				select_rect.y = y - App->render->camera->GetPosition().y;
				select_rect.w = select_rect.x;
				select_rect.h = select_rect.y;

				selecting = true;
				App->entity_manager->CheckClick(x,y);
			}

		if (event.button.button == MK_RIGHT)
		{
			placing_wall = false;
			placing_tower = T_NO_TYPE;
		}

		if (App->input->GetMouseButtonDown(3) == KEY_DOWN)
		if (selection.size() > 0 && selection[0]->GetEntityType() == E_UNIT)
			{
				iPoint objective;
				App->input->GetMousePosition(objective.x, objective.y);
				objective.x -= App->render->camera->GetPosition().x;
				objective.y -= App->render->camera->GetPosition().y;

				for (std::vector<Entity*>::iterator it = selection.begin(); it != selection.end(); ++it)
				{
					Unit* unit = (Unit*)(*it);
					unit->GoTo(objective);
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

		//building construction
		if (event.button.button == SDL_SCANCODE_1
			|| event.button.button == SDL_SCANCODE_2
			|| event.button.button == SDL_SCANCODE_3)
		{
			placing_wall = false;
			placing_tower = T_NO_TYPE;
		}

		if (event.button.button == SDL_SCANCODE_1)
			placing_tower = T_BASIC_TOWER;	

		if (event.button.button == SDL_SCANCODE_2)
			placing_tower = T_BOMBARD_TOWER;

		if (event.button.button == SDL_SCANCODE_3)
			placing_wall = true;
		//--

		if (event.button.button == SDL_SCANCODE_4)
			if (App->scene->resources->CanTrainSoldier(U_TWOHANDEDSWORDMAN))
				App->scene->resources->TrainSoldier(U_TWOHANDEDSWORDMAN);
		break;

	case SDL_KEYUP:
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
	
	//Minimap
	App->uimanager->AddMinimap();

	//Button Description
	UICheckbutton* btn_description = App->uimanager->AddCheckButton({ 1316, 653, 19, 17 }, { 1347, 1163, 19, 17 }, { 1347, 1163, 19, 17 });
	btn_description->SetStat(CB_CHECK);

	//Panel Buttons
	UIHUDPanelButtons* panel = App->uimanager->AddPanelButtons();
		//Buildings
	panel->AddButton(BP_TOWNHALL, iPoint(0, 0), GetTowerIconPositionFromAtlas(T_BASIC_TOWER), new PlaceBasicTowerTask());
	panel->AddButton(BP_TOWNHALL, iPoint(1, 0), GetTowerIconPositionFromAtlas(T_BOMBARD_TOWER), new PlaceBombardTowerTask());
	panel->AddButton(BP_TOWNHALL, iPoint(2, 0), GetBuildingIconPositionFromAtlas(B_WOOD_WALL), new PlaceWallTask());
	
		//Turrets
	panel->AddButton(BP_TURRET, iPoint(0, 0), GetTowerIconPositionFromAtlas(T_FIRE_TOWER), new UpgradeTowerTask(TU_FIRE));
	panel->AddButton(BP_TURRET, iPoint(1, 0), GetTowerIconPositionFromAtlas(T_ICE_TOWER), new UpgradeTowerTask(TU_ICE));
	panel->AddButton(BP_TURRET, iPoint(2, 0), GetTowerIconPositionFromAtlas(T_AIR_TOWER), new UpgradeTowerTask(TU_AIR));
	panel->AddButton(BP_TURRET, iPoint(4, 2), {930,962}, new DeleteTowerTask());

	panel->AddButton(BP_CANNON, iPoint(0, 0), GetTowerIconPositionFromAtlas(T_BOMBARD_FIRE_TOWER), new UpgradeTowerTask(TU_FIRE));
	panel->AddButton(BP_CANNON, iPoint(1, 0), GetTowerIconPositionFromAtlas(T_BOMBARD_ICE_TOWER), new UpgradeTowerTask(TU_ICE));
	panel->AddButton(BP_CANNON, iPoint(2, 0), GetTowerIconPositionFromAtlas(T_BOMBARD_AIR_TOWER), new UpgradeTowerTask(TU_AIR));
	panel->AddButton(BP_CANNON, iPoint(4, 2), { 930,962 }, new DeleteTowerTask());

	panel->AddButton(BP_TURRET_UPGRADED, iPoint(4, 2), { 930,962 }, new DeleteTowerTask());
		
		//Walls
	panel->AddButton(BP_WOOD_WALL, iPoint(0, 0), { 930,962 }, new UpgradeWallTask(B_STONE_WALL));
	panel->AddButton(BP_WOOD_WALL, iPoint(4, 2), { 930,962 }, new DeleteWallTask());

	panel->AddButton(BP_STONE_WALL, iPoint(0, 0), { 930,962 }, new UpgradeWallTask(B_BRICK_WALL));
	panel->AddButton(BP_STONE_WALL, iPoint(4, 2), { 930,962 }, new DeleteWallTask());

	panel->AddButton(BP_BRICK_WALL, iPoint(4, 2), { 930,962 }, new DeleteWallTask());
		
		//Units
	panel->AddButton(BP_TOWNHALL, iPoint(0, 1), GetUnitIconPositionFromAtlas(U_CHAMPION), new TrainUnitTask(U_CHAMPION));
	panel->AddButton(BP_TOWNHALL, iPoint(1, 1), GetUnitIconPositionFromAtlas(U_HEAVYCAVALRYARCHER), new TrainUnitTask(U_HEAVYCAVALRYARCHER));
	panel->AddButton(BP_TOWNHALL, iPoint(2, 1), GetUnitIconPositionFromAtlas(U_PALADIN), new TrainUnitTask(U_PALADIN));
	panel->AddButton(BP_TOWNHALL,iPoint(3, 1), GetUnitIconPositionFromAtlas(U_ARBALEST), new TrainUnitTask(U_ARBALEST));

		//University

	panel->AddButton(BP_UNIVERSITY, iPoint(0, 0), { 852,884 }, new DoInvestigation(INV_FIRE_TOWER));
	panel->AddButton(BP_UNIVERSITY, iPoint(1, 0), { 878,884 }, new DoInvestigation(INV_ICE_TOWER));
	panel->AddButton(BP_UNIVERSITY, iPoint(2, 0), { 904,884 }, new DoInvestigation(INV_AIR_TOWER));

	panel->AddButton(BP_UNIVERSITY, iPoint(3, 0), { 852,858 }, new DoInvestigation(INV_FOOD));
	panel->AddButton(BP_UNIVERSITY, iPoint(4, 0), { 878,858 }, new DoInvestigation(INV_WOOD));
	panel->AddButton(BP_UNIVERSITY, iPoint(3, 1), { 904,858 }, new DoInvestigation(INV_STONE));
	panel->AddButton(BP_UNIVERSITY, iPoint(4, 1), { 930,858 }, new DoInvestigation(INV_GOLD));

	panel->AddButton(BP_UNIVERSITY, iPoint(0, 1), { 748,858 }, new DoInvestigation(INV_INFANTRY_ATTACK));
	panel->AddButton(BP_UNIVERSITY, iPoint(1, 1), { 748,910 }, new DoInvestigation(INV_INFANTRY_DEFENSE));
	panel->AddButton(BP_UNIVERSITY, iPoint(0, 2), { 800,858 }, new DoInvestigation(INV_CAVALRY_ATTACK));
	panel->AddButton(BP_UNIVERSITY, iPoint(1, 2), { 878,884 }, new DoInvestigation(INV_CAVALRY_DEFENSE));
	panel->AddButton(BP_UNIVERSITY, iPoint(2, 2), { 800,884 }, new DoInvestigation(INV_ARCHERS_ATTACK));
	panel->AddButton(BP_UNIVERSITY, iPoint(3, 2), { 748,936 }, new DoInvestigation(INV_ARCHERS_DEFENSE));


	
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

void j1Scene::PlacingWall()
{
	SDL_Texture* wall_tex = App->tex->GetTexture(T_TURRET);
	int placing_number = 1;
	int x = 0;
	int y = 0;
	App->input->GetMousePosition(x, y);

	iPoint p = App->render->ScreenToWorld(x, y);
	iPoint r = App->map->WorldToMap(p.x, p.y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);
	iPoint actualpos, temp2;
	SDL_Rect rectnowalkable;
	rectnowalkable = { 710,289,100,106 };
	SDL_Rect rectwalkable;
	rectwalkable = { 810,289,100,106 };

	if (App->pathfinding->IsConstructible_ally(r) == false && placing_wall_clicked == false || resources->CanBuildWall(B_WOOD_WALL) == false)
	{
		App->render->PushInGameSprite(wall_tex, p.x, p.y, &rectnowalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
	}
	else if (App->pathfinding->IsConstructible_ally(r) == true && placing_wall_clicked == false)
	{

		App->render->PushInGameSprite(wall_tex, p.x, p.y, &rectwalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		firstSelectedForWall = r;
		placing_wall_clicked = true;

	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		LastSelectedForWall = r;

		if (firstSelectedForWall.x == LastSelectedForWall.x && firstSelectedForWall.y > LastSelectedForWall.y)
			direction = NORTH;
		else if (firstSelectedForWall.x == LastSelectedForWall.x && firstSelectedForWall.y < LastSelectedForWall.y)
			direction = SOUTH;
		else if (firstSelectedForWall.x < LastSelectedForWall.x && firstSelectedForWall.y == LastSelectedForWall.y)
			direction = EAST;
		else if (firstSelectedForWall.x > LastSelectedForWall.x && firstSelectedForWall.y == LastSelectedForWall.y)
			direction = WEST;
		else if (firstSelectedForWall.x > LastSelectedForWall.x && firstSelectedForWall.y > LastSelectedForWall.y)
			direction = NORTH_EAST;
		else if (firstSelectedForWall.x < LastSelectedForWall.x && firstSelectedForWall.y > LastSelectedForWall.y)
			direction = NORTH_WEST;
		else if (firstSelectedForWall.x > LastSelectedForWall.x && firstSelectedForWall.y < LastSelectedForWall.y)
			direction = SOUTH_EAST;
		else if (firstSelectedForWall.x < LastSelectedForWall.x && firstSelectedForWall.y < LastSelectedForWall.y)
			direction = SOUTH_WEST;

		switch (direction)
		{
		case NORTH:
			for (int i = firstSelectedForWall.y; i >= LastSelectedForWall.y; i--)
			{
				actualpos = { firstSelectedForWall.x,i };
				temp2 = App->map->MapToWorld(firstSelectedForWall.x, i);

				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildAmountOfWalls(placing_number) == true)
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectwalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				else
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectnowalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				placing_number++;
			}
			break;
		case SOUTH:
			for (int i = firstSelectedForWall.y; i <= LastSelectedForWall.y; i++)
			{
				actualpos = { firstSelectedForWall.x,i };
				temp2 = App->map->MapToWorld(LastSelectedForWall.x, i);

				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildAmountOfWalls(placing_number) == true)
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectwalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				else
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectnowalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				placing_number++;
			}
			break;
		case EAST:
			for (int i = firstSelectedForWall.x; i <= LastSelectedForWall.x; i++)
			{
				actualpos = { i,firstSelectedForWall.y };
				temp2 = App->map->MapToWorld(i, LastSelectedForWall.y);

				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildAmountOfWalls(placing_number) == true)
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectwalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				else
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectnowalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				placing_number++;
			}
			break;
		case WEST:
			for (int i = firstSelectedForWall.x; i >= LastSelectedForWall.x; i--)
			{
				actualpos = { i,firstSelectedForWall.y };
				temp2 = App->map->MapToWorld(i, LastSelectedForWall.y);

				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildAmountOfWalls(placing_number) == true)
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectwalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				else
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectnowalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				placing_number++;
			}
			break;
		case NORTH_WEST:
			for (int i = firstSelectedForWall.y; i >= LastSelectedForWall.y; i--)
			{
				actualpos = { firstSelectedForWall.x,i };
				temp2 = App->map->MapToWorld(firstSelectedForWall.x, i);

				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildAmountOfWalls(placing_number) == true)
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectwalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				else
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectnowalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				placing_number++;
			}
			for (int i = firstSelectedForWall.x; i <= LastSelectedForWall.x; i++)
			{
				actualpos = { i,LastSelectedForWall.y };
				temp2 = App->map->MapToWorld(i, LastSelectedForWall.y);

				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildAmountOfWalls(placing_number) == true)
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectwalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				else
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectnowalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				placing_number++;
			}
			break;
		case NORTH_EAST:
			for (int i = firstSelectedForWall.y; i >= LastSelectedForWall.y; i--)
			{
				actualpos = { firstSelectedForWall.x,i };
				temp2 = App->map->MapToWorld(firstSelectedForWall.x, i);

				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildAmountOfWalls(placing_number) == true)
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectwalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				else
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectnowalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				placing_number++;
			}
			for (int i = firstSelectedForWall.x; i >= LastSelectedForWall.x; i--)
			{
				actualpos = { i,LastSelectedForWall.y };
				temp2 = App->map->MapToWorld(i, LastSelectedForWall.y);

				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildAmountOfWalls(placing_number) == true)
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectwalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				else
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectnowalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				placing_number++;
			}
			break;
		case SOUTH_WEST:
			for (int i = firstSelectedForWall.y; i <= LastSelectedForWall.y; i++)
			{
				actualpos = { firstSelectedForWall.x,i };
				temp2 = App->map->MapToWorld(firstSelectedForWall.x, i);

				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildAmountOfWalls(placing_number) == true)
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectwalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				else
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectnowalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				placing_number++;
			}
			for (int i = firstSelectedForWall.x; i <= LastSelectedForWall.x; i++)
			{
				actualpos = { i,LastSelectedForWall.y };
				temp2 = App->map->MapToWorld(i, LastSelectedForWall.y);

				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildAmountOfWalls(placing_number) == true)
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectwalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				else
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectnowalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				placing_number++;
			}
			break;
		case SOUTH_EAST:
			for (int i = firstSelectedForWall.y; i <= LastSelectedForWall.y; i++)
			{
				actualpos = { firstSelectedForWall.x,i };
				temp2 = App->map->MapToWorld(firstSelectedForWall.x, i);

				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildAmountOfWalls(placing_number) == true)
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectwalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				else
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectnowalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				placing_number++;
			}
			for (int i = firstSelectedForWall.x; i >= LastSelectedForWall.x; i--)
			{
				actualpos = { i,LastSelectedForWall.y };
				temp2 = App->map->MapToWorld(i, LastSelectedForWall.y);

				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildAmountOfWalls(placing_number) == true)
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectwalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				else
					App->render->PushInGameSprite(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectnowalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				placing_number++;
			}
			break;
		default:
			break;
		}

	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{

		switch (direction)
		{
		case NORTH:
			for (int i = firstSelectedForWall.y; i >= LastSelectedForWall.y; i--)
			{
				actualpos = { firstSelectedForWall.x,i };
				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildWall(B_WOOD_WALL))
				{
					temp2 = App->map->MapToWorld(LastSelectedForWall.x, i);
					App->entity_manager->CreateBuilding(B_WOOD_WALL, fPoint{ float(temp2.x), float(temp2.y) }, S_ALLY);
					resources->BuildWall(B_WOOD_WALL);
				}

			}
			break;
		case SOUTH:
			for (int i = firstSelectedForWall.y; i <= LastSelectedForWall.y; i++)
			{
				actualpos = { firstSelectedForWall.x,i };
				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildWall(B_WOOD_WALL))
				{
					temp2 = App->map->MapToWorld(LastSelectedForWall.x, i);
					App->entity_manager->CreateBuilding(B_WOOD_WALL, fPoint{ float(temp2.x), float(temp2.y) }, S_ALLY);
					resources->BuildWall(B_WOOD_WALL);
				}
			}
			break;
		case EAST:
			for (int i = firstSelectedForWall.x; i <= LastSelectedForWall.x; i++)
			{
				actualpos = { i,firstSelectedForWall.y };
				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildWall(B_WOOD_WALL))
				{
					temp2 = App->map->MapToWorld(i, LastSelectedForWall.y);
					App->entity_manager->CreateBuilding(B_WOOD_WALL, fPoint{ float(temp2.x), float(temp2.y) }, S_ALLY);
					resources->BuildWall(B_WOOD_WALL);
				}
			}
		case WEST:
			for (int i = firstSelectedForWall.x; i >= LastSelectedForWall.x; i--)
			{
				actualpos = { i,firstSelectedForWall.y };
				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildWall(B_WOOD_WALL))
				{
					temp2 = App->map->MapToWorld(i, LastSelectedForWall.y);
					App->entity_manager->CreateBuilding(B_WOOD_WALL, fPoint{ float(temp2.x), float(temp2.y) }, S_ALLY);
					resources->BuildWall(B_WOOD_WALL);
				}
			}
			break;
		case NORTH_WEST:
			for (int i = firstSelectedForWall.y; i >= LastSelectedForWall.y; i--)
			{
				actualpos = { firstSelectedForWall.x,i };
				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildWall(B_WOOD_WALL))
				{
					temp2 = App->map->MapToWorld(firstSelectedForWall.x, i);
					App->entity_manager->CreateBuilding(B_WOOD_WALL, fPoint{ float(temp2.x), float(temp2.y) }, S_ALLY);
					resources->BuildWall(B_WOOD_WALL);
				}
			}
			for (int i = firstSelectedForWall.x; i <= LastSelectedForWall.x; i++)
			{
				actualpos = { i,firstSelectedForWall.y };
				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildWall(B_WOOD_WALL))
				{
					temp2 = App->map->MapToWorld(i, LastSelectedForWall.y);
					App->entity_manager->CreateBuilding(B_WOOD_WALL, fPoint{ float(temp2.x), float(temp2.y) }, S_ALLY);
					resources->BuildWall(B_WOOD_WALL);
				}
			}
			break;
		case NORTH_EAST:
			for (int i = firstSelectedForWall.y; i >= LastSelectedForWall.y; i--)
			{
				actualpos = { firstSelectedForWall.x,i };
				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildWall(B_WOOD_WALL))
				{
					temp2 = App->map->MapToWorld(firstSelectedForWall.x, i);
					App->entity_manager->CreateBuilding(B_WOOD_WALL, fPoint{ float(temp2.x), float(temp2.y) }, S_ALLY);
					resources->BuildWall(B_WOOD_WALL);
				}
			}
			for (int i = firstSelectedForWall.x; i >= LastSelectedForWall.x; i--)
			{
				actualpos = { i,firstSelectedForWall.y };
				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildWall(B_WOOD_WALL))
				{
					temp2 = App->map->MapToWorld(i, LastSelectedForWall.y);
					App->entity_manager->CreateBuilding(B_WOOD_WALL, fPoint{ float(temp2.x), float(temp2.y) }, S_ALLY);
					resources->BuildWall(B_WOOD_WALL);
				}
			}
			break;
		case SOUTH_WEST:
			for (int i = firstSelectedForWall.y; i <= LastSelectedForWall.y; i++)
			{
				actualpos = { firstSelectedForWall.x,i };
				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildWall(B_WOOD_WALL))
				{
					temp2 = App->map->MapToWorld(firstSelectedForWall.x, i);
					App->entity_manager->CreateBuilding(B_WOOD_WALL, fPoint{ float(temp2.x), float(temp2.y) }, S_ALLY);
					resources->BuildWall(B_WOOD_WALL);
				}
			}
			for (int i = firstSelectedForWall.x; i <= LastSelectedForWall.x; i++)
			{
				actualpos = { i,firstSelectedForWall.y };
				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildWall(B_WOOD_WALL))
				{
					temp2 = App->map->MapToWorld(i, LastSelectedForWall.y);
					App->entity_manager->CreateBuilding(B_WOOD_WALL, fPoint{ float(temp2.x), float(temp2.y) }, S_ALLY);
					resources->BuildWall(B_WOOD_WALL);
				}
			}
			break;
		case SOUTH_EAST:
			for (int i = firstSelectedForWall.y; i <= LastSelectedForWall.y; i++)
			{
				actualpos = { firstSelectedForWall.x,i };
				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildWall(B_WOOD_WALL))
				{
					temp2 = App->map->MapToWorld(firstSelectedForWall.x, i);
					App->entity_manager->CreateBuilding(B_WOOD_WALL, fPoint{ float(temp2.x), float(temp2.y) }, S_ALLY);
					resources->BuildWall(B_WOOD_WALL);
				}
			}
			for (int i = firstSelectedForWall.x; i >= LastSelectedForWall.x; i--)
			{
				actualpos = { i,firstSelectedForWall.y };
				if (App->pathfinding->IsConstructible_ally(actualpos) == true && resources->CanBuildWall(B_WOOD_WALL))
				{
					temp2 = App->map->MapToWorld(i, LastSelectedForWall.y);
					App->entity_manager->CreateBuilding(B_WOOD_WALL, fPoint{ float(temp2.x), float(temp2.y) }, S_ALLY);
					resources->BuildWall(B_WOOD_WALL);
				}
			}
			break;
		default:
			break;
		}
		placing_wall_clicked = false;
	}

	if (App->input->GetMouseButtonDown(MK_RIGHT) == KEY_DOWN)
	{
		placing_wall = false;
		placing_wall_clicked = false;
	}
}

void j1Scene::TutorialUI()
{

	App->render->camera->SetPosition(iPoint(700, 20));

	App->tutorial->tutorial4 = App->uimanager->AddComponent(UIT_UIIMAGE, { 1112, 274, 418, 130 }, { 0, 2737, 418, 130 });
	App->tutorial->text_tutorial4 = App->uimanager->AddLabel(1130, 290, "Here is the resource and investigation zone", { 0,0,0,0 });
	App->tutorial->text1_tutorial4 = App->uimanager->AddLabel(1130, 310, "Select the University", { 0,0,0,0 });

	App->tutorial->tutorial3 = App->uimanager->AddComponent(UIT_UIIMAGE, { 1112, 274, 418, 130 }, { 0, 2606, 418, 130 });
	App->tutorial->text_tutorial3 = App->uimanager->AddLabel(1130, 290, "Move around using keys or mouse:", { 0,0,0,0 });
	App->tutorial->text1_tutorial3 = App->uimanager->AddLabel(1130, 310, "W", { 0,0,0,0 });
	App->tutorial->text2_tutorial3 = App->uimanager->AddLabel(1130, 330, "A", { 0,0,0,0 });
	App->tutorial->text3_tutorial3 = App->uimanager->AddLabel(1130, 350, "S", { 0,0,0,0 });
	App->tutorial->text4_tutorial3 = App->uimanager->AddLabel(1130, 370, "D", { 0,0,0,0 });

	App->tutorial->tutorial2 = App->uimanager->AddComponent(UIT_UIIMAGE, { 1112, 274, 418, 130 }, { 0, 2737, 418, 130 });
	App->tutorial->text_tutorial2 = App->uimanager->AddLabel(1130, 290, "Click on the Town Hall", { 0,0,0,0 });
	App->tutorial->text1_tutorial2 = App->uimanager->AddLabel(1130, 310, "On the left-bottom Corner you can create", { 0,0,0,0 });
	App->tutorial->text2_tutorial2 = App->uimanager->AddLabel(1130, 330, "towers, walls and more soldiers", { 0,0,0,0 });

	App->tutorial->tutorial1 = App->uimanager->AddComponent(UIT_UIIMAGE, { 1112, 274, 418, 130 }, { 0, 2606, 418, 130 });
	App->tutorial->text_tutorial1 = App->uimanager->AddLabel(1130, 290, "This is your Town Hall. Protect it!", { 0,0,0,0 });
	App->tutorial->text1_tutorial1 = App->uimanager->AddLabel(1130, 310, "Press 1 to build a Simple Tower", { 0,0,0,0 });
	App->tutorial->text2_tutorial1 = App->uimanager->AddLabel(1130, 330, "Press 2 to build a Bombard Tower", { 0,0,0,0 });
	App->tutorial->text3_tutorial1 = App->uimanager->AddLabel(1130, 350, "Press 3 to build a Wall", { 0,0,0,0 });
	App->tutorial->text4_tutorial1 = App->uimanager->AddLabel(1130, 370, "Press 4 to create a Soldier", { 0,0,0,0 });


}