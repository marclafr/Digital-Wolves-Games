#define ICON_ATLASSIZE 25
#define ICON_SIZE 29
#define ICON_SEPARATION 30
#define PANEL_XPOSITION 26
#define PANEL_YPOSITION 666

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
#include "Task.h"

#include "UIButton.h"
#include "UIHUDDescription.h"

#include "Entity.h"
#include "Units.h"
#include "Buildings.h"
#include "Towers.h"

UIHUDPanelButtons::UIHUDPanelButtons(UICOMPONENT_TYPE type) : UIComponents(type)
{
	SetInteractive(false);
}

UIHUDPanelButtons::~UIHUDPanelButtons()
{
	std::vector<info_button*>::iterator ib_item;
	switch (panel_type)
	{
	case BP_TOWNHALL:
		for (ib_item = panel_townhall.begin(); ib_item != panel_townhall.end(); ++ib_item)
			(*ib_item)->ButtonToDelete();
		break;
	case BP_UNIVERSITY:
		for (ib_item = panel_university.begin(); ib_item != panel_university.end(); ++ib_item)
			(*ib_item)->ButtonToDelete();
		break;
	case BP_TURRET:
		for (ib_item = panel_turret.begin(); ib_item != panel_turret.end(); ++ib_item)
			(*ib_item)->ButtonToDelete();
		break;
	case BP_CANNON:
		for (ib_item = panel_cannon.begin(); ib_item != panel_cannon.end(); ++ib_item)
			(*ib_item)->ButtonToDelete();
		break;
	}
	
	for (ib_item = panel_townhall.begin(); ib_item != panel_townhall.end(); ++ib_item)
		DELETE_PTR(*ib_item);

	for (ib_item = panel_university.begin(); ib_item != panel_university.end(); ++ib_item)
		DELETE_PTR(*ib_item);

	for (ib_item = panel_turret.begin(); ib_item != panel_turret.end(); ++ib_item)
		DELETE_PTR(*ib_item);

	for (ib_item = panel_cannon.begin(); ib_item != panel_cannon.end(); ++ib_item)
		DELETE_PTR(*ib_item);
}

void UIHUDPanelButtons::SetPanel(Building* building)
{
	switch (building->GetBuildingType())
	{
	case B_TURRET:
		panel_type = BP_TURRET;
		b_selected = building;
		CreatePanel();
		break;
	case B_CANNON:
		panel_type = BP_CANNON;
		b_selected = building;
		CreatePanel();
		break;
	case B_TOWNHALL:
		panel_type = BP_TOWNHALL;
		b_selected = building;
		CreatePanel();
		break;
	case B_UNIVERSITY:
		panel_type = BP_UNIVERSITY;
		b_selected = building;
		CreatePanel();
		break;
	default:
		panel_type = BP_NONE;
		b_selected = nullptr;
		break;
	}
}

bool UIHUDPanelButtons::Update()
{
	if (b_selected != nullptr)
	{
		std::vector<info_button*>::iterator ib_item;
		switch (panel_type)
		{
		case BP_TOWNHALL:
			for (std::vector<info_button*>::iterator ib_item = panel_townhall.begin(); ib_item != panel_townhall.end(); ++ib_item)
			{
				if ((*ib_item)->GetButton()->IsFocus())
				{
					//App->uimanager->SetDescriptionHUDDescription(panel_active->at(i));
					break;
				}
			}
			break;
		case BP_UNIVERSITY:
			for (std::vector<info_button*>::iterator ib_item = panel_university.begin(); ib_item != panel_university.end(); ++ib_item)
			{
				if ((*ib_item)->GetButton()->IsFocus())
				{
					//App->uimanager->SetDescriptionHUDDescription(panel_active->at(i));
					break;
				}
			}
			break;
		case BP_TURRET:
			for (std::vector<info_button*>::iterator ib_item = panel_turret.begin(); ib_item != panel_turret.end(); ++ib_item)
			{
				if ((*ib_item)->GetButton()->IsFocus())
				{
					//App->uimanager->SetDescriptionHUDDescription(panel_active->at(i));
					break;
				}
			}
			break;
		case BP_CANNON:
			for (std::vector<info_button*>::iterator ib_item = panel_cannon.begin(); ib_item != panel_cannon.end(); ++ib_item)
			{
				if ((*ib_item)->GetButton()->IsFocus())
				{
					//App->uimanager->SetDescriptionHUDDescription(panel_active->at(i));
					break;
				}
			}
			break;
		}
	}
	return true;
}

