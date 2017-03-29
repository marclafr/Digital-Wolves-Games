#ifndef __UIHUDDESCRIPTION_H__
#define __UIHUDDESCRIPTION_H__

#include "UIComponents.h"
#include "UIButton.h"
#include "UILabel.h"
#include "UICheckbutton.h"

enum UNIT_TYPE;
enum BUILDING_TYPE;
enum Side;
struct info_button;

class UIHUDDescription : public UIComponents
{
public:
	UICheckbutton* button_enable_component = nullptr;

	bool enable = false;

	UIComponents* background = nullptr;
	UILabel* description = nullptr;

	UNIT_TYPE unit_desc;

	BUILDING_TYPE build_desc;

	Side side_desc;

	info_button* selected = nullptr;

public:
	UIHUDDescription(UICOMPONENT_TYPE type);

	bool Update();

	void SetEnableButton(UICheckbutton* btn);
	void SetDescription(info_button* if_btn);
	void SetLabelUnit();
	void SetLabelBuilding();

	void Clear();
};

#endif // __UIHUDDESCRIPTION_H__
