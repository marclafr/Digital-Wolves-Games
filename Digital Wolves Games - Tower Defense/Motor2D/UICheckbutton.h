#ifndef __UICHECKBUTTION_H__
#define __UICHECKBUTTION_H__

#include "UIComponents.h"
#include "UIButton.h"

class UICheckbutton : public UIButton
{
public:
	SDL_Rect rect_atlas_clicked;

public:
	UICheckbutton(UICOMPONENT_TYPE type);

	void Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h, uint atlas_clicked_x, uint atlas_clicked_y, uint atlas_clicked_w, uint atlas_clicked_h);
	void Set(const SDL_Rect& position, const SDL_Rect& atlas, const SDL_Rect & atlas_clicked);

	void Draw();
};

#endif // __UICHECKBUTTION_H__