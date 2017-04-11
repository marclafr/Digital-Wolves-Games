#ifndef __j1GUIMANAGER_H__
#define __j1GUIMANAGER_H__

#include "j1Module.h"


#include <list>
#include "SDL\include\SDL_rect.h"

class UIComponents;
enum UICOMPONENT_TYPE;
struct SDL_Texture;

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

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Gui creation functions
	UIComponents* addUIComponent(UICOMPONENT_TYPE type, std::list<UIComponents*>::iterator* ref = nullptr);

	const SDL_Texture* GetAtlas() const;

	void erase_list(std::list<UIComponents*>::iterator* first, std::list<UIComponents*>::iterator* last);

private:

	std::list<UIComponents*> components;

	SDL_Texture* atlas;
	std::string atlas_file_name;
};
#endif // __j1GUIMANAGER_H__
