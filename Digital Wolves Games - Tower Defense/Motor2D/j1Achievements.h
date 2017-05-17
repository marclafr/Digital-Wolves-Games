#pragma once
#ifndef __j1ACHIEVEMENTS_H__
#define __j1ACHIEVEMENTS_H__

#include "j1Module.h"

struct SDL_Texture;
class UILabel;
class UIComponents;
class UIButton;


class j1Achievements : public j1Module
{
public:

	j1Achievements();

	// Destructor
	virtual ~j1Achievements();

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

	void SetAchievements(int num, bool achi = false);
	bool GetAchievements(int num);
	void AchievementComplete();

private:
	bool achievement1;
};

#endif // __j1ACHIEVEMENTS_H__