#define MARK_BTN { 220, 666, 29, 29 }
#define BAR_LIFE_CENTER 16

#include "PanelInfoGroupSelection.h"

#include "j1App.h"
#include "j1UIManager.h"
#include "j1Render.h"
#include "Camera.h"

#include "UIGetEntitiesInfo.h"

GroupSelection::~GroupSelection()
{
	std::list<entity_selected*>::iterator es_item = es_selection.begin();

	while (es_item != es_selection.end())
	{
		delete *es_item;

		es_item++;
	}

	App->uimanager->erase_list(first_component, last_component);

	es_selection.clear();
}

void GroupSelection::Prepare()
{
	std::list<Entity*>::iterator check = selection->begin();

	if ((*check)->GetEntityType() == ENTITY_TYPE::E_UNIT)
		PrepareUnitSelection();
	else
		PrepareNoUnitSelection();
}

void GroupSelection::PrepareUnitSelection()
{
	std::list<Entity*>::iterator u_item = selection->begin();

	std::list<Entity*>::iterator last_u_item = selection->end();
	last_u_item--;

	while (u_item != selection->end())
	{
		entity_selected* add_entity_selected = new entity_selected();

		Unit* selected = (Unit*)*u_item;
		UIButton* new_btn = nullptr;

		new_btn = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);

		if (u_item == selection->begin())
			first_component = App->uimanager->GetLastComponent();
		else if (u_item == last_u_item)
			last_component = App->uimanager->GetLastComponent();

		new_btn->Set(MARK_BTN, GetUnitIconPositionFromAtlas(selected->GetUnitType()));

		add_entity_selected->btn_selected = new_btn;

		add_entity_selected->pointer_entity = (Entity*)selected;

		es_selection.push_back(add_entity_selected);

		u_item++;
	}
}

void GroupSelection::PrepareNoUnitSelection()
{
	std::list<Entity*>::iterator e_item = selection->begin();

	std::list<Entity*>::iterator last_e_item = selection->end();
	last_e_item--;

	while (e_item != selection->end())
	{
		entity_selected* add_entity_selected = new entity_selected();
		Building* b_selected = nullptr;
		Resources* r_selected = nullptr;

		UIButton* new_btn = nullptr;

		new_btn = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);

		if (e_item == selection->begin())
			first_component = App->uimanager->GetLastComponent();
		else if (e_item == last_e_item)
			last_component = App->uimanager->GetLastComponent();

		switch ((*e_item)->GetEntityType())
		{
		case E_BUILDING:
			b_selected = (Building*)*e_item;
			new_btn->Set(MARK_BTN, GetBuildingIconPositionFromAtlas(b_selected->GetBuildingType()));
			break;

		case E_RESOURCE:
			r_selected = (Resources*)*e_item;
			new_btn->Set(MARK_BTN, GetResourceIconPositionFromAtlas(r_selected->GetResourceType()));
			break;
		}

		add_entity_selected->btn_selected = new_btn;

		add_entity_selected->pointer_entity = *e_item;

		es_selection.push_back(add_entity_selected);

		e_item++;
	}
}

void GroupSelection::Update()
{
	std::list<entity_selected*>::iterator es_item = es_selection.begin();

	while (es_item != es_selection.end())
	{
		if ((*es_item)->btn_selected->stat == UICOMPONENT_STAT::CLICKL_UP)
			SetOneEntitySelection((*es_item)->pointer_entity);

		if ((*es_item)->pointer_entity->GetEntityStatus() == ST_NON_SELECTED)
		{
			to_delete.push_back(es_item);
			deletion = true;
		}

		es_item++;
	}

	if (deletion)
		DeleteESDeath();

	if (es_selection.empty())
		delete_panel_info = true;
}

void GroupSelection::DeleteESDeath()
{
	std::list<std::list<entity_selected*>::iterator>::iterator delete_item = to_delete.begin();

	while (delete_item != to_delete.end())
	{
		(*(*delete_item))->btn_selected->SetDraw(false);

		es_selection.erase(*delete_item);

		delete_item++;
	}

	to_delete.clear();
	deletion = false;
}

void GroupSelection::Draw()
{
	std::list<entity_selected*>::iterator es_item = es_selection.begin();

	int count = 0;

	while (es_item != es_selection.end())
	{
		if ((*es_item)->pointer_entity->GetHp() > 0)
		{
			UIButton* uibutton = (*es_item)->btn_selected;

			uibutton->Set({ 220 + (29 * count++), 666, 29, 29 }, uibutton->rect_atlas);

			SDL_Rect mark_btn{ 999, 827, 29, 29 };
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), uibutton->rect_position.x + 2 - App->render->camera->GetPosition().x, uibutton->rect_position.y + 2 - App->render->camera->GetPosition().y, &uibutton->rect_atlas, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), uibutton->rect_position.x - App->render->camera->GetPosition().x, uibutton->rect_position.y - App->render->camera->GetPosition().y, &mark_btn, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);


			int rest_life_bar = 0;
			int height_correction = 0;
			ENTITY_TYPE e_type = (*es_item)->pointer_entity->GetEntityType();
			Entity* e_selected = (*es_item)->pointer_entity;
			Building* b_selected = nullptr;
			Resources* r_selected = nullptr;
			Unit* u_selected = nullptr;
			switch (e_type)
			{
			case E_UNIT:
				u_selected = (Unit*)e_selected;
				rest_life_bar = ReturnValueBarHPUnit(u_selected->GetUnitType(), u_selected->GetHp());
				height_correction = ReturnValueHeightCorrectionUnit(u_selected->GetUnitClass());
				break;

			case E_BUILDING:
				b_selected = (Building*)e_selected;
				rest_life_bar = ReturnValueBarHPBuilding(b_selected->GetBuildingType(), b_selected->GetHp());
				height_correction = ReturnValueHeightCorrectionBuilding(b_selected->GetBuildingType());
				break;

			case E_RESOURCE:
				r_selected = (Resources*)e_selected;
				rest_life_bar = ReturnValueBarHPResource(r_selected->GetResourceType(), r_selected->GetHp());
				height_correction = ReturnValueHeightCorrectionResource(r_selected->GetResourceType());
				break;
			}

			//Bar life unit
			SDL_Rect mark_life_bar_red{ 1059, 832, 32, 4 };
			SDL_Rect mark_life_bar_green{ 1059, 827, rest_life_bar, 4 };
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), e_selected->GetX() - BAR_LIFE_CENTER, e_selected->GetY() - height_correction, &mark_life_bar_red, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), e_selected->GetX() - BAR_LIFE_CENTER, e_selected->GetY() - height_correction, &mark_life_bar_green, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);
		}

		es_item++;
	}
}

void GroupSelection::SetOneEntitySelection(Entity* e_selected)
{
	e_oneselected = e_selected;
}