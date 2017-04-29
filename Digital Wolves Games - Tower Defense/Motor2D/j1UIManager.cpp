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
			(y_mouse > (*item)->GetPosRect().y) &&
			(y_mouse < (*item)->GetPosRect().y + (*item)->GetPosRect().h))
		{
			focus = (*item);
			focus->SetIsFocus(true);
			break;
		}
		else
			(*item)->SetIsFocus(false);
	}
	return true;
}

// Update all guis
bool j1UIManager::Update(float dt)
{
	for (std::list<UIComponents*>::iterator it = components.begin(); it != components.end(); ++it)
		if (!(*it)->Update())
			return false;

	return true;
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
	case UIT_UILABEL:
		components.push_back(ret = (UIComponents*)new UILabel(UICOMPONENT_TYPE::UIT_UILABEL));
		break;
	case UIT_UIBUTTON:
		components.push_back(ret = (UIComponents*)new UIButton(UICOMPONENT_TYPE::UIT_UIBUTTON));
		break;
	case UIT_UICHECKBUTTON:
		components.push_back(ret = (UIComponents*)new UICheckbutton(UICOMPONENT_TYPE::UIT_UICHECKBUTTON));
		break;
		/*
	case UIT_UISELECTOPTION:
		components.push_back(ret = (UIComponents*)new UISelectOption(UICOMPONENT_TYPE::UIT_UISELECTOPTION));
		break;
		*/
	case UIT_UIHUDPANELBUTTONS:
		components.push_back(ret = (UIComponents*)new UIHUDPanelButtons(UICOMPONENT_TYPE::UIT_UIHUDPANELBUTTONS));
		break;
	case UIT_UIHUDPANELINFO:
		components.push_back(ret = (UIComponents*)new UIHUDPanelInfo(UICOMPONENT_TYPE::UIT_UIHUDPANELINFO));
		panel_info = (UIHUDPanelInfo*)ret;
		break;
	case UIT_UIHUDDESCRIPTION:
		components.push_back(ret = (UIComponents*)new UIHUDDescription(UICOMPONENT_TYPE::UIT_UIHUDDESCRIPTION));
		hud_description = (UIHUDDescription*)ret;
		break;
	case UIT_UIHUDRESOURCES:
		components.push_back(ret = (UIComponents*)new UIHUDResources(UICOMPONENT_TYPE::UIT_UIHUDRESOURCES));
		resources_panel = (UIHUDResources*)ret;
		break;
	case UIT_UIHUDTOWNHALLBARLIFE:
		components.push_back(ret = (UIComponents*)new UIHUDTownHallBarLife(UICOMPONENT_TYPE::UIT_UIHUDTOWNHALLBARLIFE));
		townhall_bar_life = (UIHUDTownHallBarLife*)ret;
		break;
	case  UIT_UIHUDSCOREBAR:
		components.push_back(ret = (UIComponents*)new UIHUDScoreBar(UICOMPONENT_TYPE:: UIT_UIHUDSCOREBAR));
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

void j1UIManager::SetTownHall(Building * th)
{
	townhall_bar_life->SetTownHall(th);
}

void j1UIManager::SetResource(Resources* res)
{
	resources_panel->AddResource(res);
}

void j1UIManager::AddEntityToPanelInfo(Entity* entity_selected)
{
	panel_info->AddEntitySelection(entity_selected);
}

void j1UIManager::DefineSelectionPanelInfo()
{
	panel_info->DefineSelection();
}

bool j1UIManager::IsSelectionEmptyFromPanelInfo()
{
	return panel_info->isSelectionEmpty();
}

void j1UIManager::DeleteSelectionPanelInfo()
{
	panel_info->DeleteSelection();
}

void j1UIManager::SetDescriptionHUDDescription(info_button* infobutton_desc)
{
	hud_description->SetDescription(infobutton_desc);
}

void j1UIManager::SetInfoUIComponent(UIComponents* component)
{
	info_ui = component;
}

SDL_Rect j1UIManager::GetPosRectFromInfoUI()
{
	return info_ui->GetPosRect();
}
