#define RIGHT_CLICK 3
#define LEFT_CLICK 1

#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "Entity.h"
#include "Units.h"

#include "j1UIManager.h"

j1UIManager::j1UIManager() : j1Module()
{
	name.assign("uimanager");
}

// Destructor
j1UIManager::~j1UIManager()
{}

// Called before render is available
bool j1UIManager::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1UIManager::Start()
{
	atlas = App->tex->Load(atlas_file_name.c_str());

	return true;
}

// Update all guis

bool j1UIManager::PreUpdate()
{
	
	int x_mouse = 0;
	int y_mouse = 0;

	App->input->GetMousePosition(x_mouse, y_mouse);

	std::list<UIComponents*>::iterator item;
	item = components.begin();

	while (item != components.end())
	{

		UIComponents* component = item._Ptr->_Myval;

		if (component->draw && component->interactive)
		{
			if ((x_mouse > component->rect_position.x) && (x_mouse < component->rect_position.x + component->rect_position.w) && (y_mouse > component->rect_position.y) && (y_mouse < component->rect_position.y + component->rect_position.h))
			{
				component->stat = UIComponent_Stat::SELECTED;

				if (App->input->GetMouseButtonDown(LEFT_CLICK) == KEY_DOWN)
				{
					if (component->stat == UIComponent_Stat::SELECTED)
					{
						component->stat = UIComponent_Stat::CLICKL_DOWN;

						//Check or uncheck for UICheckbutton
						if (component->type == UIComponent_TYPE::UICHECKBUTTON)
						{
							UICheckbutton* button_checked = (UICheckbutton*)component;
							if (button_checked->clicked)
								button_checked->clicked = false;
							else
								button_checked->clicked = true;
						}

						//Expand options from UISelectOption
						if (component->type == UIComponent_TYPE::UISELECTOPTION)
						{
							UISelectOption* select_option = (UISelectOption*)component;
							select_option->selecting = true;
							select_option->ChangeDrawAllOptions();
						}

						//Selecting option from UISelectOption
						if (component->type == UIComponent_TYPE::UILABEL && (component->from != nullptr && component->from->type == UIComponent_TYPE::UISELECTOPTION))
						{
							UISelectOption* from_option_selected = (UISelectOption*)component->from;
							UILabel* option_selected = (UILabel*)component;

							if (option_selected != from_option_selected->current)
							{
								from_option_selected->ChangeCurrent(option_selected);
								from_option_selected->ChangeDrawAllOptions();
								from_option_selected->selecting = false;
							}
						}

						//Click of UIButton
						if (component->type == UIComponent_TYPE::UIBUTTON)
						{
							UIButton* button = (UIButton*)component;
							button->clicked = true;
						}
					}
					else
						component->stat = UIComponent_Stat::CLICKL_REPEAT;
				}
				else if (App->input->GetMouseButtonDown(LEFT_CLICK) == KEY_UP)
				{
					if (component->stat == UIComponent_Stat::CLICKL_REPEAT || component->stat == UIComponent_Stat::CLICKL_DOWN)
						component->stat = UIComponent_Stat::CLICKL_UP;
					else
						component->stat = UIComponent_Stat::SELECTED;

					//UIButton set click to false
					if (component->type == UIComponent_TYPE::UIBUTTON)
					{
						UIButton* button = (UIButton*)component;
						button->clicked = false;
					}
				}

				if (App->input->GetMouseButtonDown(RIGHT_CLICK) == KEY_DOWN)
				{
					if (component->stat == UIComponent_Stat::SELECTED)
						component->stat = UIComponent_Stat::CLICKR_DOWN;
					else
						component->stat = UIComponent_Stat::CLICKR_REPEAT;
				}
				else if (App->input->GetMouseButtonDown(RIGHT_CLICK) == KEY_UP)
				{
					if (component->stat == UIComponent_Stat::CLICKR_REPEAT || component->stat == UIComponent_Stat::CLICKR_DOWN)
						component->stat = UIComponent_Stat::CLICKR_UP;
					else
						component->stat = UIComponent_Stat::SELECTED;
				}
			}
			else
				component->stat = UIComponent_Stat::UNSELECTED;
		}
		item++;
	}
	return true;
}


