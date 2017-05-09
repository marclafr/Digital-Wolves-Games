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
	return true;
}