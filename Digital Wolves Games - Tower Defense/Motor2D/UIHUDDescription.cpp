#define X_BACKGROUND 1
#define Y_BACKGROUND 628

#define XATLAS_BACKGROUND 1
#define YATLAS_BACKGROUND 995
#define WATLAS_BACKGROUND 194
#define HATLAS_BACKGROUND 15

#define X_LABEL 3
#define Y_LABEL 628

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


	background = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	background->Set({ X_BACKGROUND, Y_BACKGROUND, WATLAS_BACKGROUND, HATLAS_BACKGROUND }, { XATLAS_BACKGROUND, YATLAS_BACKGROUND, WATLAS_BACKGROUND, HATLAS_BACKGROUND });
	background->SetDraw(false);

	description = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	description->Set(X_LABEL, Y_LABEL, "init");
	description->SetDraw(false);

	unit_desc = UNIT_TYPE::NO_UNIT;
	build_desc = BUILDING_TYPE::NO_BUILDING;
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
		case UNIT:
			unit_desc = if_btn->u_type;
			side_desc = if_btn->s_type;
			SetLabelUnit();
			break;

		case BUILDING:
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
	std::string tmp(GetUnitName(unit_desc));
	tmp += ": ";
	tmp += GetSideName(side_desc);
	description->ChangeText(tmp.c_str());
	background->SetDraw(true);
	description->SetDraw(true);
}

void UIHUDDescription::SetLabelBuilding()
{
	std::string tmp(GetBuildingName(build_desc));
	tmp += ": ";
	tmp += GetSideName(side_desc);
	description->ChangeText(tmp.c_str());
	background->SetDraw(true);
	description->SetDraw(true);
}

void UIHUDDescription::Clear()
{
	unit_desc = UNIT_TYPE::NO_UNIT;
	build_desc = BUILDING_TYPE::NO_BUILDING;
	side_desc = Side::NO_SIDE;
	background->SetDraw(false);
	description->SetDraw(false);
	selected = nullptr;
}

