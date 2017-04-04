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

#include "UIButton.h"
#include "UILabel.h"
#include "UIHUDPanelButtons.h"
#include "UIHUDDescription.h"
#include "UIHUDResources.h"
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

	App->audio->PlayMusic("audio/music/Music_enviroment03.ogg");


	if(App->map->Load("NewMap.tmx") == true)
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
	//UIElements
		//Top_HUD
	top_hud = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	top_hud->Set({0, 0, 1336, 23}, {0, 1011, 1366, 23});
	top_hud->SetInteractive(false);

	objectives = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	objectives->Set({1252, 2, 36, 14}, {1252, 996, 36, 14});

	tree_tech = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	tree_tech->Set({1288, 2, 35, 14}, {1289, 996, 35, 14});

	ingame_menu = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	ingame_menu->Set({1323, 2, 36 , 15}, {1325, 996, 36, 14});

	resources_panel = (UIHUDResources*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIHUDRESOURCES);

	title_game_name = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	title_game_name->Set(685, 3, "AoE 2: Defenders");
	title_game_name->SetInteractive(false);
	
		//Down_HUD
	down_hud = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	down_hud->Set({0, 643, 1366, 125}, {0, 1036, 1366, 125});
	down_hud->SetInteractive(false);

	btn_description = (UICheckbutton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UICHECKBUTTON);
	btn_description->clicked = true;
	btn_description->Set({1316, 653, 19, 17}, {1347, 1163, 19, 17}, { 1347, 1163, 19, 17 });

	panel = (UIHUDPanelButtons*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIHUDPANELBUTTONS);
	info_button* panel_btns = nullptr;
	panel_btns = panel->AddButton(0, 0, 878, 910);
	panel_btns->SetBuilding(TURRET);
	panel_btns = panel->AddButton(2, 0, 774, 962);
	panel_btns->SetUnit(TWOHANDEDSWORDMAN, ALLY);
	panel_btns = panel->AddButton(2, 1, 774, 962);
	panel_btns->SetUnit(TWOHANDEDSWORDMAN, ENEMY);

	panel_info = (UIHUDPanelInfo*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIHUDPANELINFO);

	hud_description = (UIHUDDescription*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIHUDDESCRIPTION);
	hud_description->SetEnableButton(btn_description);
	/*
	//Entity Manager
	//ALLIES
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(100, 105), ALLY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(70, 115), ALLY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(40, 130), ALLY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(10, 145), ALLY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(-20, 160), ALLY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(130, 135), ALLY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(100, 150), ALLY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(70, 165), ALLY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(40, 180), ALLY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(10, 195), ALLY);

	//ENEMIES
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(500, 350), ENEMY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(470, 365), ENEMY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(440, 380), ENEMY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(410, 395), ENEMY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(380, 410), ENEMY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(530, 385), ENEMY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(500, 385), ENEMY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(470, 400), ENEMY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(440, 415), ENEMY);
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(410, 430), ENEMY);

	//TOWERS
	App->entity_manager->CreateBuilding(TURRET, fPoint(-48, 87), ENEMY);
	App->entity_manager->CreateBuilding(TURRET, fPoint(-144, 40), ENEMY);
	App->entity_manager->CreateBuilding(TURRET, fPoint(45, 40), ENEMY);
	App->entity_manager->CreateBuilding(TURRET, fPoint(145, 85), ENEMY);


	//RESOURCES
	resources_panel->AddResource((Resources*)App->entity_manager->CreateResource(STONE, fPoint(450, 850)));
	*/
	resource_stone = (Resources*)App->entity_manager->CreateResource(STONE, fPoint(-75, 50));
	resources_panel->AddResource(resource_stone);
	resource_wood = (Resources*)App->entity_manager->CreateResource(WOOD, fPoint(75, 50));
	resources_panel->AddResource(resource_wood);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	
	iPoint p = App->render->ScreenToWorld(x, y);
	
	p = App->map->WorldToMap(p.x, p.y);
	
		
	if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if(origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			App->entity_manager->GetUnitsPath(p);
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint res = App->render->ScreenToWorld(x, y);
	/*int a = 7;
	int b = 22;
	iPoint aa = App->map->MapToWorld(a, b);
	LOG("%i, %i", aa.x, aa.y);*/
	// Camera Movement

	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN )
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");
	
	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT ||  ((y < (App->render->camera->GetHeight() / 30) && res.y > -30)))
		App->render->camera->MoveUp(floor(200.0f * dt));

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || ((y > 580 && y< 642) && res.y < 2317))
		App->render->camera->MoveDown(floor(200.0f * dt));

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || (x < (App->render->camera->GetWidth() / 50 + 30) && res.x > -2400))
		App->render->camera->MoveLeft(floor(200.0f * dt));

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || (x > (((App->render->camera->GetWidth() / 50)*48.8f)) && res.x < 2349))
		App->render->camera->MoveRight(floor(200.0f * dt));

	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_REPEAT)
		App->render->camera->Move(iPoint(300,300), 10);

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
		App->render->camera->ZoomIn();

	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		App->render->camera->ZoomOut();

	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_REPEAT)
		App->render->camera->FadeToBlack(2,2,2);

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT)
		App->win->SetScale(2);

	App->map->Draw();
	

	// Debug pathfinding ------------------------------

	iPoint p = App->render->ScreenToWorld(x, y);
	iPoint r = App->map->WorldToMap(p.x, p.y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x - 44, p.y - 31);

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		if (placing_tower == false) placing_tower = true;
		else placing_tower = false;
		if (placing_wall == true) placing_wall = false;

	}
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		if (placing_wall == false) placing_wall = true;
		else placing_wall = false;
		if (placing_tower == true) placing_tower = false;

	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(p.x, p.y), ALLY);

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(p.x, p.y), ENEMY);

	


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
						BuildTower();
						if (App->pathfinding->IsConstructible_neutral(r) == true)
							App->entity_manager->CreateBuilding(TURRET, fPoint(p.x, p.y - 9), NEUTRAL);
						else if (App->pathfinding->IsConstructible_ally(r) == true)
							App->entity_manager->CreateBuilding(TURRET, fPoint(p.x, p.y - 9), ALLY);
						placing_tower = false;
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
		}
		else if (App->pathfinding->IsConstructible_ally(r) == true) {
			SDL_Rect rect;
			rect = { 325,218,99,178};
			App->render->Blit(wall_tex, p.x, p.y, &rect, SDL_FLIP_NONE, 0.494949*99, 178*0.865169);
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				if (App->collision->AbleToBuild(iPoint(p.x, p.y - 9)))
				{
					if (App->pathfinding->IsConstructible_ally(r) == true)
						App->entity_manager->CreateWall(STONE_WALL, fPoint(p.x, p.y - 9));
					placing_wall = false;
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

	//Unit test

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

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
