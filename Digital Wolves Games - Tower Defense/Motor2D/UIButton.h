#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#include "UIComponents.h"
#include "UILabel.h"

struct info_button;

class UIButton : public UIComponents
{
public:
	bool clicked = false;
	UILabel* title;

	info_button* from_if_HUDPB;

public:
	UIButton(UICOMPONENT_TYPE type);

	void Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h);
	void Set(const SDL_Rect& position, const SDL_Rect& atlas);

	void Draw();

	bool Update();
};

#endif // __UIBUTTON_H__