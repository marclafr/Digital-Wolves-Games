#define BACKGROUND_POSITION_NAME {1, 613, 194, 15}
#define BACKGROUND_POSITION_PRICE {1, 628, 194, 15}

#define ATLAS_BACKGROUND {1, 995, 194, 15}

#define X_LABEL_NAME 3
#define Y_LABEL_NAME 613
#define X_LABEL_PRICE 3
#define Y_LABEL_PRICE 628

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
	SetDraw(false);

	background_name = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	background_name->Set(BACKGROUND_POSITION_NAME, ATLAS_BACKGROUND);
	background_name->SetDraw(false);

	background_price = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	background_price->Set(BACKGROUND_POSITION_PRICE, ATLAS_BACKGROUND);
	background_price->SetDraw(false);

	description_name = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	description_name->Set(X_LABEL_NAME, Y_LABEL_NAME, "init");
	description_name->SetDraw(false);

	description_price = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	description_price->Set(X_LABEL_PRICE, Y_LABEL_PRICE, "init");
	description_price->SetDraw(false);

	unit_desc = UNIT_TYPE::U_NO_UNIT;
	build_desc = BUILDING_TYPE::B_NO_BUILDING;
}

void UIHUDDescription::SetEnableButton(UICheckbutton* btn)
{
	button_enable_component = btn;
}

bool UIHUDDescription::Update()
{
	enable = button_enable_component->clicked;

	if(enable)
		if(selected != nullptr && selected->btn->stat == UNSELECTED)
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
	description_name->ChangeText(GetUnitName(unit_desc));
	background_name->SetDraw(true);
	description_name->SetDraw(true);

	description_price->ChangeText(GetUnitPrice(unit_desc));
	background_price->SetDraw(true);
	description_price->SetDraw(true);
}

void UIHUDDescription::SetLabelBuilding()
{
	description_name->ChangeText(GetBuildingName(build_desc));
	background_name->SetDraw(true);
	description_name->SetDraw(true);

	description_price->ChangeText(GetBuildingPrice(build_desc));
	background_price->SetDraw(true);
	description_price->SetDraw(true);
}

void UIHUDDescription::Clear()
{
	unit_desc = UNIT_TYPE::U_NO_UNIT;
	build_desc = BUILDING_TYPE::B_NO_BUILDING;
	side_desc = Side::S_NO_SIDE;
	background_name->SetDraw(false);
	description_name->SetDraw(false);
	background_price->SetDraw(false);
	description_price->SetDraw(false);
	selected = nullptr;
}

