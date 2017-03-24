#include "UICheckbutton.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1UIManager.h"
#include "Camera.h"

UICheckbutton::UICheckbutton(UICOMPONENT_TYPE type) : UIButton(type) {}

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
	if (clicked)
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), rect_position.x - App->render->camera->GetPosition().x, rect_position.y - App->render->camera->GetPosition().y, &rect_atlas_clicked, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
	else
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), rect_position.x - App->render->camera->GetPosition().x, rect_position.y - App->render->camera->GetPosition().y, &rect_atlas, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);

}
