#include "UIComponents.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1UIManager.h"
#include "Camera.h"
#include "SDL/include/SDL.h"

UIComponents::UIComponents(UICOMPONENT_TYPE type)
{
	this->type = type;
}

UIComponents::~UIComponents() {}

void UIComponents::Set(const SDL_Rect & position, const SDL_Rect & atlas)
{
	rect_position = position;
	rect_atlas = atlas;
}

void UIComponents::Draw()
{
	App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), rect_position.x - App->render->camera->GetPosition().x, rect_position.y - App->render->camera->GetPosition().y, &rect_atlas, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
}

bool UIComponents::Update() 
{
	Draw();
	return true;
}

void UIComponents::SetDraw(bool draw)
{
	this->draw = draw;
}

void UIComponents::SetInteractive(bool interactive)
{
	this->interactive = interactive;
}

void UIComponents::SetFrom(UIComponents * from)
{
	this->from = from;
}

const bool UIComponents::GetDraw() const
{
	return draw;
}

const bool UIComponents::GetInteractive() const
{
	return interactive;
}

const UICOMPONENT_STAT UIComponents::GetStat() const
{
	return stat;
}

const UIComponents* UIComponents::GetFrom() const
{
	return from;
}

 const UIComponents * UIComponents::GetThisComponent() const
{
	return this;
}
