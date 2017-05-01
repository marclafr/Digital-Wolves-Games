#ifndef __j1GUIMANAGER_H__
#define __j1GUIMANAGER_H__

#include "j1Module.h"
#include <list>
#include "SDL\include\SDL_rect.h"

class UIComponents;
enum UICOMPONENT_TYPE;
struct SDL_Texture;
union SDL_Event;
class UIHUDTownHallBarLife;
class UIHUDResources;
class UIHUDPanelInfo;
class UIHUDDescription;
struct info_button;
class Building;
class Resources;
class Entity;

#define CURSOR_WIDTH 2

class j1UIManager : public j1Module
{
public:

	j1UIManager();

	// Destructor
	virtual ~j1UIManager();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Update all guis
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	//Handle inputs
	void HandleInput(SDL_Event);

	// Called before quitting
	bool CleanUp();

	// Gui creation functions
	UIComponents* addUIComponent(UICOMPONENT_TYPE type);

	const SDL_Texture* GetAtlas() const;
	const std::list<UIComponents*>::iterator GetLastComponent();
	const bool InUse() const;
	void SetAllToDelete();

	void SetTownHall(Building*);
	void SetResource(Resources*);
	void AddEntityToPanelInfo(Entity*);
	void DefineSelectionPanelInfo();
	bool IsSelectionEmptyFromPanelInfo();
	void DeleteSelectionPanelInfo();
	void SetDescriptionHUDDescription(info_button*);
	void SetInfoUIComponent(UIComponents*);
	SDL_Rect GetPosRectFromInfoUI();

private:
	std::list<UIComponents*> components;
	SDL_Texture* atlas;
	std::string atlas_file_name;
	UIComponents* focus = nullptr;

	//Pointers of some compontnts from scenes
	UIHUDTownHallBarLife* townhall_bar_life;
	UIHUDResources* resources_panel;
	UIHUDPanelInfo* panel_info;
	UIHUDDescription* hud_description;
	UIComponents* info_ui;
};
#endif // __j1GUIMANAGER_H__
