#include "UICheckbutton.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1UIManager.h"
#include "Camera.h"

UICheckbutton::UICheckbutton(UICOMPONENT_TYPE type) : UIComponents(type) {}

void UICheckbutton::Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h, uint atlas_clicked_x, uint atlas_clicked_y, uint atlas_clicked_w, uint atlas_clicked_h)
{
	rect_position.x = pos_x;
	rect_position.y = pos_y;
	rect_position.w = pos_w;
	rect_position.h = pos_h;

	rect_atlas.x = atlas_x;
	rect_atlas.y = atlas_y;
	rect_atlas.w = atlas_w;
	rect_atlas.h = atlas_h;

	rect_atlas_clicked.x = atlas_clicked_x;
	rect_atlas_clicked.y = atlas_clicked_y;
	rect_atlas_clicked.w = atlas_clicked_w;
	rect_atlas_clicked.h = atlas_clicked_h;
}

void UICheckbutton::Set(const SDL_Rect & position, const SDL_Rect & atlas, const SDL_Rect & atlas_clicked)
{
	rect_position = position;
	rect_atlas = atlas;
	rect_atlas_clicked = atlas_clicked;
}

void UICheckbutton::Draw()
{
	switch (cb_stat)
	{
	case CB_CHECK:
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), rect_position.x - App->render->camera->GetPosition().x, rect_position.y - App->render->camera->GetPosition().y, &rect_atlas_clicked, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
		break;
	case CB_UNCHECK:
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), rect_position.x - App->render->camera->GetPosition().x, rect_position.y - App->render->camera->GetPosition().y, &rect_atlas, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
		break;
	}
}


void UICheckbutton::HandleInput(SDL_Event event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (App->input->GetMouseButtonDown(LEFT_CLICK) == KEY_DOWN)
			if (cb_stat == CB_CHECK)
				cb_stat == CB_UNCHECK;
			else
				cb_stat == CB_CHECK;
		break;
	}
}

bool UICheckbutton::Update()
{
	return true;
}

void UICheckbutton::SetStat(CHECKBUTTON_STAT cb_stat)
{
	this->cb_stat = cb_stat;
}

const CHECKBUTTON_STAT UICheckbutton::GetStat() const
{
	return cb_stat;
}
