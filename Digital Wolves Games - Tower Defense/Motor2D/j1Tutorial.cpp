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
#include "j1Tutorial.h"
#include "j1SceneManager.h"
#include "Task.h"

#include "UIButton.h"
#include "UILabel.h"
#include "j1UIManager.h"

j1Tutorial::j1Tutorial() : j1Module()
{
	name.assign("tutorial");
}

// Destructor
j1Tutorial::~j1Tutorial()
{}

// Called before render is available
bool j1Tutorial::Awake()
{
	LOG("Loading Tutorial");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Tutorial::Start()
{

	return true;
}

// Called each loop iteration
bool j1Tutorial::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Tutorial::Update(float dt)
{

	Tutorial1();
	if (tutorial1_completed)
	{
		Tutorial2();
		if (tutorial2_completed)
		{
			Tutorial3();
			if (tutorial3_completed)
			{
				Tutorial4();
			}
		}
		
	}
	
	if (tutorial1_part1 && tutorial1_part2 && tutorial1_part3 && tutorial1_part4)
	{
		tutorial1_completed = true;
	}

	if (tutorial3_part1 && tutorial3_part2 && tutorial3_part3 && tutorial3_part4)
	{
		tutorial3_completed = true;
	}


	TutorialsProgression();

	return true;
}

// Called each loop iteration
bool j1Tutorial::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Tutorial::CleanUp()
{
	LOG("Freeing  MainMenu");
	App->uimanager->SetAllToDelete();
	tutorial = false;
	return true;
}


void j1Tutorial::TutorialsProgression()
{
	if (tutorial1_completed)
	{
		tutorial1->SetToDelete();
		text_tutorial1->SetToDelete();
		text1_tutorial1->SetToDelete();
		text2_tutorial1->SetToDelete();
		text3_tutorial1->SetToDelete();
		text4_tutorial1->SetToDelete();
	}

	if (tutorial2_completed)
	{
		tutorial2->SetToDelete();
		text_tutorial2->SetToDelete();
		text1_tutorial2->SetToDelete();
		text2_tutorial2->SetToDelete();
	}

	if (tutorial3_completed)
	{
		tutorial3->SetToDelete();
		text_tutorial3->SetToDelete();
		text1_tutorial3->SetToDelete();
		text2_tutorial3->SetToDelete();
		text3_tutorial3->SetToDelete();
		text4_tutorial3->SetToDelete();
	}

	if (tutorial4_completed)
	{
		tutorial4->SetToDelete();
		text_tutorial4->SetToDelete();
		text1_tutorial4->SetToDelete();

	}
}

void j1Tutorial::TutorialReset()
{
	tutorial1_completed = false;
	tutorial1_part1 = false;
	tutorial1_part2 = false;
	tutorial1_part3 = false;
	tutorial1_part4 = false;
	tutorial2_completed = false;
	tutorial2_part1 = false;
	TownHallSelected = false;
	PanelSelected = false;
	tutorial3_completed = false;
	tutorial3_part1 = false;
	tutorial3_part2 = false;
	tutorial3_part3 = false;
	tutorial3_part4 = false;
	UniversitySelected = false;
}

void j1Tutorial::Tutorial1()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		tutorial1_part1 = true;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		tutorial1_part2 = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) {
		tutorial1_part3 = true;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) {
		tutorial1_part4 = true;
	}

	if (!tutorial1_completed)
	{
		if (tutorial1_part1)
			{
				text1_tutorial1->ChangeColor({ 255,255,255,0 });
			}	
		if (tutorial1_part2)
			{
				text2_tutorial1->ChangeColor({ 255,255,255,0 });
			}
		if (tutorial1_part3)
			{
				text3_tutorial1->ChangeColor({ 255,255,255,0 });
			}
		if (tutorial1_part4)
			{
				text4_tutorial1->ChangeColor({ 255,255,255,0 });
			}
	}
}

void j1Tutorial::Tutorial2()
{
	if (TownHallSelected) {
		tutorial2_part1 = true;
	}

	if (PanelSelected) {
		tutorial2_completed = true;
	}

	if (!tutorial2_completed)
	{
		if (tutorial2_part1)
		{
			text_tutorial2->ChangeColor({ 255,255,255,0 });
		}
	}
}

void j1Tutorial::Tutorial3()
{
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		tutorial3_part1 = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
		tutorial3_part2 = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		tutorial3_part3 = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {
		tutorial3_part4 = true;
	}

	if (!tutorial3_completed)
	{
		if (tutorial3_part1)
		{
			text1_tutorial3->ChangeColor({ 255,255,255,0 });
		}
		if (tutorial3_part2)
		{
			text2_tutorial3->ChangeColor({ 255,255,255,0 });
		}
		if (tutorial3_part3)
		{
			text3_tutorial3->ChangeColor({ 255,255,255,0 });
		}
		if (tutorial3_part4)
		{
			text4_tutorial3->ChangeColor({ 255,255,255,0 });
		}
	}
}

void j1Tutorial::Tutorial4()
{
	if(!tutorial4_part1)
	{
		App->render->camera->SetPosition(iPoint(-800 , -500));
		tutorial4_part1 = true;
	}
	if (UniversitySelected) {
		tutorial4_part2 = true;
	}
	if (!tutorial4_completed)
	{
		if (tutorial4_part2)
		{
			text1_tutorial4->ChangeColor({ 255,255,255,0 });
		}
	}
}
