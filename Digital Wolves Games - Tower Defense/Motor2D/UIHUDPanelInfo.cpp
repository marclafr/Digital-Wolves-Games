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

#include "Units.h"
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
	selection.push_back(selected);
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
				Unit* selected = (Unit*)item._Ptr->_Myval;

				UIButton* new_btn = new UIButton(UICOMPONENT_TYPE::UIBUTTON);

				new_btn->SetFrom(this);

				new_btn->Set({ 220 + (29 * count++), 666, 29, 29 }, GetUnitIconPositionFromAtlas(selected->GetUnitType()));

				entities_btn.push_back(new_btn);

				item++;
			}
		}
		else
		{
			status = ENTITYINFO;

			entity_selected = new entity_info();

			std::list<Entity*>::iterator item;
			item = selection.begin();
			Unit* selected = (Unit*)item._Ptr->_Myval;

			entity_selected->image = new UIComponents(UICOMPONENT_TYPE::UIIMAGE);
			entity_selected->image->Set({ 231, 667, 29, 33 }, GetUnitIconPositionFromAtlas(selected->GetUnitType()));
			entity_selected->image->SetInteractive(false);

			entity_selected->name = new UILabel(UICOMPONENT_TYPE::UILABEL);
			entity_selected->name->Set(231, 653, GetUnitName(selected->GetUnitType()));

			std::string stats = std::to_string(selected->GetHp());
			entity_selected->life = new UILabel(UICOMPONENT_TYPE::UILABEL);
			entity_selected->life->Set(262, 690, stats.c_str());

			stats = std::to_string(selected->GetAttack());
			entity_selected->damage = new UILabel(UICOMPONENT_TYPE::UILABEL);
			entity_selected->damage->Set(272, 705, stats.c_str());

			stats = std::to_string(selected->GetArmor());
			entity_selected->armor = new UILabel(UICOMPONENT_TYPE::UILABEL);
			entity_selected->armor->Set(272, 722, stats.c_str());

			if (selected->GetUnitClass() == UNIT_CLASS::ARCHER)
			{
				stats = std::to_string(selected->GetRange());
				entity_selected->range = new UILabel(UICOMPONENT_TYPE::UILABEL);
				entity_selected->range->Set(272, 745, stats.c_str());
			}
		}
	}
	else
		status = NONE;
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

bool UIHUDPanelInfo::isSelectionEmpty()
{
	return selection.empty();
}

void UIHUDPanelInfo::Draw()
{
	if (!isSelectionEmpty())
	{
		std::list<UIButton*>::iterator item;

		switch (status)
		{
		case ENTITIESSELECTED:

			item = entities_btn.begin();

			while (item != entities_btn.end())
			{
				UIButton* uibutton = item._Ptr->_Myval;

				SDL_Rect mark_btn{ 999, 827, 29, 29 };
				App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), uibutton->rect_position.x + 2 - App->render->camera->GetPosition().x, uibutton->rect_position.y + 2 - App->render->camera->GetPosition().y, &uibutton->rect_atlas, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);
				App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), uibutton->rect_position.x - App->render->camera->GetPosition().x, uibutton->rect_position.y - App->render->camera->GetPosition().y, &mark_btn, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);

				item++;
			}

			break;

		case ENTITYINFO:
			UIComponents* entity_image = entity_selected->image;

			SDL_Rect mark_btn{ 1029, 827, 29, 33 };
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->rect_position.x + 2 - App->render->camera->GetPosition().x, entity_image->rect_position.y + 2 - App->render->camera->GetPosition().y, &entity_image->rect_atlas, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->rect_position.x - App->render->camera->GetPosition().x, entity_image->rect_position.y - App->render->camera->GetPosition().y, &mark_btn, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);

			UILabel* entity_label = entity_selected->name;
			App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera->GetPosition().x, entity_label->rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);

			entity_label = entity_selected->life;
			App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera->GetPosition().x, entity_label->rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);

			SDL_Rect mark_attack{ 956, 858, 38, 22 };
			entity_label = entity_selected->damage;
			App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera->GetPosition().x, entity_label->rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), 230 - App->render->camera->GetPosition().x, 703 - App->render->camera->GetPosition().y, &mark_attack, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);

			SDL_Rect mark_armor{ 956, 902, 37, 19 };
			entity_label = entity_selected->armor;
			App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera->GetPosition().x, entity_label->rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), 230 - App->render->camera->GetPosition().x, 725 - App->render->camera->GetPosition().y, &mark_armor, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);

			entity_label = entity_selected->range;
			if (entity_label != nullptr)
			{
				SDL_Rect mark_range{ 956, 881, 35, 20 };
				App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera->GetPosition().x, entity_label->rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);
				App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), 231 - App->render->camera->GetPosition().x, 744 - App->render->camera->GetPosition().y, &mark_range, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);
			}
			break;
		}
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
			break;
		case 
			NONE:
			break;
		}
	}


	return true;
}

SDL_Rect UIHUDPanelInfo::GetUnitIconPositionFromAtlas(const UNIT_TYPE type)
{
	SDL_Rect ret;

	switch (type)
	{
	case TWOHANDEDSWORDMAN:
		ret = { 774, 962, 25, 25 };
		break;

	case CAVALRYARCHER:
		ret = { 800, 962, 25, 25 };
		break;

	case SIEGERAM:
		ret = { 748, 962, 25, 25 };
		break;

	default:
		LOG("Error UNIT TYPE SDL_Rect NULL (UIManager)");
		ret = { 0, 0, 0, 0 };
		break;
	}

	return ret;
}

const char * UIHUDPanelInfo::GetUnitName(const UNIT_TYPE type)
{
	char* ret;

	switch (type)
	{
	case TWOHANDEDSWORDMAN:
		ret = "Two Handed Sword Man";
		break;

	case CAVALRYARCHER:
		ret = "Cavalry Archer";
		break;

	case SIEGERAM:
		ret = "Siege Ram";
		break;

	default:
		LOG("Error UNIT TYPE NAME NULL (UIManager)");
		ret = "Error";
		break;
	}

	return ret;
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
