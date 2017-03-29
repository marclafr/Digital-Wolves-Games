#include "UIHUDPanelInfo.h"

#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1UIManager.h"
#include "j1EntityManager.h"
#include "Camera.h"

#include "SDL\include\SDL_rect.h"

#include "UIButton.h"
#include "UILabel.h"

#include "UIGetEntitiesInfo.h"

UIHUDPanelInfo::UIHUDPanelInfo(UICOMPONENT_TYPE type) : UIComponents(type)
{
	SetInteractive(false);
}

UIHUDPanelInfo::~UIHUDPanelInfo()
{
	if (!selection.empty())
	{
		selection.clear();

		DeleteButtons();
	}
}

void UIHUDPanelInfo::AddEntitySelection(Entity* selected)
{
	selection_tmp.push_back(selected);
}

void UIHUDPanelInfo::DefineSelection()
{
	std::list<Entity*>::iterator item;
	item = selection_tmp.begin();

	while (item != selection_tmp.end())
	{
		if (item._Ptr->_Myval->GetEntityType() == UNIT)
		{
			unit_selection = true;
			break;
		}
		item++;
	}

	if (unit_selection)
	{
		item = selection_tmp.begin();

		while (item != selection_tmp.end())
		{
			if (item._Ptr->_Myval->GetEntityType() == UNIT)
				selection.push_back(item._Ptr->_Myval);

			item++;
		}
	}
	else
		selection = selection_tmp;

	selection_tmp.clear();

	CreatePanel();
}

void UIHUDPanelInfo::DeleteSelection()
{
	selection.clear();

	DeleteButtons();
}

void UIHUDPanelInfo::CreatePanel()
{
	if (selection.empty() == false)
	{
		if (selection.size() > 1)
		{
			status = ENTITIESSELECTED;

			std::list<Entity*>::iterator item;
			item = selection.begin();

			int count = 0;
			while (item != selection.end())
			{

				if (unit_selection)
				{

					Unit* selected = (Unit*)item._Ptr->_Myval;

					UIButton* new_btn = new UIButton(UICOMPONENT_TYPE::UIBUTTON);

					new_btn->SetFrom(this);

					new_btn->Set({ 220 + (29 * count++), 666, 29, 29 }, GetUnitIconPositionFromAtlas(selected->GetUnitType()));

					entities_btn.push_back(new_btn);
				}
				else
				{
					Building* selected = (Building*)item._Ptr->_Myval;

					UIButton* new_btn = new UIButton(UICOMPONENT_TYPE::UIBUTTON);

					new_btn->SetFrom(this);

					new_btn->Set({ 220 + (29 * count++), 666, 29, 29 }, GetBuildingIconPositionFromAtlas(selected->GetBuildingType()));

					entities_btn.push_back(new_btn);
				}

				item++;
			}
		}
		else
		{
			status = ENTITYINFO;

			entity_selected = new entity_info();

			std::list<Entity*>::iterator item;
			item = selection.begin();

			entity_selected->pointer_entity = item._Ptr->_Myval;

			switch (entity_selected->pointer_entity->GetEntityType())
			{
			case UNIT:
				entity_selected->PrepareUnitInfo();
				break;
			case BUILDING:
				entity_selected->PrepareBuildingInfo();
				break;
			}
		}
	}
	else
		status = NONE;

	unit_selection = false;
}

void UIHUDPanelInfo::DeleteButtons()
{
	std::list<UIButton*>::iterator item;

	switch (status)
	{
	case ENTITIESSELECTED:
		
		item = entities_btn.begin();

		while (item != entities_btn.end())
		{
			delete item._Ptr->_Myval;

			item++;
		}

		entities_btn.clear();
		break;

	case ENTITYINFO:
		delete entity_selected;
		break;

	case NONE:
		break;

	}

}

bool UIHUDPanelInfo::isSelectionTempEmpty()
{
	return selection_tmp.empty();
}

bool UIHUDPanelInfo::isSelectionEmpty()
{
	return selection.empty();
}

void UIHUDPanelInfo::Draw()
{
	if (!isSelectionEmpty())
	{
		switch (status)
		{
		case ENTITIESSELECTED:
			DrawButtonsEntitiesSelected();
			break;

		case ENTITYINFO:
			switch (entity_selected->pointer_entity->GetEntityType())
			{
			case UNIT:
				DrawUnitSelected();
				break;

			case BUILDING:
				DrawBuildingSelected();
				break;
			}
			break;
		}
	}
}

void UIHUDPanelInfo::DrawButtonsEntitiesSelected()
{
	std::list<UIButton*>::iterator item;

	item = entities_btn.begin();

	while (item != entities_btn.end())
	{
		UIButton* uibutton = item._Ptr->_Myval;

		SDL_Rect mark_btn{ 999, 827, 29, 29 };
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), uibutton->rect_position.x + 2 - App->render->camera->GetPosition().x, uibutton->rect_position.y + 2 - App->render->camera->GetPosition().y, &uibutton->rect_atlas, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), uibutton->rect_position.x - App->render->camera->GetPosition().x, uibutton->rect_position.y - App->render->camera->GetPosition().y, &mark_btn, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);

		item++;
	}
}