// Called after all Updates
bool j1UIManager::PostUpdate()
{
	drawAllComponents();
	
	return true;
}

// Called before quitting
bool j1UIManager::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

UIComponents* j1UIManager::addUIComponent(UIComponent_TYPE type)
{
	UIComponents* ret = nullptr;
	
	if (type == UIComponent_TYPE::UIBUTTON)
	{
		components.push_back(ret = new UIButton(UIComponent_TYPE::UIBUTTON));
	}
	else if (type == UIComponent_TYPE::UIIMAGE)
	{
		components.push_back(ret = new UIComponents(UIComponent_TYPE::UIIMAGE));
	}
	else if(type == UIComponent_TYPE::UIINPUT)
	{
		components.push_back(ret = new UIInput(UIComponent_TYPE::UIINPUT));
	}
	else if (type == UIComponent_TYPE::UILABEL)
	{
		components.push_back(ret = new UILabel(UIComponent_TYPE::UILABEL));
	}
	else if(type == UIComponent_TYPE::UICHECKBUTTON)
	{
		components.push_back(ret = new UICheckbutton(UIComponent_TYPE::UICHECKBUTTON));
	}
	else if (type == UIComponent_TYPE::UISELECTOPTION)
	{
		components.push_back(ret = new UISelectOption(UIComponent_TYPE::UISELECTOPTION));
	}
	else if (type == UIComponent_TYPE::UIHUDPANELBUTTONS)
	{
		components.push_back(ret = new UIHUDPanelButtons(UIComponent_TYPE::UIHUDPANELBUTTONS));
	}
	else if (type == UIComponent_TYPE::UIHUDPANELINFO)
	{
		components.push_back(ret = new UIHUDPanelInfo(UIComponent_TYPE::UIHUDPANELINFO));
	}
	
	
	return ret;
}

// const getter for atlas
const SDL_Texture* j1UIManager::GetAtlas() const
{
	return atlas;
}

void j1UIManager::drawAllComponents()
{
	std::list<UIComponents*>::iterator item;
	item = components.begin();

	UIComponent_TYPE* type = nullptr;

	while (item != components.end())
	{
		UIComponents* component = item._Ptr->_Myval;
		
		if (component->draw)
		{
			type = &component->type;

			if (*type == UIComponent_TYPE::UIBUTTON)
			{
				UIComponents* uibutton = component;

				if (uibutton->from != nullptr)
				{
					if (uibutton->from->type == UIHUDPANELBUTTONS)
					{
						SDL_Rect mark_btn{ 969, 827, 29, 29 };
						App->render->Blit(atlas, uibutton->rect_position.x - 2 - App->render->camera.x, uibutton->rect_position.y - 2 - App->render->camera.y, &mark_btn);
						App->render->Blit(atlas, uibutton->rect_position.x - App->render->camera.x, uibutton->rect_position.y - App->render->camera.y, &uibutton->rect_atlas);
					}
				}
				else
					App->render->Blit(atlas, uibutton->rect_position.x - App->render->camera.x, uibutton->rect_position.y - App->render->camera.y, &uibutton->rect_atlas);
			}
			else if (*type == UIComponent_TYPE::UIIMAGE)
			{
				UIComponents* uiimage = component;

				App->render->Blit(atlas, uiimage->rect_position.x - App->render->camera.x, uiimage->rect_position.y - App->render->camera.y, &uiimage->rect_atlas);
			}
			else if (*type == UIComponent_TYPE::UIINPUT)
			{
				UIComponents* uiinput = component;

				App->render->Blit(atlas, uiinput->rect_position.x - App->render->camera.x, uiinput->rect_position.y - App->render->camera.y, &uiinput->rect_atlas);
			}
			else if (*type == UIComponent_TYPE::UILABEL)
			{
				UILabel* uilabel = (UILabel*)component;

				App->render->Blit(uilabel->text_img, uilabel->rect_position.x - App->render->camera.x, uilabel->rect_position.y - App->render->camera.y);
			}
			else if (*type == UIComponent_TYPE::UICHECKBUTTON)
			{
				UICheckbutton* uicheckbutton = (UICheckbutton*)component;

				if (uicheckbutton->clicked)
					App->render->Blit(atlas, uicheckbutton->rect_position.x - App->render->camera.x, uicheckbutton->rect_position.y - App->render->camera.y, &uicheckbutton->rect_atlas_clicked);
				else
					App->render->Blit(atlas, uicheckbutton->rect_position.x - App->render->camera.x, uicheckbutton->rect_position.y - App->render->camera.y, &uicheckbutton->rect_atlas);
			}
			else if (*type == UIComponent_TYPE::UISELECTOPTION)
			{
				UISelectOption* uiselectoption = (UISelectOption*)component;

				App->render->Blit(atlas, uiselectoption->rect_position.x - App->render->camera.x, uiselectoption->rect_position.y - App->render->camera.y, &uiselectoption->rect_atlas);

				if (uiselectoption->selecting)
				{
					for(uint i = 1; i < uiselectoption->num_options + 1; i++)
						App->render->Blit(atlas, uiselectoption->rect_position.x - App->render->camera.x, (uiselectoption->rect_position.y - App->render->camera.y) + uiselectoption->rect_atlas.h * i, &uiselectoption->rect_atlas);
				}
			}
			else if (*type == UIComponent_TYPE::UIHUDPANELINFO)
			{
				UIHUDPanelInfo* uihudpanelinfo = (UIHUDPanelInfo*)component;

				uihudpanelinfo->Draw();
			}
		}
		item++;
	}
}

