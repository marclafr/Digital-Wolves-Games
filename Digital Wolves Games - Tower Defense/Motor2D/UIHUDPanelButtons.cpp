#define LEFT_CLICK 1

#include "UIHUDPanelButtons.h"

#include "j1App.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "j1UIManager.h"

#include "SDL\include\SDL_rect.h"

#include "UIButton.h"

#include "Entity.h"
#include "Units.h"
#include "Buildings.h"

UIHUDPanelButtons::UIHUDPanelButtons(UICOMPONENT_TYPE type) : UIComponents(type)
{
	SetInteractive(false);
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

	new_btn->btn->from_if_HUDPB = new_btn;

	new_btn->btn->SetFrom(this);

	panel.push_back(new_btn);

	return new_btn;
}

void UIHUDPanelButtons::CreateEntity()
{
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
		case ENTITY_TYPE::UNIT:
				if (App->pathfinding->IsWalkable(r) == true)
					App->entity_manager->CreateUnit(if_active->u_type, fPoint(s.x, s.y - 9), if_active->s_type);
			break;

			case ENTITY_TYPE::BUILDING:
				if (App->pathfinding->IsWalkable(r) == true)
					App->entity_manager->CreateBuilding(if_active->b_type, fPoint(s.x, s.y - 9), ALLY);
			break;
		}
		if_active = nullptr;
	}
}

void info_button::SetUnit(UNIT_TYPE type, Side side)
{
	u_type = type;
	e_type = ENTITY_TYPE::UNIT;
	s_type = side;
}

void info_button::SetBuilding(BUILDING_TYPE type)
{
	b_type = type;
	e_type = ENTITY_TYPE::BUILDING;
}

