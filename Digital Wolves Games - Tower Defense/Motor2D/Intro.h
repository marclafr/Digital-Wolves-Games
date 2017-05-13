#ifndef __j1INTRO__
#define __j1INTRO__

#include "j1Module.h"

struct SDL_Texture;
class UILabel;
class UIComponents;
class UIButton;


class j1Intro : public j1Module
{
public:

	j1Intro();

	// Destructor
	virtual ~j1Intro();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Enable Module with submodules
	void EnableScene();

	bool game_scene = true;

	void SetRetPreUpdate(bool ret);

private:
	bool ret_preupdate = true;
};

#endif // __j1MAINMENU_H__