// class uimanager ---------------------------------------------------

void UILabel::Set(int pos_x, int pos_y, const char * text, _TTF_Font*  font)
{
	rect_position.x = pos_x;
	rect_position.y = pos_y;

	text_img = App->font->Print(text, { 255,0,0,0 });

	this->text.assign(text);

	App->render->GetDimensionsFromTexture(text_img, rect_position.w, rect_position.h);
}

void UILabel::ChangeText(const char* text)
{
	if (this->text != text)
	{
		text_img = App->font->Print(text, { 255,0,0,0 });

		App->render->GetDimensionsFromTexture(text_img, rect_position.w, rect_position.h);

		this->text.erase();
		this->text.assign(text);
	}
}

UIButton::UIButton(UIComponent_TYPE type) : UIComponents(type)
{
	title = (UILabel*)App->uimanager->addUIComponent(UIComponent_TYPE::UILABEL);
}

void UIButton::Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h)
{
	rect_position.x = pos_x;
	rect_position.y = pos_y;
	rect_position.w = pos_w;
	rect_position.h = pos_h;

	rect_atlas.x = atlas_x;
	rect_atlas.y = atlas_y;
	rect_atlas.w = atlas_w;
	rect_atlas.h = atlas_h;
}

void UIButton::Set(const SDL_Rect & position, const SDL_Rect & atlas)
{
	rect_position = position;
	rect_atlas = atlas;
}

UICheckbutton::UICheckbutton(UIComponent_TYPE type) : UIButton(type) 
{

}

void UICheckbutton::Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h, uint atlas_clicked_x, uint atlas_clicked_y, uint atlas_clicked_w, uint atlas_clicked_h)
{
	rect_position.x = pos_x;
	rect_position.y = pos_y;
	rect_position.w = pos_w;
	rect_position.h = pos_h;

	rect_atlas.x = atlas_x;
	rect_atlas.y = atlas_y;
	rect_atlas.w = atlas_w;
	rect_atlas.h = atlas_h;

	rect_atlas_clicked.x = atlas_clicked_x;
	rect_atlas_clicked.y = atlas_clicked_y;
	rect_atlas_clicked.w = atlas_clicked_w;
	rect_atlas_clicked.h = atlas_clicked_h;
}

void UICheckbutton::Set(const SDL_Rect & position, const SDL_Rect & atlas, const SDL_Rect & atlas_clicked)
{
	rect_position = position;
	rect_atlas = atlas;
	rect_atlas_clicked = atlas_clicked;
}

UIInput::UIInput(UIComponent_TYPE type) : UIComponents(type)
{
	title = (UILabel*)App->uimanager->addUIComponent(UIComponent_TYPE::UILABEL);
}

