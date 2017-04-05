#define BLACK {0,0,0,0}

#define BAR_LIFE_CENTER 16

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
	selection.clear();

	std::list<Entity*>::iterator item;
	item = selection_tmp.begin();

	while (item != selection_tmp.end())
	{
		if (item._Ptr->_Myval->GetEntityType() == E_UNIT && (*item)->GetSide() != S_ENEMY && (*item)->GetHp() > 0 && unit_selection == true)
		{
			selection.push_back(*item);
			(*item)->SetEntityStatus(ST_SELECTED);
		}

		if (item._Ptr->_Myval->GetEntityType() == E_UNIT && (*item)->GetSide() != S_ENEMY && (*item)->GetHp() > 0 && unit_selection == false)
		{
			selection.clear();

			selection.push_back(*item);
			(*item)->SetEntityStatus(ST_SELECTED);

			unit_selection = true;
		}

		if ((item._Ptr->_Myval->GetEntityType() == E_BUILDING || item._Ptr->_Myval->GetEntityType() == E_RESOURCE) && (*item)->GetSide() != S_ENEMY && (*item)->GetHp() > 0 && unit_selection == false)
		{
			selection.push_back(*item);
			(*item)->SetEntityStatus(ST_SELECTED);
		}

		if (selection.size() >= 25)
			break;

		item++;
	}

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

			while (item != selection.end())
			{
				if (unit_selection)
				{
					entity_group_selection* add_entity_selected = new entity_group_selection();

					Unit* selected = (Unit*)item._Ptr->_Myval;

					UIButton* new_btn = new UIButton(UICOMPONENT_TYPE::UIBUTTON);

					new_btn->SetFrom(this);

					new_btn->Set({ 220, 666, 29, 29 }, GetUnitIconPositionFromAtlas(selected->GetUnitType()));

					add_entity_selected->btn_selected = new_btn;

					add_entity_selected->pointer_entity = (Entity*)selected;

					entities_btn.push_back(add_entity_selected);
				}
				else
				{
					entity_group_selection* add_entity_selected = new entity_group_selection();
					Building* b_selected = nullptr;
					Resources* r_selected = nullptr;

					UIButton* new_btn = new UIButton(UICOMPONENT_TYPE::UIBUTTON);

					new_btn->SetFrom(this);

					switch (item._Ptr->_Myval->GetEntityType())
					{
					case E_BUILDING:
						b_selected = (Building*)item._Ptr->_Myval;
						new_btn->Set({ 220, 666, 29, 29 }, GetBuildingIconPositionFromAtlas(b_selected->GetBuildingType()));
						break;

					case E_RESOURCE:
						r_selected = (Resources*)item._Ptr->_Myval;
						new_btn->Set({ 220, 666, 29, 29 }, GetResourceIconPositionFromAtlas(r_selected->GetResourceType()));
						break;
					}

					add_entity_selected->btn_selected = new_btn;

					add_entity_selected->pointer_entity = item._Ptr->_Myval;

					entities_btn.push_back(add_entity_selected);
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
			case E_UNIT:
				entity_selected->PrepareUnitInfo();
				break;
			case E_BUILDING:
				entity_selected->PrepareBuildingInfo();
				break;
			case E_RESOURCE:
				entity_selected->PrepareResourceInfo();
				break;
			}

			entity_selected_erased = false;
		}
	}
	else
		status = NONE;

	unit_selection = false;
}

void UIHUDPanelInfo::DeleteButtons()
{
	std::list<entity_group_selection*>::iterator item;

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
		if (entity_selected_erased == false)
		{
			delete entity_selected;
			entity_selected_erased = true;
		}
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
			case E_UNIT:
				DrawUnitSelected();
				break;

			case E_BUILDING:
				DrawBuildingSelected();
				break;

			case E_RESOURCE:
				DrawResourceSelected();
				break;
			}
			break;
		}
	}
}

void UIHUDPanelInfo::DrawButtonsEntitiesSelected()
{
	std::list<entity_group_selection*>::iterator item = entities_btn.begin();

	int count = 0;

	while (item != entities_btn.end())
	{
		if ((*item)->pointer_entity->GetHp() > 0)
		{
			UIButton* uibutton = item._Ptr->_Myval->btn_selected;

			uibutton->Set({ 220 + (29 * count++), 666, 29, 29 }, uibutton->rect_atlas);

			SDL_Rect mark_btn{ 999, 827, 29, 29 };
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), uibutton->rect_position.x + 2 - App->render->camera->GetPosition().x, uibutton->rect_position.y + 2 - App->render->camera->GetPosition().y, &uibutton->rect_atlas, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), uibutton->rect_position.x - App->render->camera->GetPosition().x, uibutton->rect_position.y - App->render->camera->GetPosition().y, &mark_btn, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);


			int rest_life_bar = 0;
			int height_correction = 0;
			ENTITY_TYPE e_type = item._Ptr->_Myval->pointer_entity->GetEntityType();
			Entity* e_selected = item._Ptr->_Myval->pointer_entity;
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

		item++;
	}
}

