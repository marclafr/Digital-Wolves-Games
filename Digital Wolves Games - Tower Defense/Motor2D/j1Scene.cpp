#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1UIManager.h"
#include "j1Scene.h"
#include "j1Animation.h"
#include "j1EntityManager.h"
#include "Units.h"

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
	if(App->map->Load("iso_walk.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if(App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png");

	//UIElements
		//Top_HUD
	top_hud = (UIImage*)App->uimanager->addUIComponent(UIComponent_TYPE::UIIMAGE);
	top_hud->Set({0, 0, 1336, 23}, {0, 1011, 1366, 23});
	top_hud->interactive = false;

	objectives = (UIButton*)App->uimanager->addUIComponent(UIComponent_TYPE::UIBUTTON);
	objectives->Set({1252, 2, 36, 14}, {1252, 996, 36, 14});

	tree_tech = (UIButton*)App->uimanager->addUIComponent(UIComponent_TYPE::UIBUTTON);
	tree_tech->Set({1288, 2, 35, 14}, {1289, 996, 35, 14});

	ingame_menu = (UIButton*)App->uimanager->addUIComponent(UIComponent_TYPE::UIBUTTON);
	ingame_menu->Set({1323, 2, 36 , 15}, {1325, 996, 36, 14});

	//Down_HUD
	down_hud = (UIImage*)App->uimanager->addUIComponent(UIComponent_TYPE::UIIMAGE);
	down_hud->Set({0, 643, 1366, 125}, {0, 1036, 1366, 125});
	down_hud->interactive = false;

	description = (UIButton*)App->uimanager->addUIComponent(UIComponent_TYPE::UIBUTTON);
	description->Set({1316, 653, 19, 17}, {1347, 1163, 19, 17});

	panel = (UIHUDPanelButtons*)App->uimanager->addUIComponent(UIComponent_TYPE::UIHUDPANELBUTTONS);
	panel->AddButton(2, 1, 800, 884);
	panel->AddButton(4, 1, 826, 884);
	panel->AddButton(0, 0, 878, 910);
	panel->AddButton(1, 0, 748, 936);
	panel->AddButton(4, 2, 904, 884);

	panel_info = (UIHUDPanelInfo*)App->uimanager->addUIComponent(UIComponent_TYPE::UIHUDPANELINFO);

	//Entity Manager
	App->entity_manager->CreateUnit(CAVALRYARCHER, iPoint(30, 170));
	App->entity_manager->CreateUnit(CAVALRYARCHER, iPoint(600, 400));
	App->entity_manager->CreateUnit(CAVALRYARCHER, iPoint(630, 430));
	App->entity_manager->CreateUnit(CAVALRYARCHER, iPoint(660, 460));
	App->entity_manager->CreateUnit(CAVALRYARCHER, iPoint(690, 390));
	App->entity_manager->CreateUnit(CAVALRYARCHER, iPoint(700, 360));

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
	// -------
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= floor(200.0f * dt);

	App->map->Draw();

	// Debug pathfinding ------------------------------
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x - 32, p.y - 32);

	const std::vector<iPoint>* path = App->pathfinding->GetLastPath();
	
	if (path->size() != 0)
	{
		std::vector<iPoint>::const_iterator item = path->begin();

		while(item != path->end())
		{
			iPoint pos = App->map->MapToWorld(item->x, item->y);
			App->render->Blit(debug_tex, pos.x - 32, pos.y - 32);
			item++;
		}
	}

	//SELECTION
	App->input->GetMousePosition(x, y);

	if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
	{
		App->entity_manager->UnselectEverything();

		select_rect.x = x - App->render->camera.x;
		select_rect.y = y - App->render->camera.y;
		select_rect.w = select_rect.x;
		select_rect.h = select_rect.y;
	}

	else if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
	{
		select_rect.w = x - App->render->camera.x;
		select_rect.h = y - App->render->camera.y;
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



	//--


	//Animation test

	/*App->anim->DrawAnimation(TWOHANDEDSWORDMAN, DIE, WEST, { 150,150 });

	App->anim->DrawAnimation(TWOHANDEDSWORDMAN, WALK, EAST, { 300,150 });

	App->anim->DrawAnimation(TWOHANDEDSWORDMAN, ATTACK, WEST, { 450,150 });

	App->anim->DrawAnimation(TWOHANDEDSWORDMAN, DISAPPEAR, WEST, { 600,150 });


	//Archer Cavalry
	Animation* test = App->anim->DrawAnimation(CAVALRYARCHER, ATTACK, NORTH_EAST, { 150, 450 });
	test->SetSpeed(50);

	App->anim->DrawAnimation(CAVALRYARCHER, DIE, WEST, { 300, 450 });

	App->anim->DrawAnimation(CAVALRYARCHER, DISAPPEAR, WEST, { 450, 450 });

	App->anim->DrawAnimation(CAVALRYARCHER, IDLE, EAST, { 600, 450 });

	App->anim->DrawAnimation(CAVALRYARCHER, WALK, SOUTH_EAST, { 750, 450 });


	//Siege Ram

	App->anim->DrawAnimation(SIEGERAM, WALK, NORTH_WEST, { 150, 600 });

	App->anim->DrawAnimation(SIEGERAM, ATTACK, NORTH_WEST, { 300, 600 });

	App->anim->DrawAnimation(SIEGERAM, DIE, NORTH_WEST, { 450, 600 });

	App->anim->DrawAnimation(SIEGERAM, IDLE, NORTH_WEST, { 600, 600 });
	App->anim->DrawAnimation(SIEGERAM, ATTACK, NORTH_WEST, { 600, 600 });

	App->anim->DrawAnimation(SIEGERAM, DISAPPEAR, NORTH_WEST, { 750, 600 });
	*/

	//--

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