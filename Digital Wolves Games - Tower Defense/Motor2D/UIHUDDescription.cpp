#define BACKGROUND_POSITION_NAME {1, 593, 194, 15}
#define BACKGROUND_POSITION_PRICE {1, 608, 194, 15}

#define ATLAS_BACKGROUND {1, 995, 194, 15}

#define X_LABEL_NAME 3
#define Y_LABEL_NAME 593
#define X_LABEL_PRICE 3
#define Y_LABEL_PRICE 608

#include "UIHUDDescription.h"

#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1UIManager.h"
#include "Camera.h"

#include "UIHUDPanelButtons.h"
#include "UILabel.h"
#include "UICheckbutton.h"

#include "UIGetEntitiesInfo.h"

UIHUDDescription::UIHUDDescription(UICOMPONENT_TYPE type) : UIComponents(type)
{
	SetInteractive(false);

	unit_desc = UNIT_TYPE::U_NO_UNIT;
	build_desc = BUILDING_TYPE::B_NO_BUILDING;
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

	if(enable)
		if(selected != nullptr && selected->btn->IsFocus() == false)
			Clear();

	return true;
}

void UIHUDDescription::SetDescription(info_button * if_btn)
{
	if (enable)
	{
		switch (if_btn->e_type)
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
	unit_desc = UNIT_TYPE::U_NO_UNIT;
	build_desc = BUILDING_TYPE::B_NO_BUILDING;
	side_desc = Side::S_NO_SIDE;
	background_name->SetToDelete(true);
	description_name->SetToDelete(true);
	background_price->SetToDelete(true);
	description_price->SetToDelete(true);
	selected = nullptr;
}