void UIHUDPanelInfo::DrawUnitSelected()
{
	Unit* unit_life_bar = (Unit*)entity_selected->pointer_entity;
	
	//Bar life
	if (unit_life_bar->GetHp() > 0)
	{
		int rest_life_bar = ReturnValueBarHPUnit(unit_life_bar->GetUnitType(), unit_life_bar->GetHp());
		int height_correction = ReturnValueHeightCorrectionUnit(unit_life_bar->GetUnitClass());
		SDL_Rect mark_life_bar_red{ 1059, 832, 32, 4 };
		SDL_Rect mark_life_bar_green{ 1059, 827, rest_life_bar, 4 };
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), unit_life_bar->GetX() - BAR_LIFE_CENTER, unit_life_bar->GetY() - height_correction, &mark_life_bar_red, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), unit_life_bar->GetX() - BAR_LIFE_CENTER, unit_life_bar->GetY() - height_correction, &mark_life_bar_green, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);

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
}

void UIHUDPanelInfo::DrawBuildingSelected()
{
	Building* building_life_bar = (Building*)entity_selected->pointer_entity;

	if (building_life_bar->GetHp() > 0)
	{
		//Bar life
		int rest_life_bar = ReturnValueBarHPBuilding(building_life_bar->GetBuildingType(), building_life_bar->GetHp());
		int height_correction = ReturnValueHeightCorrectionBuilding(building_life_bar->GetBuildingType());
		SDL_Rect mark_life_bar_red{ 1059, 832, 32, 4 };
		SDL_Rect mark_life_bar_green{ 1059, 827, rest_life_bar, 4 };
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), building_life_bar->GetX() - BAR_LIFE_CENTER, building_life_bar->GetY() - height_correction, &mark_life_bar_red, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), building_life_bar->GetX() - BAR_LIFE_CENTER, building_life_bar->GetY() - height_correction, &mark_life_bar_green, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);

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

		if (entity_selected->build)
		{
			uint percentage = GetBuildingPercentage(entity_selected->pointer_entity);
			if (percentage < 100)
			{
				entity_selected->UpdateBuildingPercentageStr();
				entity_label = entity_selected->building_percentage;
				App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera->GetPosition().x, entity_label->rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);

				entity_label = entity_selected->name;
				App->render->Blit(entity_label->text_img, 400 - App->render->camera->GetPosition().x, 716 - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);

				//Icon
				SDL_Rect mark_icon{ 339, 1162, 40, 40 };
				App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), 356 - App->render->camera->GetPosition().x, 701 - App->render->camera->GetPosition().y, &mark_icon, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);

				//Progress bar
				SDL_Rect mark_clear_bar{ 330, 1203, 100, 12 };
				SDL_Rect mark_last_bar{ 403, 1216, 1, 12 };
				SDL_Rect mark_full_bar{ 298, 1216, 2 + (int)percentage, 12 };
				App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), 400 - App->render->camera->GetPosition().x, 729 - App->render->camera->GetPosition().y, &mark_clear_bar, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
				App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), 399 - App->render->camera->GetPosition().x, 729 - App->render->camera->GetPosition().y, &mark_full_bar, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
				if (percentage < 98)
					App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), 401 + (int)percentage - App->render->camera->GetPosition().x, 729 - App->render->camera->GetPosition().y, &mark_last_bar, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
			}
			else
				entity_selected->build = false;
			
		}
	}
}

void UIHUDPanelInfo::DrawResourceSelected()
{
	Resources* resource_life_bar = (Resources*)entity_selected->pointer_entity;

	if (resource_life_bar->GetHp() > 0)
	{
		//Bar life
		int rest_life_bar = ReturnValueBarHPResource(resource_life_bar->GetResourceType(), resource_life_bar->GetHp());
		int height_correction = ReturnValueHeightCorrectionResource(resource_life_bar->GetResourceType());
		SDL_Rect mark_life_bar_red{ 1059, 832, 32, 4 };
		SDL_Rect mark_life_bar_green{ 1059, 827, rest_life_bar, 4 };
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), resource_life_bar->GetX() - BAR_LIFE_CENTER, resource_life_bar->GetY() - height_correction, &mark_life_bar_red, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), resource_life_bar->GetX() - BAR_LIFE_CENTER, resource_life_bar->GetY() - height_correction, &mark_life_bar_green, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);

		UIComponents* entity_image = entity_selected->image;

		SDL_Rect mark_btn{ 1029, 827, 29, 33 };
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->rect_position.x + 2 - App->render->camera->GetPosition().x, entity_image->rect_position.y + 2 - App->render->camera->GetPosition().y, &entity_image->rect_atlas, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->rect_position.x - App->render->camera->GetPosition().x, entity_image->rect_position.y - App->render->camera->GetPosition().y, &mark_btn, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);

		UILabel* entity_label = entity_selected->name;
		App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera->GetPosition().x, entity_label->rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);

		entity_label = entity_selected->life;
		App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera->GetPosition().x, entity_label->rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
	}
}

