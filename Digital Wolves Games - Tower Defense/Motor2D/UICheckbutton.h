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

	void Set(const SDL_Rect& position, const SDL_Rect& atlas, const SDL_Rect & atlas_clicked);

	void Draw();
	bool Update();
};

#endif // __UICHECKBUTTION_H__