void UIInput::Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h)
{
	rect_position.x = pos_x;
	rect_position.y = pos_y;
	rect_position.w = pos_w;
	rect_position.h = pos_h;

	rect_atlas.x = atlas_x;
	rect_atlas.y = atlas_y;
	rect_atlas.w = atlas_w;
	rect_atlas.h = atlas_h;
}

void UIInput::Set(const SDL_Rect & position, const SDL_Rect & atlas)
{
	rect_position = position;
	rect_atlas = atlas;
}

UIImage::UIImage(UIComponent_TYPE type) : UIComponents(type)
{

}

void UIImage::Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h)
{
	rect_position.x = pos_x;
	rect_position.y = pos_y;
	rect_position.w = pos_w;
	rect_position.h = pos_h;

	rect_atlas.x = atlas_x;
	rect_atlas.y = atlas_y;
	rect_atlas.w = atlas_w;
	rect_atlas.h = atlas_h;
}

void UIImage::Set(const SDL_Rect & position, const SDL_Rect & atlas)
{
	rect_position = position;
	rect_atlas = atlas;
}

UISelectOption::UISelectOption(UIComponent_TYPE type) : UIComponents(type)
{
	title = (UILabel*)App->uimanager->addUIComponent(UIComponent_TYPE::UILABEL);
}

void UISelectOption::Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h)
{
	rect_position.x = pos_x;
	rect_position.y = pos_y;
	rect_position.w = pos_w;
	rect_position.h = pos_h;

	rect_atlas.x = atlas_x;
	rect_atlas.y = atlas_y;
	rect_atlas.w = atlas_w;
	rect_atlas.h = atlas_h;
}

void UISelectOption::Set(const SDL_Rect & position, const SDL_Rect & atlas)
{
	rect_position = position;
	rect_atlas = atlas;
}

void UISelectOption::AddOption(const char * text)
{
	UILabel* newoption = (UILabel*)App->uimanager->addUIComponent(UIComponent_TYPE::UILABEL);

	num_options++;

	newoption->Set(rect_position.x + 2 , (rect_position.y + (rect_position.h/4)) + (num_options*rect_position.h), text);

	newoption->draw = false;

	newoption->from = this;

	if (current == nullptr)
		ChangeCurrent(newoption);
	else
		options.push_back(newoption);
}

const char * UISelectOption::CheckSelected()
{
	return current->text.c_str();
}

void UISelectOption::ChangeCurrent(UILabel * change)
{
	if (current == nullptr)
	{
		current = change;
		current->rect_position.x = rect_position.x;
		current->rect_position.y = rect_position.y;
		current->draw = true;
	}
	else
	{
		std::list<UILabel*>::iterator item = options.begin();

		while (item._Ptr->_Myval != change)
		{
			item++;
		}

		options.erase(item);
		

		SWAP<int>(current->rect_position.x, change->rect_position.x);
		SWAP<int>(current->rect_position.y, change->rect_position.y);

		options.push_back(current);


		current = change;
		current->draw = true;
	}
}

void UISelectOption::ChangeDrawAllOptions()
{
	std::list<UILabel*>::iterator item;
	item = options.begin();

	while(item != options.end())
	{
		if (item._Ptr->_Myval->draw)
			item._Ptr->_Myval->draw = false;
		else
			item._Ptr->_Myval->draw = true;

		item++;
	}
}

UIHUDPanelButtons::UIHUDPanelButtons(UIComponent_TYPE type) : UIComponents(type)
{
	interactive = false;
}

//x - 0 to 4 | y - 0 to 2 | Max 15 buttons
void UIHUDPanelButtons::AddButton(uint x, uint y, uint atlas_x, uint atlas_y)
{
	info_button* new_btn = new info_button;

	new_btn->btn = (UIButton*)App->uimanager->addUIComponent(UIComponent_TYPE::UIBUTTON);
	new_btn->x = x;
	new_btn->y = y;
	new_btn->btn->Set({ 26 + (30 * (int)x), 666 + (30 * (int)y), 29, 29}, { (int)atlas_x, (int)atlas_y, 25, 25});

	new_btn->btn->from = this;

	panel.push_back(new_btn);
}

