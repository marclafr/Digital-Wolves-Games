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

	/*TEST UI 
	//Definition UIElements
	uint width = 0;
	uint height = 0;
	App->win->GetWindowSize(width, height);


	banner = (UIImage*)App->uimanager->addUIComponent(UIComponent_TYPE::UIIMAGE);
	banner->Set({ (int)width / 2, (int)height / 4, 328, 103 }, { 485, 829, 328, 103 });

	text = (UILabel*)App->uimanager->addUIComponent(UIComponent_TYPE::UILABEL);
	text->Set(width / 2, height / 4 - 120, "Hello World");

	check_test = (UICheckbutton*)App->uimanager->addUIComponent(UIComponent_TYPE::UICHECKBUTTON);
	check_test->Set({ 100, 100, 328, 103 }, { 485, 829, 328, 103 }, { 100, 100, 328, 103 });
	check_test->title->Set(150, 75, "Test Chech Button");

	select_test = (UISelectOption*)App->uimanager->addUIComponent(UIComponent_TYPE::UISELECTOPTION);
	select_test->Set({ 100,500,100,50 }, { 485, 829, 100, 50 });
	select_test->title->Set(100, 475, "Select Your Destiny");
	select_test->AddOption("OMG");
	select_test->AddOption("YOU'RE FAGGOT");
	select_test->AddOption("LET ME SUICIDE");
	select_test->AddOption("FUCK OFF");
	*/

	App->units->CreateUnit(TWOHANDEDSWORDMAN, iPoint(800, 200));
	App->units->CreateUnit(CAVALRYARCHER, iPoint(900, 200));

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
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	// UIManager ---
	/* TEST
	if (text->stat == UIComponent_Stat::SELECTED)
	{
		if (right_click == true)
			text->ChangeText("Hello World - right click");
		else if (left_click == true)
			text->ChangeText("Hello World - left click");
		else
			text->ChangeText("It's hover");
	}
	else if (text->stat == UIComponent_Stat::UNSELECTED)
	{
		text->ChangeText("Hello World");
		right_click = false;
		left_click = false;
	}
	else if (text->stat == UIComponent_Stat::CLICKR_DOWN)
	{
		right_click = true;
		left_click = false;
	}
	else if (text->stat == UIComponent_Stat::CLICKL_DOWN)
	{
		left_click = true;
		right_click = false;
	}
	/**/
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

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	
	std::string title;
	title = ("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.size(),
		map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.c_str());

	// Debug pathfinding ------------------------------
	//int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const std::vector<iPoint>* path = App->pathfinding->GetLastPath();
	
	if (path->size() != 0)
	{
		std::vector<iPoint>::const_iterator item = path->begin();

		//for (uint i = 0; i < path->size() - 1; ++i)
		while(item != path->end())
		{
			iPoint pos = App->map->MapToWorld(item->x, item->y);
			App->render->Blit(debug_tex, pos.x, pos.y);
			item++;
		}
	}

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