//x - 0 to 4 | y - 0 to 2 | Max 15 buttons
info_button* UIHUDPanelButtons::AddButton(BUILDING_PANELINFO type, iPoint position, iPoint atlas, Task* task)
{
	info_button* new_btn = new info_button(position,atlas, task);

	switch (type)
	{
	case BP_TOWNHALL:
		panel_townhall.push_back(new_btn);
		return new_btn;
		break;
	case BP_UNIVERSITY:
		panel_university.push_back(new_btn);
		return new_btn;
		break;
	case BP_TURRET:
		panel_turret.push_back(new_btn);
		return new_btn;
		break;
	case BP_CANNON:
		panel_cannon.push_back(new_btn);
		return new_btn;
		break;
	}

	DELETE_PTR(new_btn);
	return nullptr;
}

void UIHUDPanelButtons::CreatePanel()
{
	std::vector<info_button*>::iterator ib_item;
	switch (panel_type)
	{
	case BP_TOWNHALL:
		for (ib_item = panel_townhall.begin(); ib_item != panel_townhall.end(); ++ib_item)
			(*ib_item)->CreateButton();
		break;
	case BP_UNIVERSITY:
		for (ib_item = panel_university.begin(); ib_item != panel_university.end(); ++ib_item)
			(*ib_item)->CreateButton();
		break;
	case BP_TURRET:
		for (ib_item = panel_turret.begin(); ib_item != panel_turret.end(); ++ib_item)
			(*ib_item)->CreateButton();
		break;
	case BP_CANNON:
		for (ib_item = panel_cannon.begin(); ib_item != panel_cannon.end(); ++ib_item)
			(*ib_item)->CreateButton();
		break;
	}
}

void UIHUDPanelButtons::DeletePanel()
{
	std::vector<info_button*>::iterator ib_item;
	switch (panel_type)
	{
	case BP_TOWNHALL:
		for (ib_item = panel_townhall.begin(); ib_item != panel_townhall.end(); ++ib_item)
			(*ib_item)->ButtonToDelete();
		break;
	case BP_UNIVERSITY:
		for (ib_item = panel_university.begin(); ib_item != panel_university.end(); ++ib_item)
			(*ib_item)->ButtonToDelete();
		break;
	case BP_TURRET:
		for (ib_item = panel_turret.begin(); ib_item != panel_turret.end(); ++ib_item)
			(*ib_item)->ButtonToDelete();
		break;
	case BP_CANNON:
		for (ib_item = panel_cannon.begin(); ib_item != panel_cannon.end(); ++ib_item)
			(*ib_item)->ButtonToDelete();
		break;
	}

	panel_type = BP_NONE;
	b_selected = nullptr;
}

void info_button::CreateButton()
{
	btn = App->uimanager->AddButton(
	{ PANEL_XPOSITION + (ICON_SEPARATION * position.x),PANEL_YPOSITION + (ICON_SEPARATION * position.y),ICON_SIZE, ICON_SIZE },
	{ atlas.x, atlas.y,ICON_ATLASSIZE, ICON_ATLASSIZE }
	);
	btn->SetTask(task);
	btn->SetNotDeleteTask();
}

void info_button::ButtonToDelete()
{
	btn->SetToDelete();
}

const UIButton* info_button::GetButton() const
{
	return btn;
}

const Task* info_button::GetTask() const
{
	return task;
}
