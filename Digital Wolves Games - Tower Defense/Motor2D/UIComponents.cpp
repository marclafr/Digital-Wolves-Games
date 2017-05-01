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

void UIComponents::Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h)
{
	rect_position.x = pos_x;
	rect_position.y = pos_y;
	rect_position.w = pos_w;
	rect_position.h = pos_h;

	rect_atlas.x = atlas_x;
	rect_atlas.y = atlas_y;
	rect_atlas.w = atlas_w;
	rect_atlas.h = atlas_h;
}

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

void UIComponents::HandleInput(SDL_Event event)
{
	//if(event == SDL_MOUSEBUTTON) el que sigui (sta dins, clica, etd)
	//if MOUSEBUTTONUP
	//if(event == SDL_KEYDOWN) el que sigui (crear unit, readrecar camera, etd)
	//if(KEYUP)
}

void UIComponents::SetInteractive(bool interactive)
{
	this->interactive = interactive;
}

void UIComponents::SetIsFocus(bool is_focus)
{
	this->is_focus = is_focus;
}

void UIComponents::SetFrom(UIComponents * from)
{
	this->from = from;
}

void UIComponents::SetToDelete(bool to_delete)
{
	this->to_delete = to_delete;
}

const bool UIComponents::GetInteractive() const
{
	return interactive;
}

const bool UIComponents::IsFocus() const
{
	return is_focus;
}

const SDL_Rect UIComponents::GetAtlasRect() const
{
	return rect_atlas;
}

const SDL_Rect UIComponents::GetPosRect() const
{
	return rect_position;
}

const bool UIComponents::ToDelete() const
{
	return to_delete;
}

const UICOMPONENT_TYPE UIComponents::GetType() const
{
	return type;
}

const UIComponents* UIComponents::GetFrom() const
{
	return from;
}

 const UIComponents * UIComponents::GetThisComponent() const
{
	return this;
}