void UIHUDPanelInfo::DrawUnitSelected()
{
	UIComponents* entity_image = entity_selected->image;

	SDL_Rect mark_btn{ 1029, 827, 29, 33 };
	App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->rect_position.x + 2 - App->render->camera->GetPosition().x, entity_image->rect_position.y + 2 - App->render->camera->GetPosition().y, &entity_image->rect_atlas, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
	App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->rect_position.x - App->render->camera->GetPosition().x, entity_image->rect_position.y - App->render->camera->GetPosition().y, &mark_btn, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);

	UILabel* entity_label = entity_selected->name;
	App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera->GetPosition().x, entity_label->rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);

	entity_label = entity_selected->life;
	App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera->GetPosition().x, entity_label->rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);

	SDL_Rect mark_attack{ 956, 858, 38, 22 };
	entity_label = entity_selected->damage;
	App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera->GetPosition().x, entity_label->rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
	App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), 230 - App->render->camera->GetPosition().x, 703 - App->render->camera->GetPosition().y, &mark_attack, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);

	SDL_Rect mark_armor{ 956, 902, 37, 19 };
	entity_label = entity_selected->armor;
	App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera->GetPosition().x, entity_label->rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
	App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), 230 - App->render->camera->GetPosition().x, 725 - App->render->camera->GetPosition().y, &mark_armor, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);

	entity_label = entity_selected->range;
	if (entity_label != nullptr)
	{
		SDL_Rect mark_range{ 956, 881, 35, 20 };
		App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera->GetPosition().x, entity_label->rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), 231 - App->render->camera->GetPosition().x, 744 - App->render->camera->GetPosition().y, &mark_range, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
	}
}

void UIHUDPanelInfo::DrawBuildingSelected()
{
	UIComponents* entity_image = entity_selected->image;

	SDL_Rect mark_btn{ 1029, 827, 29, 33 };
	App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->rect_position.x + 2 - App->render->camera->GetPosition().x, entity_image->rect_position.y + 2 - App->render->camera->GetPosition().y, &entity_image->rect_atlas, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
	App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->rect_position.x - App->render->camera->GetPosition().x, entity_image->rect_position.y - App->render->camera->GetPosition().y, &mark_btn, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);

	UILabel* entity_label = entity_selected->name;
	App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera->GetPosition().x, entity_label->rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);

	entity_label = entity_selected->life;
	App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera->GetPosition().x, entity_label->rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);

	entity_label = entity_selected->range;
	if (entity_label != nullptr)
	{
		SDL_Rect mark_range{ 956, 881, 35, 20 };
		App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera->GetPosition().x, entity_label->rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), 231 - App->render->camera->GetPosition().x, 711 - App->render->camera->GetPosition().y, &mark_range, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
	}
}

bool UIHUDPanelInfo::Update()
{
	if (!isSelectionEmpty())
	{
		switch (status)
		{
		case ENTITIESSELECTED:
			break;
		case ENTITYINFO:
			UpdateHP();
			break;
		case NONE:
			break;
		}
	}

	return true;
}

void UIHUDPanelInfo::UpdateHP()
{
	std::string hp = std::to_string(entity_selected->pointer_entity->GetHp());
	if (!entity_selected->life->text.compare(hp))
		entity_selected->life->ChangeText(hp.c_str());
}

UIHUDPanelInfo::entity_info::~entity_info()
{
	delete image;
	delete name;
	delete life;
	delete damage;
	delete armor;
	delete range;
}

void UIHUDPanelInfo::entity_info::PrepareUnitInfo()
{
	Unit* selected = (Unit*)this->pointer_entity;

	image = new UIComponents(UICOMPONENT_TYPE::UIIMAGE);
	image->Set({ 231, 667, 29, 33 }, GetUnitIconPositionFromAtlas(selected->GetUnitType()));
	image->SetInteractive(false);

	name = new UILabel(UICOMPONENT_TYPE::UILABEL);
	name->Set(231, 653, GetUnitName(selected->GetUnitType()));

	std::string stats = std::to_string(selected->GetHp());
	life = new UILabel(UICOMPONENT_TYPE::UILABEL);
	life->Set(262, 690, stats.c_str());

	stats = std::to_string(selected->GetAttack());
	damage = new UILabel(UICOMPONENT_TYPE::UILABEL);
	damage->Set(272, 705, stats.c_str());

	stats = std::to_string(selected->GetArmor());
	armor = new UILabel(UICOMPONENT_TYPE::UILABEL);
	armor->Set(272, 722, stats.c_str());

	if (selected->GetUnitClass() == UNIT_CLASS::ARCHER)
	{
		stats = std::to_string(selected->GetRange());
		range = new UILabel(UICOMPONENT_TYPE::UILABEL);
		range->Set(272, 745, stats.c_str());
	}
}

void UIHUDPanelInfo::entity_info::PrepareBuildingInfo()
{
	Building* selected = (Building*)this->pointer_entity;

	BUILDING_TYPE b_type_selected = selected->GetBuildingType();

	image = new UIComponents(UICOMPONENT_TYPE::UIIMAGE);
	image->Set({ 231, 667, 29, 33 }, GetBuildingIconPositionFromAtlas(b_type_selected));
	image->SetInteractive(false);

	name = new UILabel(UICOMPONENT_TYPE::UILABEL);
	name->Set(231, 653, GetBuildingName(b_type_selected));

	std::string stats = std::to_string(selected->GetHp());
	life = new UILabel(UICOMPONENT_TYPE::UILABEL);
	life->Set(262, 690, stats.c_str());

	if (selected->GetBuildingType() == b_type_selected)
	{
		stats = std::to_string(selected->GetRange());
		range = new UILabel(UICOMPONENT_TYPE::UILABEL);
		range->Set(272, 710, stats.c_str());
	}

}