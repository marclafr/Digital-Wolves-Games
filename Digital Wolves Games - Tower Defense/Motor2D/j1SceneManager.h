#ifndef __j1SCENEMANAGER_H__
#define __j1SCENEMANAGER_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1SceneManager : public j1Module
{
public:

	j1SceneManager();

	virtual ~j1SceneManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool ChangeScene(j1Module* scene_on , j1Module* scene_off);

private:
	SDL_Rect screen;

	j1Module* new_scene = nullptr;
	j1Module* old_scene = nullptr;
};

#endif // __j1SCENEMANAGER_H__