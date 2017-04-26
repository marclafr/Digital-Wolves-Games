#pragma once
#ifndef __UICOMPONENTS_H__
#define ___UICOMPONENTS_H__

#include "p2Defs.h"
#include "j1App.h"
#include "j1UIManager.h"
#include "SDL\include\SDL.h"

#define RIGHT_CLICK 3
#define LEFT_CLICK 1

enum UICOMPONENT_TYPE
{
	UILABEL,
	UIBUTTON,
	UICHECKBUTTON,
	UISELECTOPTION,
	UIHUDPANELBUTTONS,
	UIHUDPANELINFO,
	UIIMAGE,
	UIHUDDESCRIPTION,
	UIHUDRESOURCES,
	UIHUDTOWNHALLBARLIFE,
	UIHUDSCOREBAR
};

class UIComponents
{
protected:
	bool interactive = true;
	bool is_focus = false;
	bool to_delete = false;
	UIComponents* from = nullptr;
	SDL_Rect rect_position;
	SDL_Rect rect_atlas;
	UICOMPONENT_TYPE type;

public:
	UIComponents(UICOMPONENT_TYPE type);

	virtual ~UIComponents();

	virtual void Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h);
	virtual void Set(const SDL_Rect& position, const SDL_Rect& atlas);

	virtual void Draw();
	virtual void HandleInput(SDL_Event);
	virtual bool Update();

	const bool GetInteractive() const;
	const bool IsFocus() const;
	const SDL_Rect GetAtlasRect() const;
	const SDL_Rect GetPosRect() const;
	const bool ToDelete() const;
	const UICOMPONENT_TYPE GetType() const;
	const UIComponents* GetFrom()const;
	const UIComponents* GetThisComponent() const;
	void SetInteractive(bool interactive);
	void SetIsFocus(bool is_focus);
	void SetFrom(UIComponents* from);
	void SetToDelete(bool to_delete);
	
};

#endif // __UICOMPONENTS_H__