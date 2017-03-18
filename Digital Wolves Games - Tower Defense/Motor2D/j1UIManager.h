#ifndef __d1GUIMANAGER_H__
#define __d1GUIMANAGER_H__

#include "j1Module.h"

#include <list>

struct _TTF_Font;

#define CURSOR_WIDTH 2

class Entity;
enum UNIT_TYPE;

// TODO 1: Create your structure of classes

enum UIComponent_Stat
{
	SELECTED,
	UNSELECTED,

	CLICKL_DOWN,
	CLICKR_DOWN,

	CLICKL_REPEAT,
	CLICKR_REPEAT,

	CLICKL_UP,
	CLICKR_UP
};

enum UIComponent_TYPE
{
	UILABEL,
	UIBUTTON,
	UIINPUT,
	UIIMAGE,
	UICHECKBUTTON,
	UISELECTOPTION,
	UIHUDPANELBUTTONS,
	UIHUDPANELINFO
};

class UIComponents
{
public:
	SDL_Rect rect_position;
	SDL_Rect rect_atlas;

	UIComponent_TYPE type;

	UIComponent_Stat stat = UIComponent_Stat::UNSELECTED;

	bool draw = true;

	bool interactive = true;

	UIComponents* from = nullptr;
public:
	UIComponents(UIComponent_TYPE type) : type(type) {}

	virtual void Set() {};
};

//Label
class UILabel : public UIComponents
{
public:
	std::string text;
	SDL_Texture* text_img;
	_TTF_Font*  font;

public:
	UILabel(UIComponent_TYPE type) : UIComponents(type) {}

	void Set(int pos_x, int pos_y, const char* text, _TTF_Font*  font = nullptr);

	void ChangeText(const char* text);
};
//------

//Image
class UIImage : public UIComponents
{
public:
	UIImage(UIComponent_TYPE type);

	void Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h);
	void Set(const SDL_Rect& position, const SDL_Rect& atlas);
};
//------

//Buttons
class UIButton : public UIComponents
{
public:
	bool clicked = false;
	UILabel* title;

public:
	UIButton(UIComponent_TYPE type);

	void Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h);
	void Set(const SDL_Rect& position, const SDL_Rect& atlas);
};

class UICheckbutton : public UIButton
{
public:
	SDL_Rect rect_atlas_clicked;

public:
	UICheckbutton(UIComponent_TYPE type);
	void Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h, uint atlas_clicked_x, uint atlas_clicked_y, uint atlas_clicked_w, uint atlas_clicked_h);
	void Set(const SDL_Rect& position, const SDL_Rect& atlas, const SDL_Rect & atlas_clicked);
};
//------

//Input
class UIInput : public UIComponents
{
public:
	std::string input_text;
	UILabel* title;

public:
	UIInput(UIComponent_TYPE type);

	void Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h);
	void Set(const SDL_Rect& position, const SDL_Rect& atlas);

	const char* GetStr();
};
//------

class UISelectOption : public UIComponents
{
public:
	UILabel* title;

	UILabel* current = nullptr;
	std::list<UILabel*> options;

	bool selecting = false;

	int num_options = -1;

public:
	UISelectOption(UIComponent_TYPE type);

	void Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h);
	void Set(const SDL_Rect& position, const SDL_Rect& atlas);

	void AddOption(const char* text);
	const char* CheckSelected();
	void ChangeCurrent(UILabel* change);

	void ChangeDrawAllOptions();
};
//------

class UIHUDPanelButtons : public UIComponents
{
public:
	struct info_button
	{
		UIButton* btn;
		uint x;
		uint y;
	};

	std::list<info_button*> panel;

public:
	UIHUDPanelButtons(UIComponent_TYPE type);

	//x - 0 to 4 | y - 0 to 2 | Max 15 buttons
	void AddButton(uint x, uint y, uint atlas_x, uint atlas_y);
};

//------

class UIHUDPanelInfo : public UIComponents
{
private:
	enum PANEL_TYPE
	{
		ENTITIESSELECTED,
		ENTITYINFO,
		NONE
	};

	struct entity_info
	{
		UIImage* image = nullptr;

		UILabel* name = nullptr;

		UILabel* life = nullptr;
		UILabel* damage = nullptr;
		UILabel* armor = nullptr;
		UILabel* range = nullptr;

		~entity_info()
		{
			delete image;
			delete name;
			delete life;
			delete damage;
			delete armor;
			delete range;
		}

	};

	std::list<Entity*> selection;

	std::list<UIButton*>  entities_btn;

	entity_info* entity_selected;

	PANEL_TYPE status = NONE;

public:
	UIHUDPanelInfo(UIComponent_TYPE type);

	void AddEntitySelection(Entity* selected);
	void DeleteSelection();

	void CreatePanel();
	void DeleteButtons();
	bool isSelectionEmpty();

	void Draw();

	SDL_Rect GetUnitIconPositionFromAtlas(const UNIT_TYPE type);

	const char* GetUnitName(const UNIT_TYPE type);

};

// ---------------------------------------------------
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

	// TODO 2: Create the factory methods
	// Gui creation functions

	UIComponents* addUIComponent(UIComponent_TYPE type);

	const SDL_Texture* GetAtlas() const;

private:

	std::list<UIComponents*> components;

	SDL_Texture* atlas;
	std::string atlas_file_name;

private:
	void drawAllComponents();
};
#endif // __d1GUIMANAGER_H__
