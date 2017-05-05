#ifndef __j1SCENEMANAGER_H__
#define __j1SCENEMANAGER_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

enum SCENES
{
	SC_NO_SCENE,
	SC_SCORE,
	SC_MAIN_MENU,
	SC_GAME
};

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

	void ChangeScene(SCENES to);

	j1Module* GetCurrentScenePtr() const;

	SCENES GetCurrentScene() const;

private:
	SCENES change_to = SC_NO_SCENE;

	j1Module* main_menu = nullptr;
	j1Module* game_scene = nullptr;
	j1Module* score_scene = nullptr;

	SCENES current_scene = SC_NO_SCENE;

};

#endif // __j1SCENEMANAGER_H__