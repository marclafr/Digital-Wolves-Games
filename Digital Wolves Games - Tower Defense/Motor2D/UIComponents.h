#pragma once
#ifndef __UICOMPONENTS_H__
#define ___UICOMPONENTS_H__

#include "p2Defs.h"
#include "j1App.h"
#include "j1UIManager.h"
#include "SDL\include\SDL.h"

enum UICOMPONENT_STAT
{
	SELECTED,
	UNSELECTED,

	CLICKL_DOWN,
	CLICKR_DOWN,

	CLICKL_REPEAT,
	CLICKR_REPEAT,

	CLICKL_UP,
	CLICKR_UP
};

enum UICOMPONENT_TYPE
{
	UILABEL,
	UIBUTTON,
	UICHECKBUTTON,
	UISELECTOPTION,
	UIHUDPANELBUTTONS,
	UIHUDPANELINFO,
	UIIMAGE
};

class UIComponents
{
public:
	SDL_Rect rect_position;
	SDL_Rect rect_atlas;

	UICOMPONENT_TYPE type;

	UICOMPONENT_STAT stat = UICOMPONENT_STAT::UNSELECTED;


private:
	bool draw = true;

	bool interactive = true;

	UIComponents* from = nullptr;

public:
	UIComponents(UICOMPONENT_TYPE type);

	virtual ~UIComponents();

	virtual void Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h);
	virtual void Set(const SDL_Rect& position, const SDL_Rect& atlas);

	virtual void Draw();

	virtual bool Update();

	const bool GetDraw() const;
	const bool GetInteractive() const;
	const UICOMPONENT_STAT GetStat() const;
	const UIComponents* GetFrom()const;
	const UIComponents* GetThisComponent() const;
	void SetDraw(bool draw);
	void SetInteractive(bool interactive);
	void SetFrom(UIComponents* from);
	
};

#endif // __UICOMPONENTS_H__