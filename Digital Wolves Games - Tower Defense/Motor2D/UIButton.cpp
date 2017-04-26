#define RECT_NULL {0,0,0,0}

#include "UIButton.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1UIManager.h"
#include "Camera.h"
#include "j1Scene.h"

#include "UIHUDPanelButtons.h"
#include "UIHUDDescription.h"

UIButton::UIButton(UICOMPONENT_TYPE type) : UIComponents(type) {}

void UIButton::Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h)
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

void UIButton::Set(const SDL_Rect & position, const SDL_Rect & atlas)
{
	rect_position = position;
	rect_atlas = atlas;
}

void UIButton::Draw()
{
	SDL_Rect null_atlas = RECT_NULL;
	SDL_Rect actual_atlas;
	SDL_Rect actual_pos;

	switch (ba_stat)
	{
	case UIButton::BA_UNSELECTED:
		actual_atlas = GetAtlasRect();
		actual_pos = GetPosRect();
		break;
	case UIButton::BA_SELECTED:
		actual_atlas = atlas_selected;
		actual_pos = pos_selected;
		break;
	case UIButton::BA_CLICKED:
		actual_atlas = atlas_clicked;
		actual_pos = pos_clicked;
		break;
	case UIButton::BA_UNCLICKED:
		actual_atlas = atlas_unclicked;
		actual_pos = pos_unclicked;
		break;
	}
	if (SDL_RectEquals(&actual_atlas,&null_atlas) == SDL_TRUE)
		actual_atlas = GetAtlasRect();

	if (SDL_RectEquals(&actual_pos, &null_atlas) == SDL_TRUE)
		actual_pos = GetPosRect();

	if (GetFrom() != nullptr)
	{
		SDL_Rect mark_btn{ 969, 827, 29, 29 };

		switch (GetFrom()->GetType())
		{
		case UIT_UIHUDPANELBUTTONS:
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), rect_position.x - 2 - App->render->camera->GetPosition().x, rect_position.y - 2 - App->render->camera->GetPosition().y, &mark_btn, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), actual_pos.x - App->render->camera->GetPosition().x, actual_pos.y - App->render->camera->GetPosition().y, &actual_atlas, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
			break;
		}
	}
	else
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), actual_pos.x - App->render->camera->GetPosition().x, actual_pos.y - App->render->camera->GetPosition().y, &actual_atlas, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
}

void UIButton::HandleInput(SDL_Event event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (App->input->GetMouseButtonDown(LEFT_CLICK) == KEY_DOWN)
		{
			bs_stat = BS_CLICKED;
			if (ba_stat == BA_SELECTED)
				ba_stat = BA_CLICKED;
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (App->input->GetMouseButtonDown(LEFT_CLICK) == KEY_UP)
		{
			bs_stat = BS_UNCLICKED;
			if (ba_stat == BA_CLICKED)
				ba_stat = BA_UNCLICKED;
		}
		break;
	}
}

bool UIButton::Update()
{
	if (IsFocus() || (ba_stat == BA_UNCLICKED))
		ba_stat = BA_SELECTED;
	else
		ba_stat = BA_UNSELECTED;
	return true;
}

const BUTTON_STAT UIButton::GetStat() const
{
	return bs_stat;
}

void UIButton::SetAtlasSelected(const SDL_Rect & a_selected, const SDL_Rect& p_selected)
{
	atlas_selected = a_selected;
	pos_selected = p_selected;
}

void UIButton::SetAtlasClicked(const SDL_Rect & a_clicked, const SDL_Rect& p_clicked)
{
	atlas_clicked = a_clicked;
	pos_clicked = p_clicked;
}

void UIButton::SetAtlasUnclicked(const SDL_Rect & a_unclicked, const SDL_Rect& p_unclicked)
{
	atlas_unclicked = a_unclicked;
	pos_unclicked = p_unclicked;
}
