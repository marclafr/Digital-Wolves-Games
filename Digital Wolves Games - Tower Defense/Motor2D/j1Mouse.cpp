#include "j1Mouse.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"

bool j1Mouse::Start()
{
	SDL_ShowCursor(SDL_DISABLE);

	return true;
}

bool j1Mouse::Update(float dt)
{
	if (App->scene->placing_wall == true || App->scene->placing_tower != T_NO_TYPE)
		icon_rect = { 24,0,35,35 };
	else
		icon_rect = { 0,0,24,32 };
	App->render->SetMouseIconRect(icon_rect);
	return true;
}
