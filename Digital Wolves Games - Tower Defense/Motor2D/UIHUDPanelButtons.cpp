#define LEFT_CLICK 1

#include "UIHUDPanelButtons.h"

#include "j1App.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "j1UIManager.h"
#include "j1Scene.h"
#include "SDL\include\SDL_rect.h"
#include "j1Collision.h"
#include "UIButton.h"
#include "UIHUDDescription.h"
#include "Towers.h"
#include "Entity.h"
#include "Units.h"
#include "Buildings.h"

UIHUDPanelButtons::UIHUDPanelButtons(UICOMPONENT_TYPE type) : UIComponents(type)
{
	SetInteractive(false);
	SetDraw(false);
}

UIHUDPanelButtons::~UIHUDPanelButtons()
{
	std::list<info_button*>::iterator item;
	item = panel.begin();

	while (item != panel.end())
	{
		delete	item._Ptr->_Myval;
		item++;
	}

	panel.clear();
}

bool UIHUDPanelButtons::Update()
{
	std::list<info_button*>::iterator ib_item = panel.begin();

	while (ib_item != panel.end())
	{
		if ((*ib_item)->btn->GetStat() == UICOMPONENT_STAT::SELECTED)
			App->scene->hud_description->SetDescription((*ib_item));

		if((*ib_item)->btn->GetStat() == UICOMPONENT_STAT::CLICKL_UP)
			if_active = (*ib_item);

		ib_item++;
	}
	
	if (if_active != nullptr)
		CreateEntity();

	return true;
}

//x - 0 to 4 | y - 0 to 2 | Max 15 buttons
info_button* UIHUDPanelButtons::AddButton(uint x, uint y, uint atlas_x, uint atlas_y)
{
	info_button* new_btn = new info_button();

	new_btn->btn = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	new_btn->x = x;
	new_btn->y = y;
	new_btn->btn->Set({ 26 + (30 * (int)x), 666 + (30 * (int)y), 29, 29 }, { (int)atlas_x, (int)atlas_y, 25, 25 });

	panel.push_back(new_btn);

	return new_btn;
}

void UIHUDPanelButtons::CreateEntity()
{
	if (if_active->e_type == E_BUILDING && if_active->b_type == B_TURRET) {
		if (App->scene->placing_tower == false)
			App->scene->placing_tower = true;
		if (App->scene->placing_wall == true)
			App->scene->placing_wall = false;
	}
	if (if_active->e_type == E_BUILDING && if_active->b_type == B_STONE_WALL) {
		if (App->scene->placing_wall == false)
			App->scene->placing_wall = true;
		if (App->scene->placing_tower == true)
			App->scene->placing_tower = false;
	}
	if (if_active->e_type == E_UNIT) {
		if (App->scene->CanTrainSoldier())
		{
			App->scene->TrainSoldier();
			App->entity_manager->CreateUnit(if_active->u_type, fPoint(-480, 552), if_active->s_type);
		}
		if_active = nullptr;
		return;
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN) {
		App->scene->placing_tower = false;
		if_active = nullptr;
		return;
	}
	if (App->input->GetMouseButtonDown(LEFT_CLICK) == KEY_DOWN)
	{
		int x;
		int y;
		App->input->GetMousePosition(x, y);
		iPoint p = App->render->ScreenToWorld(x, y);

		iPoint r = App->map->WorldToMap(p.x, p.y);
		iPoint s = App->map->MapToWorld(r.x, r.y);

		switch (if_active->e_type)
		{
		case ENTITY_TYPE::E_UNIT:

			if (App->scene->CanTrainSoldier())
			{
				App->scene->TrainSoldier();
				App->entity_manager->CreateUnit(if_active->u_type, fPoint(-720, 672), if_active->s_type);
			}
			break;

			case ENTITY_TYPE::E_BUILDING:
				if (App->scene->placing_tower == true) {
						if (App->collision->AbleToBuild(iPoint(s.x, s.y - 9)))
						{
							if (App->scene->CanBuildTower())
							{
								if (App->pathfinding->IsConstructible_ally(r) == true)
								{
									App->scene->BuildTower();
									App->entity_manager->CreateTower(T_BASIC_TOWER, fPoint(s.x, s.y - 9));
								}
								if (App->pathfinding->IsConstructible_neutral(r) == true)
								{
									App->scene->BuildTower();
									App->entity_manager->CreateTower(T_BASIC_TOWER, fPoint(s.x, s.y - 9));
								}
							}
					}
				}
			break;
		}
		if_active = nullptr;
	}
}

void info_button::SetUnit(UNIT_TYPE type, Side side)
{
	u_type = type;
	e_type = ENTITY_TYPE::E_UNIT;
	s_type = side;
}

void info_button::SetBuilding(BUILDING_TYPE type)
{
	b_type = type;
	e_type = ENTITY_TYPE::E_BUILDING;
}

void info_button::SetTurret(TOWER_TYPE type)
{
	t_type = type;
	e_type = ENTITY_TYPE::E_BUILDING;
}


