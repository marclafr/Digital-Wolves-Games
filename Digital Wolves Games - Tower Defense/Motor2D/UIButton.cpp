#include "UIButton.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1UIManager.h"
#include "Camera.h"

#include "UIHUDPanelButtons.h"

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
	if (GetFrom() != nullptr)
	{
		SDL_Rect mark_btn{ 969, 827, 29, 29 };

		switch (GetFrom()->type)
		{
		case UIHUDPANELBUTTONS:
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), rect_position.x - 2 - App->render->camera->GetPosition().x, rect_position.y - 2 - App->render->camera->GetPosition().y, &mark_btn, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), rect_position.x - App->render->camera->GetPosition().x, rect_position.y - App->render->camera->GetPosition().y, &rect_atlas, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
			break;
		}
	}
	else
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), rect_position.x - App->render->camera->GetPosition().x, rect_position.y - App->render->camera->GetPosition().y, &rect_atlas, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
}

bool UIButton::Update()
{
	switch (GetStat())
	{
	//Click of UIButton
	case CLICKL_DOWN:
		clicked = true;

		if (from_if_HUDPB != nullptr)
		{
			UIHUDPanelButtons* temp = (UIHUDPanelButtons*)GetFrom();
			temp->if_active = from_if_HUDPB;
		}
		break;

	//UIButton set click to false
	case CLICKL_UP:
		clicked = false;
		break;
	}

	return true;
}
