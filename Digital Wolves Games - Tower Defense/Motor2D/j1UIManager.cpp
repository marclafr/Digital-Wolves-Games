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
#include "UIHUDResources.h"
#include "UIHUDTownHallBarLife.h"
#include "UIHUDScoreBar.h"

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
	int x_mouse = 0;
	int y_mouse = 0;

	App->input->GetMousePosition(x_mouse, y_mouse);

	for (std::list<UIComponents*>::iterator item = components.begin(); item != components.end(); ++item)
	{
		if ((*item)->GetInteractive() &&
			(x_mouse > (*item)->GetPosRect().x) &&
			(x_mouse < (*item)->GetPosRect().x + (*item)->GetPosRect().w) &&
			(y_mouse >(*item)->GetPosRect().y) &&
			(y_mouse < (*item)->GetPosRect().y + (*item)->GetPosRect().h))
		{
			if (focus != nullptr)
				focus->SetIsFocus(false);
			focus = (*item);
			focus->SetIsFocus(true);
			break;
		}
	}
	return true;
}

// Update all guis
bool j1UIManager::Update(float dt)
{
	bool ret = false;

	for (std::list<UIComponents*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		ret = (*it)->Update();
		(*it)->Draw();

		if (ret == false)
			break;
	}

	return ret;
}

// Called after all Updates
bool j1UIManager::PostUpdate()
{
	for (std::list<UIComponents*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->ToDelete())
		{
			delete *it;
			components.erase(it);
		}
	}
	return true;
}

void j1UIManager::HandleInput(SDL_Event event)
{
	focus->HandleInput(event);
}

// Called before quitting
bool j1UIManager::CleanUp()
{
	LOG("Freeing GUI");

	std::list<UIComponents*>::iterator item;
	item = components.begin();

	while (item != components.end())
	{
		delete *item;
		
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
		/*
	case UISELECTOPTION:
		components.push_back(ret = (UIComponents*)new UISelectOption(UICOMPONENT_TYPE::UISELECTOPTION));
		break;
		*/
	case UIHUDPANELBUTTONS:
		components.push_back(ret = (UIComponents*)new UIHUDPanelButtons(UICOMPONENT_TYPE::UIHUDPANELBUTTONS));
		break;
	case UIHUDPANELINFO:
		components.push_back(ret = (UIComponents*)new UIHUDPanelInfo(UICOMPONENT_TYPE::UIHUDPANELINFO));
		break;
	case UIHUDDESCRIPTION:
		components.push_back(ret = (UIComponents*)new UIHUDDescription(UICOMPONENT_TYPE::UIHUDDESCRIPTION));
		break;
	case UIHUDRESOURCES:
		components.push_back(ret = (UIComponents*)new UIHUDResources(UICOMPONENT_TYPE::UIHUDRESOURCES));
		break;
	case UIHUDTOWNHALLBARLIFE:
		components.push_back(ret = (UIComponents*)new UIHUDTownHallBarLife(UICOMPONENT_TYPE::UIHUDTOWNHALLBARLIFE));
		break;
	case UIHUDSCOREBAR:
		components.push_back(ret = (UIComponents*)new UIHUDScoreBar(UICOMPONENT_TYPE::UIHUDSCOREBAR));
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

void j1UIManager::erase_list(std::list<UIComponents*>::iterator first, std::list<UIComponents*>::iterator last)
{
	first_item_delete = first;

	last_item_delete = last;
}

const std::list<UIComponents*>::iterator j1UIManager::GetLastComponent()
{
	std::list<UIComponents*>::iterator temp = components.end();

	--temp;

	return temp;
}

const bool j1UIManager::InUse() const
{
	if (focus != nullptr)
		return true;
	return false;
}