#define Y_LABEL 1

#include "UIHUDResources.h"

#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1UIManager.h"
#include "Camera.h"

#include "Resources.h"

#include "SDL\include\SDL_rect.h"

#include "UILabel.h"

#include "UIGetEntitiesInfo.h"

UIHUDResources::UIHUDResources(UICOMPONENT_TYPE type) : UIComponents(type) 
{
	SetInteractive(false);
	stone = App->uimanager->AddLabel(185, Y_LABEL, "0");
	food = App->uimanager->AddLabel(76, Y_LABEL, "0");
	wood = App->uimanager->AddLabel(21, Y_LABEL, "0");
	gold = App->uimanager->AddLabel(143, Y_LABEL, "0");
}

UIHUDResources::~UIHUDResources()
{
	entities_resources.clear();
}

void UIHUDResources::AddResource(Resources * res)
{
	entities_resources.push_back(res);
}

bool UIHUDResources::Update()
{
	UpdateResourcesNum();
	UpdateLabels();
	return true;
}

void UIHUDResources::UpdateResourcesNum()
{
	num_stone = 0;
	num_food = 0;
	num_gold = 0;
	num_wood = 0;

	std::list<Resources*>::iterator item;
	item = entities_resources.begin();

	while (item != entities_resources.end())
	{
		Resources* resource = item._Ptr->_Myval;
		switch (resource->GetResourceType())
		{
		case STONE:
			num_stone += resource->GetResource();
			break;
		case WOOD:
			num_wood += resource->GetResource();
			break;
		case FOOD:
			num_food += resource->GetResource();
			break;
		case GOLD:
			num_gold += resource->GetResource();
			break;
		}
		item++;
	}
}

void UIHUDResources::UpdateLabels()
{
	std::string num_stone_str = std::to_string(num_stone);
	stone->ChangeText(num_stone_str.c_str());

	std::string num_food_str = std::to_string(num_food);
	food->ChangeText(num_food_str.c_str());

	std::string num_gold_str = std::to_string(num_gold);
	gold->ChangeText(num_gold_str.c_str());

	std::string num_wood_str = std::to_string(num_wood);
	wood->ChangeText(num_wood_str.c_str());
}