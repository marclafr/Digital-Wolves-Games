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
#include "Units.h"

#include "UIButton.h"
#include "UILabel.h"
#include "UIHUDPanelButtons.h"
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


	if(App->map->Load("Prueba.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if(App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png", T_MAP);

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

	//Down_HUD
	down_hud = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	down_hud->Set({0, 643, 1366, 125}, {0, 1036, 1366, 125});
	down_hud->SetInteractive(false);

	description = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	description->Set({1316, 653, 19, 17}, {1347, 1163, 19, 17});

	panel = (UIHUDPanelButtons*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIHUDPANELBUTTONS);
	info_button* panel_btns = nullptr;
	panel->AddButton(2, 1, 800, 884);
	panel->AddButton(4, 1, 826, 884);
	panel_btns = panel->AddButton(0, 0, 878, 910);
	panel_btns->SetBuilding(TURRET);
	panel->AddButton(1, 0, 748, 936);
	panel->AddButton(4, 2, 904, 884);

	panel_info = (UIHUDPanelInfo*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIHUDPANELINFO);

	//Entity Manager
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(500, 300));
	//App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(500, 350));
	//App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(500, 400));
	//App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(550, 400));
	//App->entity_manager->CreatBuilding(TURRET, fPoint(600,300));


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
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		iPoint r = App->map->WorldToMap(p.x, p.y);
		iPoint s = App->map->MapToWorld(r.x, r.y);
		App->entity_manager->CreatBuilding(TURRET, fPoint(s.x, s.y - 9));

	}
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

	// -------
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera->MoveUp(floor(200.0f * dt));

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera->MoveDown(floor(200.0f * dt));

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera->MoveLeft(floor(200.0f * dt));

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
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
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x - 44, p.y - 31);

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