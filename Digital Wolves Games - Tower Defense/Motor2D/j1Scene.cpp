#define RECT_INGAME_WITHOUT_UI {0, 27, 1360, 644}

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
	App->wave_manager->Enable();
	App->audio->PlayMusic("audio/music/Music_enviroment03.ogg");
	App->uimanager->Enable();

	App->render->camera->SetPosition(iPoint(2300, -800));

	if(App->map->Load("OtherMap.tmx") == true)
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
	townhall_bar_life->SetTownHall(townhall);
	resource_stone = (Resources*)App->entity_manager->CreateResource(STONE, fPoint(1680, 1008));
	resources_panel->AddResource(resource_stone);
	resource_wood = (Resources*)App->entity_manager->CreateResource(WOOD, fPoint(1824, 1080));
	resources_panel->AddResource(resource_wood);
	townhalltower1 = (Building*)App->entity_manager->CreateBuilding(B_TURRET, fPoint(-624, 528), S_ALLY);
	townhalltower2 = (Building*)App->entity_manager->CreateBuilding(B_TURRET, fPoint(-432, 624), S_ALLY);
	//Reset scores and timers
	game_time.Start();
	App->entity_manager->ResetScores();

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
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint res = App->render->ScreenToWorld(x, y);

	//DEBUG: increase resources
	if (App->debug_features.add_wood)
	{
		resource_wood->AddResource(1000);
		App->debug_features.add_wood = false;
	}
	
	if (App->debug_features.add_stone)
	{
		resource_stone->AddResource(1000);
		App->debug_features.add_stone = false;
	}//--

	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN )
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	// Camera Movement
	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT ||  ((y < (App->render->camera->GetHeight() / 30) && res.y > -30)))
		App->render->camera->MoveUp(floor(450.0f * dt));

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || ((y > 750) && res.y < 2317))
		App->render->camera->MoveDown(floor(450.0f * dt));

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || (x < (App->render->camera->GetWidth() / 70 ) && res.x > -2400))
		App->render->camera->MoveLeft(floor(450.0f * dt));

	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || (x > (((App->render->camera->GetWidth() / 50)*49.8f)) && res.x < 2349))
		App->render->camera->MoveRight(floor(450.0f * dt));

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

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
		App->render->camera->Move(iPoint(1200, -250), 10);

	//App->map->Draw();
	
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
		win = true;
		App->scene_manager->ChangeScene(App->main_menu, this);
		
	}

	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN) {
		lose = true;
		App->scene_manager->ChangeScene(App->main_menu, this);
	}

		

	// Debug pathfinding ------------------------------

	iPoint p = App->render->ScreenToWorld(x, y);
	iPoint r = App->map->WorldToMap(p.x, p.y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	//App->render->Blit(debug_tex, p.x - 44, p.y - 31);

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		if (placing_tower == false) placing_tower = true;
		else placing_tower = false;
		if (placing_wall == true) placing_wall = false;

	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		if (placing_wall == false) placing_wall = true;
		else placing_wall = false;
		if (placing_tower == true) placing_tower = false;

	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		if (App->scene->CanTrainSoldier())
		{
			App->scene->TrainSoldier();
			App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(-480, 552), S_ALLY);
		}
	}
	/*
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(p.x, p.y), S_ALLY);

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(p.x, p.y), S_ENEMY);
	*/
	const std::vector<iPoint>* path = App->pathfinding->GetLastPath();
	
	if (path->size() != 0)
	{
		std::vector<iPoint>::const_iterator item = path->begin();

		while(item != path->end())
		{
			iPoint pos = App->map->MapToWorld(item->x, item->y);
			//App->render->Blit(debug_tex, pos.x - 32, pos.y - 32);
			item++;
		}
	}

	//SELECTION
	App->input->GetMousePosition(x, y);

	SDL_Rect rect_ingame_no_ui = RECT_INGAME_WITHOUT_UI;
	if (x > rect_ingame_no_ui.x && x < rect_ingame_no_ui.w && y > rect_ingame_no_ui.y && y < rect_ingame_no_ui.h)
	{
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
		{
			App->entity_manager->UnselectEverything();

			select_rect.x = x - App->render->camera->GetPosition().x;
			select_rect.y = y - App->render->camera->GetPosition().y;
			select_rect.w = select_rect.x;
			select_rect.h = select_rect.y;
		}

		else if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
		{
			select_rect.w = x - App->render->camera->GetPosition().x;
			select_rect.h = y - App->render->camera->GetPosition().y;
			App->render->DrawQuad({ select_rect.x, select_rect.y, select_rect.w - select_rect.x, select_rect.h - select_rect.y }, 255, 255, 255, 255, false);
		}

		if (App->input->GetMouseButtonDown(1) == KEY_UP)
		{
			App->entity_manager->SelectInQuad(select_rect);
		}
	}
	//--

	App->render->BlitAllEntities();
	if (placing_tower == true)
	{
		if (CanBuildTower())
		{
			if (App->pathfinding->IsConstructible_neutral(r) == false && App->pathfinding->IsConstructible_ally(r) == false)
			{
				SDL_Rect rect;
				rect = { 1,284,107,206 };
				App->render->Blit(tower_tex, p.x, p.y, &rect, SDL_FLIP_NONE, 107 * 0.5, 206 * 0.902913);
			}
			else
			{
				SDL_Rect rect;
				rect = { 610,1,107,206 };
				App->render->Blit(tower_tex, p.x, p.y, &rect, SDL_FLIP_NONE, 107 * 0.5, 206 * 0.902913);
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
				{
					if (App->collision->AbleToBuild(iPoint(p.x, p.y - 9)))
					{
						App->audio->PlayFx(App->entity_manager->fx_construction);
						if (App->pathfinding->IsConstructible_neutral(r) == true)
						{
							App->entity_manager->CreateBuilding(B_TURRET, fPoint(p.x, p.y - 9), S_NEUTRAL);
							BuildTower();
						}
						else if (App->pathfinding->IsConstructible_ally(r) == true)
						{
							App->entity_manager->CreateBuilding(B_TURRET, fPoint(p.x, p.y - 9), S_ALLY);
							BuildTower();
						}
					}
				}
			}
		}
		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
		{
			placing_tower = false;
		}
	}

	if (placing_wall == true) {
		if (App->pathfinding->IsConstructible_ally(r) == false)
		{
			SDL_Rect rect;
			rect = { 689,122,99,178 };
			App->render->Blit(wall_tex, p.x, p.y, &rect, SDL_FLIP_NONE, 0.494949 * 99, 178 * 0.865169);
		}
		else if (App->pathfinding->IsConstructible_ally(r) == true)		
		{
			SDL_Rect rect;
			rect = { 325,218,99,178};
			App->render->Blit(wall_tex, p.x, p.y, &rect, SDL_FLIP_NONE, 0.494949*99, 178*0.865169);
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
		App->scene_manager->ChangeScene(App->main_menu, this);
	}
	if (game_time.ReadSec() >= WINNING_TIME)
	{
		win = true;
		App->entity_manager->AddScore(townhall->GetHp() * 3);
		App->scene_manager->ChangeScene(App->main_menu, this);
	}

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		App->scene_manager->ChangeScene(App->main_menu, this);

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->wave_manager->Disable();
	App->entity_manager->CleanUp();
	App->uimanager->Disable();
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

void j1Scene::CreateSceneUI()
{
	//UIElements
	//Top_HUD
	top_hud = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	top_hud->Set({ 0, 0, 1336, 23 }, { 0, 1011, 1366, 23 });
	top_hud->SetInteractive(false);

	objectives = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	objectives->Set({ 1252, 2, 36, 14 }, { 1252, 996, 36, 14 });

	tree_tech = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	tree_tech->Set({ 1288, 2, 35, 14 }, { 1289, 996, 35, 14 });

	ingame_menu = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	ingame_menu->Set({ 1323, 2, 36 , 15 }, { 1325, 996, 36, 14 });

	resources_panel = (UIHUDResources*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIHUDRESOURCES);

	title_game_name = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	title_game_name->Set(685, 3, "AoE 2: Defenders");
	title_game_name->SetInteractive(false);

	//Down_HUD
	down_hud = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	down_hud->Set({ 0, 643, 1366, 125 }, { 0, 1036, 1366, 125 });
	down_hud->SetInteractive(false);

	btn_description = (UICheckbutton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UICHECKBUTTON);
	btn_description->clicked = true;
	btn_description->Set({ 1316, 653, 19, 17 }, { 1347, 1163, 19, 17 }, { 1347, 1163, 19, 17 });

	panel = (UIHUDPanelButtons*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIHUDPANELBUTTONS);
	info_button* panel_btns = nullptr;
	panel_btns = panel->AddButton(0, 0, 878, 910);
	panel_btns->SetBuilding(B_TURRET);
	panel_btns = panel->AddButton(2, 0, 774, 962);
	panel_btns->SetUnit(U_TWOHANDEDSWORDMAN, S_ALLY);
	//	panel_btns = panel->AddButton(2, 1, 774, 962);
	//	panel_btns->SetUnit(U_TWOHANDEDSWORDMAN, S_ENEMY);
	panel_btns = panel->AddButton(1, 0, 774, 910);
	panel_btns->SetBuilding(B_STONE_WALL);

	panel_info = (UIHUDPanelInfo*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIHUDPANELINFO);

	hud_description = (UIHUDDescription*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIHUDDESCRIPTION);
	hud_description->SetEnableButton(btn_description);

	townhall_bar_life = (UIHUDTownHallBarLife*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIHUDTOWNHALLBARLIFE);
}