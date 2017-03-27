#ifndef __UIHUDPANELINFO_H__
#define __UIHUDPANELINFO_H__

#include "UIComponents.h"

#include "UILabel.h"
#include "UIButton.h"
#include <list>

class Entity;

enum UNIT_TYPE;

class UIHUDPanelInfo : public UIComponents
{
private:
	enum PANEL_TYPE
	{
		ENTITIESSELECTED,
		ENTITYINFO,
		NONE
	};

	struct entity_info
	{
		UIComponents* image = nullptr;

		UILabel* name = nullptr;

		UILabel* life = nullptr;
		UILabel* damage = nullptr;
		UILabel* armor = nullptr;
		UILabel* range = nullptr;

		Entity* pointer_entity = nullptr;

		~entity_info();

		void PrepareUnitInfo();
		void PrepareBuildingInfo();
	};

	std::list<Entity*> selection_tmp;
	std::list<Entity*> selection;
	bool unit_selection = false;

	std::list<UIButton*>  entities_btn;

	entity_info* entity_selected;

	PANEL_TYPE status = NONE;

public:
	UIHUDPanelInfo(UICOMPONENT_TYPE type);
	~UIHUDPanelInfo();

	void AddEntitySelection(Entity* selected);
	void DefineSelection();
	void DeleteSelection();

	void CreatePanel();
	void DeleteButtons();
	bool isSelectionTempEmpty();
	bool isSelectionEmpty();


	void Draw();
	void DrawButtonsEntitiesSelected();
	void DrawUnitSelected();
	void DrawBuildingSelected();
	
	bool Update();
};

#endif // __UIHUDPANELINFO_H__