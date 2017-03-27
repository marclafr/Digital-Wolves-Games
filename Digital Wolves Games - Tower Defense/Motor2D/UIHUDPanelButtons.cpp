#define LEFT_CLICK 1

#include "UIHUDPanelButtons.h"

#include "j1App.h"
#include "j1Input.h"
#include "j1EntityManager.h"
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
	int x;
	int y;

	App->input->GetMousePosition(x, y);

	if (App->input->GetMouseButtonDown(LEFT_CLICK) == KEY_DOWN)
	{

		switch (if_active->e_type)
		{
			case ENTITY_TYPE::UNIT:
			App->entity_manager->CreateUnit(if_active->u_type, fPoint((float)x, (float)y));
			break;

			case ENTITY_TYPE::BUILDING:
			App->entity_manager->CreatBuilding(if_active->b_type, fPoint((float)x, (float)y));
			break;
		}
		if_active = nullptr;
	}
		


}

void info_button::SetUnit(UNIT_TYPE type)
{
	u_type = type;
	e_type = ENTITY_TYPE::UNIT;
}

void info_button::SetBuilding(BUILDING_TYPE type)
{
	b_type = type;
	e_type = ENTITY_TYPE::BUILDING;
}

