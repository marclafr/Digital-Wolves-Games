#ifndef __UIHUDPANELINFO_H__
#define __UIHUDPANELINFO_H__

#include "PanelInfoBasicStructure.h"
#include "UIComponents.h"

#include <list>

class Entity;

class UIHUDPanelInfo : public UIComponents
{
private:
	//Actual panel info
	PanelInfo* actual_panelinfo = nullptr;

	//list of Entity_manager
	std::list<Entity*> selection_tmp;

	//Final list after Defione selection
	std::list<Entity*> selection;
	bool unit_selection = false;

	//Bool of entity selected if erased
	bool entity_selected_erased = false;

public:
	UIHUDPanelInfo(UICOMPONENT_TYPE type);
	~UIHUDPanelInfo();

	//Main Functions of list fill o remove
	void AddEntitySelection(Entity* selected);
	void DefineSelection();
	void DeleteSelection();

	//Prepare the Panel Info structure
	void CreatePanel();
	bool isSelectionTempEmpty();
	bool isSelectionEmpty();

	void Draw();
	
	bool Update();
};

#endif // __UIHUDPANELINFO_H__