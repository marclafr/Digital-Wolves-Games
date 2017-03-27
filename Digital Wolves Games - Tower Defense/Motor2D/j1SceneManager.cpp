#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1SceneManager.h"


j1SceneManager::j1SceneManager() : j1Module()
{
	name.assign("scene_manager");
}

// Destructor
j1SceneManager::~j1SceneManager()
{}

bool j1SceneManager::Awake(pugi::xml_node& config)
{
	

	return true;
}

bool j1SceneManager::Start()
{
	return true;
}

bool j1SceneManager::Update(float dt)
{
	return true;
}

bool j1SceneManager::PostUpdate()
{

	return true;
}


bool j1SceneManager::CleanUp()
{
	
	return true;
}


bool j1SceneManager::FadeToBlack()
{
	return true;
}