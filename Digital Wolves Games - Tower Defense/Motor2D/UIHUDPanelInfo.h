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

		Entity* entity_selected = nullptr;

		~entity_info();
	};

	std::list<Entity*> selection;

	std::list<UIButton*>  entities_btn;

	entity_info* entity_selected;

	PANEL_TYPE status = NONE;

public:
	UIHUDPanelInfo(UICOMPONENT_TYPE type);
	~UIHUDPanelInfo();

	void AddEntitySelection(Entity* selected);
	void DeleteSelection();

	void CreatePanel();
	void DeleteButtons();
	bool isSelectionEmpty();

	void Draw();

	SDL_Rect GetUnitIconPositionFromAtlas(const UNIT_TYPE type);

	const char* GetUnitName(const UNIT_TYPE type);
};

#endif // __UIHUDPANELINFO_H__