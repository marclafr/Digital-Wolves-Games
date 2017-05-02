#define BLACK {0,0,0,0}

#define BAR_LIFE_CENTER 16

#include "UIHUDPanelInfo.h"

#include "j1App.h"
#include "j1UIManager.h"
#include "j1EntityManager.h"

#include "SDL\include\SDL_rect.h"

#include "UIGetEntitiesInfo.h"

#include "PanelInfoGroupSelection.h"
#include "PanelInfoOneSelection.h"

UIHUDPanelInfo::UIHUDPanelInfo(UICOMPONENT_TYPE type) : UIComponents(type)
{
	SetInteractive(false);
}

UIHUDPanelInfo::~UIHUDPanelInfo()
{
	if (!selection.empty())
	{
		selection.clear();

		if (actual_panelinfo != nullptr)
			delete actual_panelinfo;
	}
}

void UIHUDPanelInfo::AddEntitySelection(Entity* selected)
{
	selection_tmp.push_back(selected);
}

void UIHUDPanelInfo::DefineSelection()
{
	selection.clear();

	std::list<Entity*>::iterator item;
	item = selection_tmp.begin();

	while (item != selection_tmp.end())
	{
		if (item._Ptr->_Myval->GetEntityType() == E_UNIT && (*item)->GetSide() != S_ENEMY && (*item)->GetHp() > 0 && unit_selection == true)
		{
			selection.push_back(*item);
			(*item)->SetEntityStatus(ST_SELECTED);
		}

		if (item._Ptr->_Myval->GetEntityType() == E_UNIT && (*item)->GetSide() != S_ENEMY && (*item)->GetHp() > 0 && unit_selection == false)
		{
			selection.clear();

			selection.push_back(*item);
			(*item)->SetEntityStatus(ST_SELECTED);

			unit_selection = true;
		}

		if ((item._Ptr->_Myval->GetEntityType() == E_BUILDING || item._Ptr->_Myval->GetEntityType() == E_RESOURCE) && (*item)->GetSide() != S_ENEMY && (*item)->GetHp() > 0 && unit_selection == false)
		{
			selection.push_back(*item);
			(*item)->SetEntityStatus(ST_SELECTED);
		}

		if (selection.size() >= 25)
			break;

		item++;
	}

	selection_tmp.clear();

	CreatePanel();
}

void UIHUDPanelInfo::DefineOneSelected(Entity* oneselected)
{
	selection_tmp.push_back(oneselected);

	DeleteSelection();

	DefineSelection();
}

void UIHUDPanelInfo::DeleteSelection()
{
	selection.clear();

	if (actual_panelinfo != nullptr)
	{
		delete actual_panelinfo;
		actual_panelinfo = nullptr;
	}
}

void UIHUDPanelInfo::CreatePanel()
{
	if (selection.empty() == false)
	{
		if (selection.size() > 1)
		{
			actual_panelinfo = new GroupSelection(selection);
		}
		else
		{
			actual_panelinfo = new OneSelection(selection);
		}
		actual_panelinfo->Prepare();
	}

	unit_selection = false;
}

bool UIHUDPanelInfo::isSelectionTempEmpty()
{
	return selection_tmp.empty();
}

bool UIHUDPanelInfo::isSelectionEmpty()
{
	return selection.empty();
}

bool UIHUDPanelInfo::Update()
{
	if (actual_panelinfo != nullptr)
	{
		actual_panelinfo->Update();

		if (actual_panelinfo->GetEntityForOneSelected() != nullptr && !actual_panelinfo->if_ToDelete())
			DefineOneSelected(actual_panelinfo->GetEntityForOneSelected());

		if (actual_panelinfo->if_ToDelete())
			DeleteSelection();
	}

	return true;
}