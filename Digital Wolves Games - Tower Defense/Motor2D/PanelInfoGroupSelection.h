#pragma once

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

	std::list<std::list<entity_selected*>::iterator*> to_delete;

	bool deletion = false;

public:
	GroupSelection(std::list<Entity*>& selection) : PanelInfo(selection) { type = PANEL_TYPE::GROUPSELECTION; };

	~GroupSelection();

	void Prepare();
	void PrepareUnitSelection();
	void PrepareNoUnitSelection();

	void Update();
	void DeleteESDeath();

	void Draw();

	void SetOneEntitySelection(Entity* e_selected);
};