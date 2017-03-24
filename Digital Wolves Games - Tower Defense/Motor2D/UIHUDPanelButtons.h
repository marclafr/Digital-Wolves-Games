#ifndef __UIHUDPANELBUTTONS_H__
#define __UIHUDPANELBUTTONS_H__

#include "UIComponents.h"
#include "UIButton.h"

class UIHUDPanelButtons : public UIComponents
{
public:
	struct info_button
	{
		UIButton* btn;
		uint x;
		uint y;
	};

	std::list<info_button*> panel;

public:
	UIHUDPanelButtons(UICOMPONENT_TYPE type);

	~UIHUDPanelButtons();

	//x - 0 to 4 | y - 0 to 2 | Max 15 buttons
	void AddButton(uint x, uint y, uint atlas_x, uint atlas_y);
};

#endif // __UIHUDPANELBUTTONS_H__