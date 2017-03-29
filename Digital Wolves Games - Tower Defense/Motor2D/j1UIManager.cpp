#define RIGHT_CLICK 3
#define LEFT_CLICK 1

#include "j1UIManager.h"

#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1FileSystem.h"
#include "Camera.h"

#include "UIComponents.h"
#include "UIButton.h"
#include "UILabel.h"
#include "UICheckbutton.h"
#include "UISelectOption.h"
#include "UIHUDPanelButtons.h"
#include "UIHUDPanelInfo.h"
#include "UIHUDDescription.h"

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
	atlas = App->tex->Load(atlas_file_name.c_str(), T_UI);

	return true;
}

// Update all guis

bool j1UIManager::PreUpdate()
{
	bool ret = false;

	int x_mouse = 0;
	int y_mouse = 0;

	App->input->GetMousePosition(x_mouse, y_mouse);

	std::list<UIComponents*>::iterator item;
	item = components.begin();

	while (item != components.end())
	{
		UIComponents* component = item._Ptr->_Myval;

		if (component->GetDraw() && component->GetInteractive())
		{
			if ((x_mouse > component->rect_position.x) && (x_mouse < component->rect_position.x + component->rect_position.w) && (y_mouse > component->rect_position.y) && (y_mouse < component->rect_position.y + component->rect_position.h))
			{
				component->stat = UICOMPONENT_STAT::SELECTED;

				if (App->input->GetMouseButtonDown(LEFT_CLICK) == KEY_DOWN)
				{
					if (component->stat == UICOMPONENT_STAT::SELECTED)
					{
						component->stat = UICOMPONENT_STAT::CLICKL_DOWN;
					}
					else
						component->stat = UICOMPONENT_STAT::CLICKL_REPEAT;
				}
				else if (App->input->GetMouseButtonDown(LEFT_CLICK) == KEY_UP)
				{
					if (component->stat == UICOMPONENT_STAT::SELECTED)
						component->stat = UICOMPONENT_STAT::CLICKL_UP;
				}

				if (App->input->GetMouseButtonDown(RIGHT_CLICK) == KEY_DOWN)
				{
					if (component->stat == UICOMPONENT_STAT::SELECTED)
						component->stat = UICOMPONENT_STAT::CLICKR_DOWN;
					else
						component->stat = UICOMPONENT_STAT::CLICKR_REPEAT;
				}
				else if (App->input->GetMouseButtonDown(RIGHT_CLICK) == KEY_UP)
				{
					if (component->stat = UICOMPONENT_STAT::SELECTED)
						component->stat = UICOMPONENT_STAT::CLICKR_UP;
				}
			}
			else
				component->stat = UICOMPONENT_STAT::UNSELECTED;
		}
		item++;
	}

	item = components.begin();

	while (item != components.end())
	{
		ret = item._Ptr->_Myval->Update();

		item++;
	}

	return ret;
}


// Called after all Updates
bool j1UIManager::PostUpdate()
{
	std::list<UIComponents*>::iterator item;
	item = components.begin();

	while (item != components.end())
	{
		if(item._Ptr->_Myval->GetDraw())
			item._Ptr->_Myval->Draw();

		item++;
	}
	
	return true;
}

// Called before quitting
bool j1UIManager::CleanUp()
{
	LOG("Freeing GUI");

	std::list<UIComponents*>::iterator item;
	item = components.begin();

	while (item != components.end())
	{
		delete item._Ptr->_Myval;
		
		item++;
	}

	components.clear();

	return true;
}

UIComponents* j1UIManager::addUIComponent(UICOMPONENT_TYPE type)
{
	UIComponents* ret = nullptr;

	switch (type)
	{
	case UILABEL:
		components.push_back(ret = (UIComponents*)new UILabel(UICOMPONENT_TYPE::UILABEL));
		break;
	case UIBUTTON:
		components.push_back(ret = (UIComponents*)new UIButton(UICOMPONENT_TYPE::UIBUTTON));
		break;
	case UICHECKBUTTON:
		components.push_back(ret = (UIComponents*)new UICheckbutton(UICOMPONENT_TYPE::UICHECKBUTTON));
		break;
	case UISELECTOPTION:
		components.push_back(ret = (UIComponents*)new UISelectOption(UICOMPONENT_TYPE::UISELECTOPTION));
		break;
	case UIHUDPANELBUTTONS:
		components.push_back(ret = (UIComponents*)new UIHUDPanelButtons(UICOMPONENT_TYPE::UIHUDPANELBUTTONS));
		break;
	case UIHUDPANELINFO:
		components.push_back(ret = (UIComponents*)new UIHUDPanelInfo(UICOMPONENT_TYPE::UIHUDPANELINFO));
		break;
	case UIHUDDESCRIPTION:
		components.push_back(ret = (UIComponents*)new UIHUDDescription(UICOMPONENT_TYPE::UIHUDDESCRIPTION));
		break;
	default:
		components.push_back(ret = new UIComponents(type));
		break;
	}
	
	return ret;
}

// const getter for atlas
const SDL_Texture* j1UIManager::GetAtlas() const
{
	return atlas;
}