UIHUDPanelInfo::UIHUDPanelInfo(UIComponent_TYPE type) : UIComponents(type)
{
	interactive = false;
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

				UIButton* new_btn = new UIButton(UIBUTTON);

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

			entity_selected->image = new UIImage(UIIMAGE);
			
			entity_selected->image->Set({ 231, 667, 29, 33 }, GetUnitIconPositionFromAtlas(selected->GetUnitType()));

			entity_selected->image->interactive = false;

			entity_selected->name = new UILabel(UILABEL);
			entity_selected->name->Set(231, 653, GetUnitName(selected->GetUnitType()));

			std::string stats = std::to_string(selected->GetHp());
			entity_selected->life = new UILabel(UILABEL);
			entity_selected->life->Set(262, 690, stats.c_str());

			stats = std::to_string(selected->GetAttack());
			entity_selected->damage = new UILabel(UILABEL);
			entity_selected->damage->Set(272, 705, stats.c_str());

			stats = std::to_string(selected->GetArmor());
			entity_selected->armor = new UILabel(UILABEL);
			entity_selected->armor->Set(272, 722, stats.c_str());

			if (selected->GetUnitClass() == UNIT_CLASS::ARCHER)
			{
				stats = std::to_string(selected->GetRange());
				entity_selected->range = new UILabel(UILABEL);
				entity_selected->range->Set(272, 745, stats.c_str());
			}
		}
	}
	else
		status = NONE;
}

void UIHUDPanelInfo::DeleteButtons()
{
	if (status == ENTITIESSELECTED)
	{
		std::list<UIButton*>::iterator item;
		item = entities_btn.begin();

		while (item != entities_btn.end())
		{
			delete item._Ptr->_Myval;

			item++;
		}

		entities_btn.clear();
	}
	else if (status == ENTITYINFO)
	{
		delete entity_selected;
	}

}

void UIHUDPanelInfo::Draw()
{
	if (status == ENTITIESSELECTED)
	{
		std::list<UIButton*>::iterator item;
		item = entities_btn.begin();

		while (item != entities_btn.end())
		{
			UIButton* uibutton = item._Ptr->_Myval;

			SDL_Rect mark_btn{ 999, 827, 29, 29 };
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), uibutton->rect_position.x + 2 - App->render->camera.x, uibutton->rect_position.y + 2 - App->render->camera.y, &uibutton->rect_atlas);
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), uibutton->rect_position.x - App->render->camera.x, uibutton->rect_position.y - App->render->camera.y, &mark_btn);

			item++;
		}
	}
	else if (status == ENTITYINFO)
	{
		UIImage* entity_image = entity_selected->image;

		SDL_Rect mark_btn{ 1029, 827, 29, 33 };
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->rect_position.x + 2 - App->render->camera.x, entity_image->rect_position.y + 2 - App->render->camera.y, &entity_image->rect_atlas);
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->rect_position.x - App->render->camera.x, entity_image->rect_position.y - App->render->camera.y, &mark_btn);
	
		UILabel* entity_label = entity_selected->name;
		App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera.x, entity_label->rect_position.y - App->render->camera.y);

		entity_label = entity_selected->life;
		App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera.x, entity_label->rect_position.y - App->render->camera.y);

		SDL_Rect mark_attack{ 956, 858, 38, 22 };
		entity_label = entity_selected->damage;
		App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera.x, entity_label->rect_position.y - App->render->camera.y);
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), 230 - App->render->camera.x, 703 - App->render->camera.y, &mark_attack);

		SDL_Rect mark_armor{ 956, 902, 37, 19 };
		entity_label = entity_selected->armor;
		App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera.x, entity_label->rect_position.y - App->render->camera.y);
		App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), 230 - App->render->camera.x, 725 - App->render->camera.y, &mark_armor);

		entity_label = entity_selected->range;
		if (entity_label != nullptr)
		{
			SDL_Rect mark_range{ 956, 881, 35, 20 };
			App->render->Blit(entity_label->text_img, entity_label->rect_position.x - App->render->camera.x, entity_label->rect_position.y - App->render->camera.y);
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), 231 - App->render->camera.x, 744 - App->render->camera.y, &mark_range);
		}


	}

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
