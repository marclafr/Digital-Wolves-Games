#include "UIHUDPanelButtons.h"

#include "j1App.h"
#include "j1UIManager.h"

#include "SDL\include\SDL_rect.h"

#include "UIButton.h"

#include "Units.h"

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

//x - 0 to 4 | y - 0 to 2 | Max 15 buttons
void UIHUDPanelButtons::AddButton(uint x, uint y, uint atlas_x, uint atlas_y)
{
	info_button* new_btn = new info_button;

	new_btn->btn = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	new_btn->x = x;
	new_btn->y = y;
	new_btn->btn->Set({ 26 + (30 * (int)x), 666 + (30 * (int)y), 29, 29 }, { (int)atlas_x, (int)atlas_y, 25, 25 });

	new_btn->btn->SetFrom(this);

	panel.push_back(new_btn);
}