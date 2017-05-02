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

UIButton::UIButton(UICOMPONENT_TYPE type) : UIComponents(type)
{}

UIButton::~UIButton()
{
	if (task != nullptr)
		DELETE_PTR(task);
}

void UIButton::Set(const SDL_Rect & position, const SDL_Rect & atlas)
{
	rect_position = position;
	rect_atlas = atlas;
}

void UIButton::Draw()
{
	SDL_Texture* atlas = (SDL_Texture*)App->uimanager->GetAtlas();
	SDL_Rect rect(GetPosRect());
	rect.x -= App->render->camera->GetPosition().x;
	rect.y -= App->render->camera->GetPosition().y;

	SDL_Rect r_atlas = RECT_NULL;
	switch (state)
	{
	case BS_NONE:
		r_atlas = GetAtlasRect();
		break;
	case BS_CLICKED:
		r_atlas = atlas_clicked;
		break;
	case BS_MOUSE_ON_TOP:
		r_atlas = atlas_mouse_on_top;
		break;
	}
	SDL_Rect null = RECT_NULL;
	if (SDL_RectEquals(&r_atlas, &null) == SDL_TRUE)
		r_atlas = GetAtlasRect();
	App->render->Blit(atlas, rect.x, rect.y, &r_atlas, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);

	if (is_ui_pannel)
	{
		SDL_Rect mark_btn{ 969, 827, 29, 29 };
		App->render->Blit(atlas, rect.x - UI_PANEL_MARK_MARGIN, rect.y - UI_PANEL_MARK_MARGIN, &mark_btn, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
	}
}

void UIButton::HandleInput(SDL_Event event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == MK_LEFT)
			state = BS_CLICKED;
		break;

	case SDL_MOUSEBUTTONUP:
		if (event.button.button == MK_LEFT)
		{
			state = BS_MOUSE_ON_TOP;
			task->Execute();
		}
		break;
	}
}

bool UIButton::Update()
{
	if (IsFocus() && (state != BS_CLICKED))
		state = BS_MOUSE_ON_TOP;
	else if (!IsFocus())
		state = BS_NONE;

	if (IsFocus())
		if (App->input->GetMouseButtonDown(MK_LEFT) == KEY_DOWN)
			state = BS_CLICKED;
		else if (App->input->GetMouseButtonDown(MK_LEFT) == KEY_UP)
		{
			state = BS_MOUSE_ON_TOP;
			if (task != nullptr)
				task->Execute();
		}

	Draw();

	return true;
}

const BUTTON_STAT UIButton::GetStat() const
{
	return state;
}

void UIButton::SetMouseOnTopTextRect(const SDL_Rect & rect)
{
	atlas_mouse_on_top = rect;
}

void UIButton::SetClickedTextRect(const SDL_Rect & rect)
{
	atlas_clicked = rect;
}

void UIButton::SetTask(Task* task)
{
	this->task = task;
}

void UIButton::IsUiPanel(bool is_ui_p)
{
	is_ui_pannel = is_ui_p;
}
