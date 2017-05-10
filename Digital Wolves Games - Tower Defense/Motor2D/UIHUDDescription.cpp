#include "UIHUDDescription.h"

#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1UIManager.h"
#include "Camera.h"
#include "Task.h"

#include "UIHUDPanelButtons.h"
#include "UILabel.h"
#include "UICheckbutton.h"

#include "UIGetEntitiesInfo.h"

#define BACKGROUND_POSITION_NAME {1, 593, 194, 15}
#define BACKGROUND_POSITION_PRICE {1, 608, 194, 15}

#define ATLAS_BACKGROUND {1, 995, 194, 15}

#define X_LABEL_NAME 3
#define Y_LABEL_NAME 593
#define X_LABEL_PRICE 3
#define Y_LABEL_PRICE 608

UIHUDDescription::UIHUDDescription(UICOMPONENT_TYPE type) : UIComponents(type)
{
	SetInteractive(false);

	unit_desc = U_NO_UNIT;
	build_desc = B_NO_BUILDING;
	tower_desc = T_NO_TYPE;
}

void UIHUDDescription::SetEnableButton(UICheckbutton* btn)
{
	button_enable_component = btn;
}

bool UIHUDDescription::Update()
{
	if (button_enable_component->GetStat() == CB_CHECK)
		enable = true;
	else
		enable = false;

	if (enable)
		if (selected != nullptr && selected->GetButton()->IsFocus() == false)
			Clear();

	return true;
}

void UIHUDDescription::SetDescription(info_button * if_btn)
{
	if (enable)
	{
		const EntityTask* e_task = (EntityTask*)if_btn->GetTask();

		switch (e_task->GetEntityType())
		{
		case ET_BASICTOWER:
			tower_desc = T_BASIC_TOWER;
			break;
		case ET_BOMBARDTOWER:
			tower_desc = T_BOMBARD_TOWER;
			break;
		case ET_WALL:
			build_desc = B_STONE_WALL;
			break;
		case ET_UNIT:
			TrainUnitTask* u_task = (TrainUnitTask*)e_task;
			unit_desc = u_task->GetUnitType();
			break;
		}

		/*
		switch (if_btn->GetTask()->)
		{
		case E_UNIT:
			unit_desc = if_btn->u_type;
			side_desc = if_btn->s_type;
			SetLabelUnit();
			break;

		case E_BUILDING:
			build_desc = if_btn->b_type;
			side_desc = if_btn->s_type;
			SetLabelBuilding();
			break;
		}
		*/
		selected = if_btn;
	}
}

void UIHUDDescription::SetLabelUnit()
{
	background_name = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_NAME, ATLAS_BACKGROUND);

	background_price = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_PRICE, ATLAS_BACKGROUND);

	description_name = App->uimanager->AddLabel(X_LABEL_NAME, Y_LABEL_NAME, GetUnitName(unit_desc));

	description_price = App->uimanager->AddLabel(X_LABEL_PRICE, Y_LABEL_PRICE, GetUnitPrice(unit_desc));
}

void UIHUDDescription::SetLabelBuilding()
{
	background_name = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_NAME, ATLAS_BACKGROUND);

	background_price = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_PRICE, ATLAS_BACKGROUND);

	description_name = App->uimanager->AddLabel(X_LABEL_NAME, Y_LABEL_NAME, GetBuildingName(build_desc));

	description_price = App->uimanager->AddLabel(X_LABEL_PRICE, Y_LABEL_PRICE, GetBuildingPrice(build_desc));
}

void UIHUDDescription::Clear()
{
	unit_desc = U_NO_UNIT;
	build_desc = B_NO_BUILDING;
	tower_desc = T_NO_TYPE;
	background_name->SetToDelete();
	description_name->SetToDelete();
	background_price->SetToDelete();
	description_price->SetToDelete();
	selected = nullptr;
}