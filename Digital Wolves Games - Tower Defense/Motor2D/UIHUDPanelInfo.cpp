#define BLACK {0,0,0,0}

#define BAR_LIFE_CENTER 16

#include "UIHUDPanelInfo.h"

#include "j1App.h"
#include "j1UIManager.h"
#include "j1EntityManager.h"

#include "SDL\include\SDL_rect.h"

#include "UIGetEntitiesInfo.h"

#include "PanelInfoGroupSelection.h"
#include "PanelInfoOneSelection.h"

UIHUDPanelInfo::UIHUDPanelInfo(UICOMPONENT_TYPE type) : UIComponents(type)
{
	SetInteractive(false);
}

UIHUDPanelInfo::~UIHUDPanelInfo()
{}

void UIHUDPanelInfo::DeleteSelection()
{
	if (actual_panelinfo != nullptr)
		DELETE_PTR(actual_panelinfo);
}

void UIHUDPanelInfo::CreatePanel(const std::vector<Entity*>& selection)
{
	if (selection.empty() == false)
	{
		if (selection.size() > 1)
			actual_panelinfo = new GroupSelection();
		else
			actual_panelinfo = new OneSelection();
		actual_panelinfo->Prepare();
	}
}

bool UIHUDPanelInfo::Update()
{
	if (actual_panelinfo != nullptr)
	{
		actual_panelinfo->Update();

		if (actual_panelinfo->GetEntityForOneSelected() != nullptr && !actual_panelinfo->if_ToDelete())
		{
			//DefineSelection();// (actual_panelinfo->GetEntityForOneSelected());
		}

		if (actual_panelinfo->if_ToDelete())
			DeleteSelection();
	}

	return true;
}