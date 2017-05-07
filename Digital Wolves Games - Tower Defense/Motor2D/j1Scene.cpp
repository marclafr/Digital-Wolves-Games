#define RECT_INGAME_WITHOUT_UI {0, 27, 1360, 624}

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
	App->map->Enable();
	App->anim->Enable();
	App->collision->Enable();
	App->entity_manager->Enable();
	App->projectile_manager->Enable();
	App->wave_manager->Enable();
	App->investigations->Enable();
	App->score->Enable();

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

	CreateSceneUI();
	//ENTITIES
	townhall = (Building*)App->entity_manager->CreateBuilding(B_TOWNHALL, fPoint(-720, 672), S_ALLY);
	resource_food = (Resources*)App->entity_manager->CreateResource(FOOD, fPoint(860, 640));
	resource_wood = (Resources*)App->entity_manager->CreateResource(WOOD, fPoint(1040, 775));
	resource_gold = (Resources*)App->entity_manager->CreateResource(GOLD, fPoint(1000, 645));
	resource_stone = (Resources*)App->entity_manager->CreateResource(STONE, fPoint(1200, 750));
	townhalltower1 = (Building*)App->entity_manager->CreateTower(T_BOMBARD_TOWER, fPoint(-624, 528));
	townhalltower2 = (Building*)App->entity_manager->CreateTower(T_BASIC_TOWER, fPoint(-432, 624));
	
	//Reset scores and timers
	App->score->Reset();

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
	
	iPoint result = App->map->MapToWorld(49, 73);

	a1->Update(rect_test, pivot);
	App->render->PushInGameSprite(App->tex->GetTexture(T_ARROW_BOMB), 0, 300, &rect_test);
	a2->Update(rect_test, pivot);
	App->render->PushInGameSprite(App->tex->GetTexture(T_ARROW_BOMB), 100, 300, &rect_test);
	a3->Update(rect_test, pivot);
	App->render->PushInGameSprite(App->tex->GetTexture(T_ARROW_BOMB), 200, 300, &rect_test);
	a4->Update(rect_test, pivot);
	App->render->PushInGameSprite(App->tex->GetTexture(T_ARROW_BOMB), 300, 300, &rect_test);
	a5->Update(rect_test, pivot);
	App->render->PushInGameSprite(App->tex->GetTexture(T_ARROW_BOMB), 0, 400, &rect_test);
	a6->Update(rect_test, pivot);
	App->render->PushInGameSprite(App->tex->GetTexture(T_ARROW_BOMB), 100, 400, &rect_test);
	a7->Update(rect_test, pivot);
	App->render->PushInGameSprite(App->tex->GetTexture(T_ARROW_BOMB), 200, 400, &rect_test);
	a8->Update(rect_test, pivot);
	App->render->PushInGameSprite(App->tex->GetTexture(T_ARROW_BOMB), 300, 400, &rect_test);

	a_fire->Update(rect_test, pivot);
	App->render->PushInGameSprite(App->tex->GetTexture(T_FIRE), 0, 200, &rect_test, SDL_FLIP_NONE, pivot.x, pivot.y);
	//--

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint res = App->render->ScreenToWorld(x, y);

	//TEST INVESTIGATIONS
	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		App->investigations->WantToInvestigate(App->investigations->GetInvestigation(INV_WOOD));

	if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
		App->investigations->WantToInvestigate(App->investigations->GetInvestigation(INV_STONE));

	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		App->investigations->WantToInvestigate(App->investigations->GetInvestigation(INV_CAVALRY_ATTACK));

	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		App->investigations->WantToInvestigate(App->investigations->GetInvestigation(INV_CAVALRY_DEFENSE));
	//--

	//CREATE UNITS
	if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
		App->entity_manager->CreateUnit(U_GOD, { -200.0f, 372 }, S_ALLY);

	/*
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		App->entity_manager->CreateUnit(U_SPEARMAN, { -200.0f, 372 }, S_ALLY);

	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
		App->entity_manager->CreateUnit(U_HEAVYCAVALRYARCHER, { -200.0f, 372 }, S_ALLY);

	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
		App->entity_manager->CreateUnit(U_KNIGHT, { -200.0f, 372 }, S_ALLY);

	if (App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
		App->entity_manager->CreateUnit(U_CAVALIER, { -200.0f, 372 }, S_ALLY);
	*/
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
		App->render->PushInGameSprite(debug_tex, pos.x - 32, pos.y - 32);
	}

	

	//--

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

	App->render->BlitGameScene();

	//SELECTION
	if (selecting)
	{
		select_rect.w = x - App->render->camera->GetPosition().x;
		select_rect.h = y - App->render->camera->GetPosition().y;
		App->render->DrawQuad({ select_rect.x, select_rect.y, select_rect.w - select_rect.x, select_rect.h - select_rect.y }, 255, 255, 255, 255, false);

		if (App->input->GetMouseButtonDown(MK_LEFT) == KEY_UP)
		{
			App->entity_manager->SelectInQuad(select_rect, selection);
			selecting = false;
		}
	}

	// Camera Movement (has to go after blit so that sprites print in the right camera position)
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		App->render->camera->MoveUp(floor(450.0f * dt));

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		App->render->camera->MoveDown(floor(450.0f * dt));

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		App->render->camera->MoveLeft(floor(450.0f * dt));

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		App->render->camera->MoveRight(floor(450.0f * dt));

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

	App->score->active = false;

	App->investigations->Disable();
	App->wave_manager->Disable();
	App->projectile_manager->Disable();
	App->entity_manager->Disable();
	App->collision->Disable();
	App->anim->Disable();
	App->map->Disable();
	App->pathfinding->Disable();

	
	App->uimanager->SetAllToDelete();

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
	SDL_Texture* tower_tex = App->tex->GetTexture(T_TURRET);
	//SDL_SetTextureAlphaMod(tower_tex, 180);

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
			App->render->PushInGameSprite(tower_tex, p.x, p.y, &rect, SDL_FLIP_NONE, 107 * 0.5, 206 * 0.902913);
		}
		else
		{
			rect = { 521,0,107,208 };
			//rect = { 610,1,107,206 };
			App->render->PushInGameSprite(tower_tex, p.x, p.y, &rect, SDL_FLIP_NONE, 107 * 0.5, 206 * 0.902913);
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
	SDL_Texture* tower_tex = App->tex->GetTexture(T_TURRET);
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
			App->render->PushInGameSprite(tower_tex, p.x, p.y, &rect, SDL_FLIP_NONE, 130 * 0.5, 281 * 0.902913);
		}
		else
		{
			rect = { 759,0,130,281};
			App->render->PushInGameSprite(tower_tex, p.x, p.y, &rect, SDL_FLIP_NONE, 130 * 0.5, 281 * 0.914591);
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
	SDL_Texture* wall_tex = App->tex->GetTexture(T_WALL);
	//SDL_SetTextureAlphaMod(wall_tex, 180);

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	iPoint r = App->map->WorldToMap(p.x, p.y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);
	SDL_Rect rectnowalkable;
	rectnowalkable = { 710,289,100,106 };
	SDL_Rect rectwalkable;
	rectwalkable = { 810,289,100,106 };

	if (App->pathfinding->IsConstructible_ally(r) == false && placing_wall_clicked == false)
	{
		SDL_Rect rect;
		rect = { 689,122,99,178 };
		App->render->PushInGameSprite(wall_tex, p.x, p.y, &rect, SDL_FLIP_NONE, 0.494949 * 99, 178 * 0.865169);
	}
	else if (App->pathfinding->IsConstructible_ally(r) == true)
	{
		SDL_Rect rect;
		rect = { 690,303,99,178 };
		//rect = { 325,218,99,178 };
		App->render->PushInGameSprite(wall_tex, p.x, p.y, &rect, SDL_FLIP_NONE, 0.494949 * 99, 178 * 0.865169);
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
			else
				direction = DIAGONAL;

			switch (direction)
			{
			case NORTH:
				for (int i = LastSelectedForWall.y; i <= firstSelectedForWall.y; i++)
				{
					iPoint actualpos = { firstSelectedForWall.x,i };
					iPoint temp2 = App->map->MapToWorld(LastSelectedForWall.x, i);

					if (App->pathfinding->IsConstructible_ally(actualpos) == true)
						App->render->Blit(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectwalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
					else
						App->render->Blit(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectnowalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				}
				break;
			case SOUTH:
				for (int i = firstSelectedForWall.y; i <= LastSelectedForWall.y; i++)
				{
					iPoint actualpos = { firstSelectedForWall.x,i };
					iPoint temp2 = App->map->MapToWorld(LastSelectedForWall.x, i);

					if (App->pathfinding->IsConstructible_ally(actualpos) == true)
						App->render->Blit(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectwalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
					else
						App->render->Blit(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectnowalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				}
				break;
			case EAST:
				for (int i = firstSelectedForWall.x; i <= LastSelectedForWall.x; i++)
				{
					iPoint actualpos = { i,firstSelectedForWall.y };
					iPoint temp2 = App->map->MapToWorld(i, LastSelectedForWall.y);

					if (App->pathfinding->IsConstructible_ally(actualpos) == true)
						App->render->Blit(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectwalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
					else
						App->render->Blit(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectnowalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				}
				break;
			case WEST:
				for (int i = LastSelectedForWall.x; i <= firstSelectedForWall.x; i++)
				{
					iPoint actualpos = { i,firstSelectedForWall.y };
					iPoint temp2 = App->map->MapToWorld(i, LastSelectedForWall.y);

					if (App->pathfinding->IsConstructible_ally(actualpos) == true)
						App->render->Blit(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectwalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
					else
						App->render->Blit(App->tex->GetTexture(T_TURRET), temp2.x, temp2.y, &rectnowalkable, SDL_FLIP_NONE, 0.49 * 100, 106 * 0.754717);
				}
				break;
			default:
				break;
			}

		}

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
			LastSelectedForWall = r;
			if (firstSelectedForWall.x == LastSelectedForWall.x && firstSelectedForWall.y > LastSelectedForWall.y)
				direction = NORTH;
			else if (firstSelectedForWall.x == LastSelectedForWall.x && firstSelectedForWall.y < LastSelectedForWall.y)
				direction = SOUTH;
			else if (firstSelectedForWall.x < LastSelectedForWall.x && firstSelectedForWall.y == LastSelectedForWall.y)
				direction = EAST;
			else if (firstSelectedForWall.x > LastSelectedForWall.x && firstSelectedForWall.y == LastSelectedForWall.y)
				direction = WEST;

			switch (direction)
			{
			case NORTH:
				for (int i = LastSelectedForWall.y; i <= firstSelectedForWall.y; i++)
				{
					iPoint actualpos = { firstSelectedForWall.x,i };
					if (App->pathfinding->IsConstructible_ally(actualpos) == true) 
					{
						iPoint temp2 = App->map->MapToWorld(LastSelectedForWall.x, i);
						App->entity_manager->CreateBuilding(B_STONE_WALL, fPoint{ float(temp2.x), float(temp2.y) }, S_ALLY);
					}
				}
				break;
			case SOUTH:
				for (int i = firstSelectedForWall.y; i <= LastSelectedForWall.y; i++)
				{
					iPoint actualpos = { firstSelectedForWall.x,i };
					if (App->pathfinding->IsConstructible_ally(actualpos) == true)
					{
						iPoint temp2 = App->map->MapToWorld(LastSelectedForWall.x, i);
						App->entity_manager->CreateBuilding(B_STONE_WALL, fPoint{ float(temp2.x), float(temp2.y) }, S_ALLY);
					}
				}
				break;
			case EAST:
				for (int i = firstSelectedForWall.x; i <= LastSelectedForWall.x; i++)
				{
					iPoint actualpos = { i,firstSelectedForWall.y };
					if (App->pathfinding->IsConstructible_ally(actualpos) == true)
					{
						iPoint temp2 = App->map->MapToWorld(i, LastSelectedForWall.y);
						App->entity_manager->CreateBuilding(B_STONE_WALL, fPoint{ float(temp2.x), float(temp2.y) }, S_ALLY);
					}
				}
				break;
			case WEST:
				for (int i = LastSelectedForWall.x; i <= firstSelectedForWall.x; i++)
				{
					iPoint actualpos = { i,firstSelectedForWall.y };
					if (App->pathfinding->IsConstructible_ally(actualpos) == true)
					{
						iPoint temp2 = App->map->MapToWorld(i, LastSelectedForWall.y);
						App->entity_manager->CreateBuilding(B_STONE_WALL, fPoint{ float(temp2.x), float(temp2.y) }, S_ALLY);
					}
				}
				break;
			case DIAGONAL:

				if (App->pathfinding->IsConstructible_ally(LastSelectedForWall) == true) 
				{
					iPoint temp2 = App->map->MapToWorld(LastSelectedForWall.x, LastSelectedForWall.y);
					App->entity_manager->CreateBuilding(B_STONE_WALL, fPoint{ float(temp2.x), float(temp2.y) }, S_ALLY);
				}
			default:
				break;
			}
			placing_wall_clicked = false;
		}
		/*if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
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
		}*/
		if (App->input->GetMouseButtonDown(MK_RIGHT) == KEY_DOWN)
		{
			placing_wall = false;
			placing_wall_clicked = false;
		}
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
		//Buildings
	panel->AddButton(BP_TOWNHALL, iPoint(0, 0), iPoint(826, 910), new PlaceBasicTowerTask());
	panel->AddButton(BP_TOWNHALL, iPoint(1, 0), iPoint(826, 910), new PlaceBombardTowerTask());
	panel->AddButton(BP_TOWNHALL, iPoint(2, 0), iPoint(826, 910), new PlaceWallTask());
		//Units
	panel->AddButton(BP_TOWNHALL,iPoint(0, 1), iPoint(826, 910), new TrainUnitTask(U_ARCHER));
	panel->AddButton(BP_TOWNHALL, iPoint(1, 1), iPoint(826, 910), new TrainUnitTask(U_TWOHANDEDSWORDMAN));
	panel->AddButton(BP_TOWNHALL, iPoint(2, 1), iPoint(826, 910), new TrainUnitTask(U_KNIGHT));

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
