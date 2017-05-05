#ifndef _PANELINFO
#define _PANELINFO

#include "PanelInfoBasicStructure.h"
#include "UIButton.h"

class GroupSelection : public PanelInfo
{
private:
	struct entity_selected
	{
		UIButton* btn_selected = nullptr;

		Entity* pointer_entity = nullptr;
	};

	std::list<entity_selected*> es_selection;
	SDL_Rect life_bar;

public:
	GroupSelection(std::list<Entity*>& selection) : PanelInfo(selection) { type = PANEL_TYPE::GROUPSELECTION; };

	~GroupSelection();

	void Prepare();
	void PrepareUnitSelection();
	void PrepareNoUnitSelection();

	void Update();
	void Draw();

	void SetOneEntitySelection(Entity* e_selected);
};

#endif