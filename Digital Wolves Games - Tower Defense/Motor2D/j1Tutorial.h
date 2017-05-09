#ifndef __j1TUTORIAL_H__
#define __j1TUTORIAL_H__

#include "j1Module.h"

struct SDL_Texture;
class UILabel;
class UIComponents;
class UIButton;


class j1Tutorial : public j1Module
{
public:

	j1Tutorial();

	// Destructor
	virtual ~j1Tutorial();

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

	bool tutorial = false;
	UIComponents* tutorial1;
	UILabel* text_tutorial1;
	UILabel* text1_tutorial1;
	UILabel* text2_tutorial1;
	UILabel* text3_tutorial1;
	UILabel* text4_tutorial1;


private:
	
};

#endif // __j1MAINMENU_H__