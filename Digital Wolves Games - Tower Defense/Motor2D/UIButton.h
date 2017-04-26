#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#include "UIComponents.h"
#include "UILabel.h"

struct info_button;

enum BUTTON_STAT
{
	BS_NONE,
	BS_CLICKED,
	BS_UNCLICKED
};

class UIButton : public UIComponents
{
private:
	enum BUTTON_ATLAS_STAT
	{
		BA_UNSELECTED,
		BA_SELECTED,
		BA_CLICKED,
		BA_UNCLICKED
	};
	BUTTON_STAT bs_stat = BS_NONE;
	BUTTON_ATLAS_STAT ba_stat = BA_UNSELECTED;

	SDL_Rect atlas_selected = { 0,0,0,0 };
	SDL_Rect atlas_clicked = { 0,0,0,0 };
	SDL_Rect atlas_unclicked = { 0,0,0,0 };

	SDL_Rect pos_selected = { 0,0,0,0 };
	SDL_Rect pos_clicked = { 0,0,0,0 };
	SDL_Rect pos_unclicked = { 0,0,0,0 };

public:
	UILabel* title;

public:
	UIButton(UICOMPONENT_TYPE type);

	void Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h);
	void Set(const SDL_Rect& position, const SDL_Rect& atlas);

	void Draw();
	void HandleInput(SDL_Event);
	bool Update();

	const BUTTON_STAT GetStat() const;

	void SetAtlasSelected(const SDL_Rect& a_selected, const SDL_Rect& p_selected = {0,0,0,0});
	void SetAtlasClicked(const SDL_Rect& a_clicked, const SDL_Rect& p_clicked = { 0,0,0,0 });
	void SetAtlasUnclicked(const SDL_Rect& a_unclicked, const SDL_Rect& p_unclicked = { 0,0,0,0 });
};

#endif // __UIBUTTON_H__