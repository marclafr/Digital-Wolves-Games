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
	//Stat of Panel Info
	enum PANEL_TYPE
	{
		//Entityes selected
		ENTITIESSELECTED,
		//One selection
		ENTITYINFO,
		NONE
	};

	//Struct of 1 selection entity
	struct entity_info
	{
		UIComponents* image = nullptr;

		UILabel* name = nullptr;

		UILabel* life = nullptr;
		UILabel* damage = nullptr;
		UILabel* armor = nullptr;
		UILabel* range = nullptr;

		Entity* pointer_entity = nullptr;

		bool build = false;
		UILabel* building_percentage = nullptr;

		~entity_info();

		void PrepareUnitInfo();
		void PrepareBuildingInfo();
		void PrepareBuildingConstruction();
		void UpdateBuildingPercentageStr();
		void PrepareResourceInfo();
	};

	//Struct of every Button of entities selection
	struct entity_group_selection
	{
		UIButton* btn_selected = nullptr;

		Entity* pointer_entity = nullptr;

		~entity_group_selection();
	};

	//list of Entity_manager
	std::list<Entity*> selection_tmp;

	//Final list after Defione selection
	std::list<Entity*> selection;
	bool unit_selection = false;

	//List of various entities selected
	std::list<entity_group_selection*>  entities_btn;

	//Pointer when selecting one entity
	entity_info* entity_selected;

	//Status of Panel Info
	PANEL_TYPE status = NONE;

public:
	UIHUDPanelInfo(UICOMPONENT_TYPE type);
	~UIHUDPanelInfo();

	//Main Functions of list fill o remove
	void AddEntitySelection(Entity* selected);
	void DefineSelection();
	void DeleteSelection();

	//Prepare the Panel Info structure
	void CreatePanel();
	void DeleteButtons();
	bool isSelectionTempEmpty();
	bool isSelectionEmpty();

	//Draw functions
	void Draw();
	void DrawButtonsEntitiesSelected();
	void DrawUnitSelected();
	void DrawBuildingSelected();
	void DrawResourceSelected();
	
	bool Update();

	void UpdateHP();
};

#endif // __UIHUDPANELINFO_H__