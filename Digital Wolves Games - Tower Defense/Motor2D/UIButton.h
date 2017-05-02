#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#include "UIComponents.h"
#include "UILabel.h"
#include "Task.h"

#define UI_PANEL_MARK_MARGIN 2
struct info_button;

enum BUTTON_STAT
{
	BS_NONE,
	BS_CLICKED,
	BS_MOUSE_ON_TOP
};

class UIButton : public UIComponents
{
private:
	/*enum BUTTON_ATLAS_STAT
	{
		BA_UNSELECTED,
		BA_SELECTED,
		BA_CLICKED,
		BA_UNCLICKED
	};*/
	BUTTON_STAT state = BS_NONE;
	//BUTTON_ATLAS_STAT ba_stat = BA_UNSELECTED;

	SDL_Rect atlas_mouse_on_top = { 0,0,0,0 };
	SDL_Rect atlas_clicked = { 0,0,0,0 };

	Task* task = nullptr;
	
	bool is_ui_pannel = false;

public:
	UILabel* title;

public:
	UIButton(UICOMPONENT_TYPE type);
	~UIButton();

	void Set(const SDL_Rect& position, const SDL_Rect& atlas);

	void Draw();
	void HandleInput(SDL_Event);
	bool Update();

	const BUTTON_STAT GetStat() const;

	void SetMouseOnTopTextRect(const SDL_Rect & rect);
	void SetClickedTextRect(const SDL_Rect& rect);
	void SetTask(Task* task);
	void IsUiPanel(bool);
};

#endif // __UIBUTTON_H__