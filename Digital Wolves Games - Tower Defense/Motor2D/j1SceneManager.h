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

	bool FadeToBlack();

private:

};

#endif // __j1SCENEMANAGER_H__