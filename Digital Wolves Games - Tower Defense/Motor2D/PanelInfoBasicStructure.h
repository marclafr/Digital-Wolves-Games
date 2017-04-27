#pragma once

#include "Entity.h"
#include "UIComponents.h"
#include <list>

enum PANEL_TYPE
{
	GROUPSELECTION,
	ONESELECTION,
	NONE
};

class PanelInfo
{
public:
	PANEL_TYPE type = NONE;

protected:
	std::list<Entity*>* selection;

	std::list<UIComponents*>::iterator first_component;
	std::list<UIComponents*>::iterator last_component;

	bool delete_panel_info = false;

	Entity* e_oneselected = nullptr;

public:
	PanelInfo(std::list<Entity*>& selection) { this->selection = &selection; };

	virtual ~PanelInfo() {};

	virtual void Prepare() {};

	virtual void Update() {};
	virtual void Draw() {};

	bool if_ToDelete() { return delete_panel_info; };

	Entity* GetEntityForOneSelected() { return e_oneselected; };

	void ClearOneSelected() { e_oneselected = nullptr; };
};