bool UIHUDPanelInfo::Update()
{
	if (!isSelectionEmpty())
	{
		std::list<entity_group_selection*>::iterator item = entities_btn.begin();

		switch (status)
		{
		case ENTITIESSELECTED:
			while (item != entities_btn.end())
			{
				if ((*item)->pointer_entity->GetEntityStatus() == ST_NON_SELECTED)
					entities_btn.erase(item);

				item++;
			}

			break;
		case ENTITYINFO:

			switch (entity_selected->pointer_entity->GetEntityStatus())
			{
			case ST_NON_SELECTED:
					DeleteButtons();
				break;

			default:
				UpdateHP();

				if (entity_selected->pointer_entity->GetEntityType() == E_BUILDING)
					if (entity_selected->build == false && isBuilded(entity_selected->pointer_entity))
					{
						entity_selected->build = true;
						entity_selected->PrepareBuildingConstruction();
					}
				break;
			}
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
	if (entity_selected->life->text.compare(hp))
		entity_selected->life->ChangeText(hp.c_str(), BLACK);
}

UIHUDPanelInfo::entity_info::~entity_info()
{
	if(image != nullptr)
		delete image;

	if (name != nullptr)
	delete name;

	if (life != nullptr)
	delete life;

	if (damage != nullptr)
	delete damage;

	if (armor != nullptr)
	delete armor;

	if (range != nullptr)
	delete range;

	if (build)
	{
		if (building_percentage != nullptr)
			delete building_percentage;
		build = false;
	}
}

void UIHUDPanelInfo::entity_info::PrepareUnitInfo()
{
	Unit* selected = (Unit*)this->pointer_entity;

	image = new UIComponents(UICOMPONENT_TYPE::UIIMAGE);
	image->Set({ 231, 667, 29, 33 }, GetUnitIconPositionFromAtlas(selected->GetUnitType()));
	image->SetInteractive(false);

	name = new UILabel(UICOMPONENT_TYPE::UILABEL);
	name->Set(231, 653, GetUnitName(selected->GetUnitType()), BLACK);

	std::string stats = std::to_string(selected->GetHp());
	life = new UILabel(UICOMPONENT_TYPE::UILABEL);
	life->Set(262, 690, stats.c_str(), BLACK);

	stats = std::to_string(selected->GetAttack());
	damage = new UILabel(UICOMPONENT_TYPE::UILABEL);
	damage->Set(272, 705, stats.c_str(), BLACK);

	stats = std::to_string(selected->GetArmor());
	armor = new UILabel(UICOMPONENT_TYPE::UILABEL);
	armor->Set(272, 722, stats.c_str(), BLACK);

	if (selected->GetUnitClass() == UNIT_CLASS::C_ARCHER)
	{
		stats = std::to_string(selected->GetRange());
		range = new UILabel(UICOMPONENT_TYPE::UILABEL);
		range->Set(272, 745, stats.c_str(), BLACK);
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
	name->Set(231, 653, GetBuildingName(b_type_selected), BLACK);

	std::string stats = std::to_string(selected->GetHp());
	life = new UILabel(UICOMPONENT_TYPE::UILABEL);
	life->Set(262, 690, stats.c_str(), BLACK);

	if (selected->GetBuildingType() == b_type_selected)
	{
		stats = std::to_string(selected->GetRange());
		range = new UILabel(UICOMPONENT_TYPE::UILABEL);
		range->Set(272, 710, stats.c_str(), BLACK);
	}
}

void UIHUDPanelInfo::entity_info::PrepareBuildingConstruction()
{
	building_percentage = new UILabel(UICOMPONENT_TYPE::UILABEL);
	uint percentage = GetBuildingPercentage(pointer_entity);
	std::string str_percentage("Constructing - ");
	str_percentage += std::to_string(percentage);
	str_percentage += "%";
	building_percentage->Set(400,703, str_percentage.c_str(), BLACK);
	building_percentage->SetInteractive(false);
}

void UIHUDPanelInfo::entity_info::UpdateBuildingPercentageStr()
{
	uint percentage = GetBuildingPercentage(pointer_entity);
	std::string str_percentage("Constructing - ");
	str_percentage += std::to_string(percentage);
	str_percentage += "%";

	building_percentage->ChangeText(str_percentage.c_str(), BLACK);
}

void UIHUDPanelInfo::entity_info::PrepareResourceInfo()
{
	Resources* selected = (Resources*)this->pointer_entity;

	RESOURCE_TYPE r_type_selected = selected->GetResourceType();

	image = new UIComponents(UICOMPONENT_TYPE::UIIMAGE);
	image->Set({ 231, 667, 29, 33 }, GetResourceIconPositionFromAtlas(r_type_selected));
	image->SetInteractive(false);

	name = new UILabel(UICOMPONENT_TYPE::UILABEL);
	name->Set(231, 653, GetResourceName(r_type_selected), BLACK);

	std::string stats = std::to_string(selected->GetHp());
	life = new UILabel(UICOMPONENT_TYPE::UILABEL);
	life->Set(262, 690, stats.c_str(), BLACK);
}

UIHUDPanelInfo::entity_group_selection::~entity_group_selection()
{
	delete btn_selected;
}
